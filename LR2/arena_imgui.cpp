#include "arena_imgui.h"
#include "arena_imgui_gui.h"
#include "arena_maniac.h"

#include <DxLib.h>
#include <DxFunctionWin.h>

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <implot.h>
#include <ImGuiNotify.hpp>

#include <d3d9.h>
#include <d3d11.h>
#include <windowsx.h>

#include <algorithm>
#include <cmath>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace openlr2::arena_imgui {
namespace {

enum class BackendKind { None, Dx9, Dx11 };

bool gInited{};
bool gAvailable{};
BackendKind gBackend{BackendKind::None};
bool gDeviceObjectsValid{};
void* gLastDevice9{};
ID3D11Device* gLastDevice11{};
ID3D11DeviceContext* gLastContext11{};
ID3D11RenderTargetView* gRtv11{};

/** Skin/backbuffer size (ImGui space). Updated each frame; used by WndProc mouse scale. */
int gCanvasW{};
int gCanvasH{};
/** Client / present size (OS mouse space). */
int gClientW{};
int gClientH{};
/** only feed ImGui from WndProc while lobby/graph is open (avoids eating game UI). */
bool gOverlayWantsInput{};
bool gMenuVisible{};
bool gMuteGameInputs{};
bool gWaitingForKeyBind{};
ImFont* gBigIconFont{};

void ReleaseRtv11() {
	if (gRtv11 != nullptr) {
		gRtv11->Release();
		gRtv11 = nullptr;
	}
}

bool EnsureRtv11(ID3D11Device* device) {
	ReleaseRtv11();
	const void* bb = GetUseDirect3D11BackBufferRenderTargetView();
	if (bb != nullptr) {
		gRtv11 = const_cast<ID3D11RenderTargetView*>(
			static_cast<const ID3D11RenderTargetView*>(bb)
		);
		gRtv11->AddRef();
		return true;
	}
	const void* tex = GetUseDirect3D11BackBufferTexture2D();
	if (tex == nullptr || device == nullptr) return false;
	auto* texture = const_cast<ID3D11Texture2D*>(static_cast<const ID3D11Texture2D*>(tex));
	HRESULT hr = device->CreateRenderTargetView(texture, nullptr, &gRtv11);
	return SUCCEEDED(hr) && gRtv11 != nullptr;
}

LRESULT CALLBACK ArenaImGuiWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (!gAvailable || !gOverlayWantsInput) {
		SetUseHookWinProcReturnValue(FALSE);
		return 0;
	}
	LPARAM imguiLParam = lParam;
	if (msg == WM_MOUSEMOVE || msg == WM_NCMOUSEMOVE) {
		// LR2OOL: map OS client mouse → skin canvas (HD windowsize ≠ GraphMode).
		RECT r{};
		if (::GetClientRect(hWnd, &r) != 0) {
			const int clientW = r.right - r.left;
			const int clientH = r.bottom - r.top;
			const int canvasW = gCanvasW;
			const int canvasH = gCanvasH;
			if (clientW > 0 && clientH > 0 && canvasW > 0 && canvasH > 0
				&& !(clientW == canvasW && clientH == canvasH)) {
				const float scaleX = static_cast<float>(canvasW) / static_cast<float>(clientW);
				const float scaleY = static_cast<float>(canvasH) / static_cast<float>(clientH);
				float mx = static_cast<float>(GET_X_LPARAM(lParam)) * scaleX;
				float my = static_cast<float>(GET_Y_LPARAM(lParam)) * scaleY;
				const float canvasAr = static_cast<float>(canvasW) / static_cast<float>(canvasH);
				const float clientAr = static_cast<float>(clientW) / static_cast<float>(clientH);
				if (std::fabs(canvasAr - clientAr) > 0.001f) {
					const bool pillar = canvasAr > clientAr;
					float& pos = pillar ? my : mx;
					const float outSize = static_cast<float>(pillar ? clientH : clientW);
					const float scale = outSize / (outSize / canvasAr);
					const float scalePos = pillar ? scaleY : scaleX;
					pos *= scale;
					pos -= (outSize - outSize / canvasAr) * 0.5f * scalePos * scale;
				}
				const auto ix = static_cast<SHORT>(std::clamp(static_cast<int>(mx), -32768, 32767));
				const auto iy = static_cast<SHORT>(std::clamp(static_cast<int>(my), -32768, 32767));
				imguiLParam = MAKELPARAM(ix, iy);
			}
		}
	}
	// always feed ImGui while overlay open; mute swallows game-bound msgs.
	ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, imguiLParam);
	bool muteMsg = false;
	if (gMenuVisible && gMuteGameInputs) {
		// mute game-bound window messages while overlay open
		switch (msg) {
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_MOUSEHWHEEL:
		case WM_MOUSEWHEEL:
		case WM_SETCURSOR:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
			muteMsg = true;
			break;
		default:
			break;
		}
	}
	if (muteMsg) {
		SetUseHookWinProcReturnValue(TRUE);
		return 1;
	}
	SetUseHookWinProcReturnValue(FALSE);
	return 0;
}

} // namespace

bool Init() {
	if (gInited) return gAvailable;
	gInited = true;
	gAvailable = false;

	if (GetUse3DFlag() == FALSE) {
		return false;
	}
	const int ver = GetUseDirect3DVersion();
	if (ver == DX_DIRECT3D_NONE) {
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// NavEnableKeyboard left commented out.
	io.IniFilename = "LR2files/Config/imgui.ini";

	SetupImGuiStyle();

	// SetupFonts: no embedded Noto/FA.
	// GlyphRanges must stay alive until atlas Build (ImGui stores the pointer).
	// Do NOT pair GetGlyphRangesJapanese() with segoeui (or any missing CJK font).
	{
		int dx = 0, dy = 0;
		GetDrawScreenSize(&dx, &dy);
		const float mainSize = (dy > 0 && dy < 720) ? 12.0f : 16.0f;
		struct Candidate {
			const char* path;
			const ImWchar* (*rangesFn)(ImFontAtlas*);
		};
		const Candidate cjk[] = {
			{"C:\\Windows\\Fonts\\meiryo.ttc", [](ImFontAtlas* a) { return a->GetGlyphRangesJapanese(); }},
			{"C:\\Windows\\Fonts\\YuGothM.ttc", [](ImFontAtlas* a) { return a->GetGlyphRangesJapanese(); }},
			{"C:\\Windows\\Fonts\\msgothic.ttc", [](ImFontAtlas* a) { return a->GetGlyphRangesJapanese(); }},
			{"C:\\Windows\\Fonts\\malgun.ttf", [](ImFontAtlas* a) { return a->GetGlyphRangesKorean(); }},
		};
		ImFont* font = nullptr;
		for (const Candidate& c : cjk) {
			if (c.path == nullptr || c.path[0] == '\0') continue;
			const DWORD attr = GetFileAttributesA(c.path);
			if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY) != 0) continue;
			font = io.Fonts->AddFontFromFileTTF(c.path, mainSize, nullptr, c.rangesFn(io.Fonts));
			if (font != nullptr) break;
		}
		if (font == nullptr) {
			io.Fonts->AddFontDefault();
		}
		io.Fonts->Build();
		gBigIconFont = nullptr;
	}

	HWND hwnd = GetMainWindowHandle();
	if (hwnd == nullptr || !ImGui_ImplWin32_Init(hwnd)) {
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
		return false;
	}

	bool rendererOk = false;
	if (ver == DX_DIRECT3D_11) {
		auto* device = const_cast<ID3D11Device*>(
			static_cast<const ID3D11Device*>(GetUseDirect3D11Device())
		);
		auto* ctx = const_cast<ID3D11DeviceContext*>(
			static_cast<const ID3D11DeviceContext*>(GetUseDirect3D11DeviceContext())
		);
		if (device != nullptr && ctx != nullptr && EnsureRtv11(device)) {
			if (ImGui_ImplDX11_Init(device, ctx)) {
				gBackend = BackendKind::Dx11;
				gLastDevice11 = device;
				gLastContext11 = ctx;
				gDeviceObjectsValid = true;
				rendererOk = true;
			} else {
				ReleaseRtv11();
			}
		}
	} else if (ver == DX_DIRECT3D_9 || ver == DX_DIRECT3D_9EX) {
		auto* device = static_cast<IDirect3DDevice9*>(const_cast<void*>(GetUseDirect3DDevice9()));
		if (device != nullptr && ImGui_ImplDX9_Init(device)) {
			gBackend = BackendKind::Dx9;
			gLastDevice9 = device;
			gDeviceObjectsValid = true;
			rendererOk = true;
		}
	}
	if (!rendererOk) {
		ImGui_ImplWin32_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
		return false;
	}

	SetHookWinProc(ArenaImGuiWndProc);
	gAvailable = true;
	return true;
}

void Shutdown() {
	if (!gInited) return;
	if (gAvailable) {
		NotifyDeviceLost();
		ReleaseRtv11();
		if (gBackend == BackendKind::Dx9) {
			ImGui_ImplDX9_Shutdown();
		} else if (gBackend == BackendKind::Dx11) {
			ImGui_ImplDX11_Shutdown();
		}
		gBackend = BackendKind::None;
		gLastDevice9 = nullptr;
		gLastDevice11 = nullptr;
		gLastContext11 = nullptr;
		gDeviceObjectsValid = false;
		ImGui_ImplWin32_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
		SetHookWinProc(nullptr);
	}
	gAvailable = false;
	gInited = false;
	gOverlayWantsInput = false;
	gCanvasW = gCanvasH = gClientW = gClientH = 0;
}

void NotifyDeviceLost() {
	if (!gAvailable) return;
	if (gBackend == BackendKind::Dx9 && gDeviceObjectsValid) {
		ImGui_ImplDX9_InvalidateDeviceObjects();
		gDeviceObjectsValid = false;
	} else if (gBackend == BackendKind::Dx11) {
		ReleaseRtv11();
		gDeviceObjectsValid = false;
	}
}

void NotifyDeviceReset() {
	if (!gAvailable) return;
	if (gBackend == BackendKind::Dx9) {
		auto* device = static_cast<IDirect3DDevice9*>(const_cast<void*>(GetUseDirect3DDevice9()));
		if (device == nullptr) return;
		if (device != gLastDevice9) {
			ImGui_ImplDX9_Shutdown();
			if (!ImGui_ImplDX9_Init(device)) {
				gAvailable = false;
				return;
			}
			gLastDevice9 = device;
		}
		// on failed device reset keep invalid (do not Create device objects).
		if (!ImGui_ImplDX9_CreateDeviceObjects()) {
			gDeviceObjectsValid = false;
			return;
		}
		gDeviceObjectsValid = true;
	} else if (gBackend == BackendKind::Dx11) {
		auto* device = const_cast<ID3D11Device*>(
			static_cast<const ID3D11Device*>(GetUseDirect3D11Device())
		);
		auto* ctx = const_cast<ID3D11DeviceContext*>(
			static_cast<const ID3D11DeviceContext*>(GetUseDirect3D11DeviceContext())
		);
		if (device == nullptr || ctx == nullptr) return;
		if (device != gLastDevice11 || ctx != gLastContext11) {
			ImGui_ImplDX11_Shutdown();
			if (!ImGui_ImplDX11_Init(device, ctx)) {
				gAvailable = false;
				return;
			}
			gLastDevice11 = device;
			gLastContext11 = ctx;
		}
		if (!EnsureRtv11(device)) {
			gAvailable = false;
			return;
		}
		gDeviceObjectsValid = true;
	}
}

bool IsAvailable() {
	return gAvailable;
}

bool WantCaptureKeyboard() {
	return gAvailable && ImGui::GetIO().WantCaptureKeyboard;
}

bool WantCaptureMouse() {
	return gAvailable && ImGui::GetIO().WantCaptureMouse;
}

bool IsWaitingForKeyBind() {
	return gWaitingForKeyBind;
}

void SetWaitingForKeyBind(bool waiting) {
	gWaitingForKeyBind = waiting;
}

ImFont* BigIconFont() {
	return gBigIconFont;
}

void DrawFrame(
	game* g,
	const openlr2::ArenaRoomState& state,
	bool& menuVisible,
	bool& graphVisible,
	bool& automaticGraph,
	bool& muteGameInputs,
	bool& itemModeEnabled,
	char* roomIdInput,
	size_t roomIdCap,
	char* chatInput,
	size_t chatCap,
	bool (*joinRoom)(game*, const char*),
	void (*leave)(game*)
) {
	if (!gAvailable) return;

	{
		int dx = 0, dy = 0;
		GetDrawScreenSize(&dx, &dy);
		if (dx <= 0 || dy <= 0) {
			GetWindowSize(&dx, &dy);
		}
		gCanvasW = dx;
		gCanvasH = dy;
		gClientW = 0;
		gClientH = 0;
		if (const HWND hwnd = GetMainWindowHandle(); hwnd != nullptr) {
			RECT r{};
			if (::GetClientRect(hwnd, &r) != 0) {
				gClientW = r.right - r.left;
				gClientH = r.bottom - r.top;
			}
		}
		if (gClientW <= 0 || gClientH <= 0) {
			gClientW = gCanvasW;
			gClientH = gCanvasH;
		}
	}

	// feed WndProc only while menu || graph (items render inside graph).
	gOverlayWantsInput = menuVisible || graphVisible;
	gMenuVisible = menuVisible;
	gMuteGameInputs = muteGameInputs;

	// Soft recover if DxLib recreated the device without notifying us.
	if (gBackend == BackendKind::Dx9) {
		auto* device = static_cast<IDirect3DDevice9*>(const_cast<void*>(GetUseDirect3DDevice9()));
		if (device == nullptr) return;
		if (device != gLastDevice9 || !gDeviceObjectsValid) {
			NotifyDeviceLost();
			NotifyDeviceReset();
			if (!gAvailable || !gDeviceObjectsValid) return;
		}
	} else if (gBackend == BackendKind::Dx11) {
		auto* device = const_cast<ID3D11Device*>(
			static_cast<const ID3D11Device*>(GetUseDirect3D11Device())
		);
		if (device == nullptr || !gDeviceObjectsValid) {
			NotifyDeviceLost();
			NotifyDeviceReset();
			if (!gAvailable || !gDeviceObjectsValid) return;
		}
	}

	ImGui_ImplWin32_NewFrame();
	// force DisplaySize to canvas after Win32 overwrites with GetClientRect.
	if (gCanvasW > 0 && gCanvasH > 0) {
		ImGui::GetIO().DisplaySize = ImVec2(static_cast<float>(gCanvasW), static_cast<float>(gCanvasH));
	}
	if (gBackend == BackendKind::Dx9) {
		ImGui_ImplDX9_NewFrame();
	} else {
		ImGui_ImplDX11_NewFrame();
	}
	ImGui::NewFrame();

	// always NewFrame; draw menu/graph when shown; always notifications.
	if (menuVisible || graphVisible) {
		RenderArenaGui(
			g,
			state,
			menuVisible,
			graphVisible,
			automaticGraph,
			muteGameInputs,
			itemModeEnabled,
			roomIdInput,
			roomIdCap,
			chatInput,
			chatCap,
			joinRoom,
			leave
		);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 0.90f));
	ImGui::RenderNotifications();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);

	ImGui::Render();
	ImDrawData* drawData = ImGui::GetDrawData();
	if (drawData == nullptr || drawData->CmdListsCount <= 0) return;

	if (gBackend == BackendKind::Dx9) {
		ImGui_ImplDX9_RenderDrawData(drawData);
	} else if (gBackend == BackendKind::Dx11 && gLastContext11 != nullptr && gRtv11 != nullptr) {
		gLastContext11->OMSetRenderTargets(1, &gRtv11, nullptr);
		ImGui_ImplDX11_RenderDrawData(drawData);
	}
}

} // namespace openlr2::arena_imgui
