#include "arena_imgui_gui.h"

#include "LR2_arena.h"
#include "arena_backend.h"
#include "arena_gui_util.h"
#include "arena_imgui.h"
#include "arena_keys.h"
#include "arena_maniac.h"
#include "structure.h"

#include <DxLib.h>
#include <ImGuiNotify.hpp>
#include <imgui.h>
#include <implot.h>

#define NOMINMAX
#include <windows.h>
#include <commdlg.h>

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace openlr2::arena_imgui {

void SetupImGuiStyle() {
	// Hazy Dark (ImThemes), Tab* renamed for ImGui 1.91.
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6f;
	style.WindowPadding = ImVec2(5.5f, 8.3f);
	style.WindowRounding = 4.5f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 3.2f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 2.7f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 2.4f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 3.2f;
	style.TabRounding = 3.5f;
	style.TabBorderSize = 1.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.498f, 0.498f, 0.498f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.059f, 0.059f, 0.059f, 0.94f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.078f, 0.078f, 0.078f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.427f, 0.427f, 0.498f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.137f, 0.173f, 0.227f, 0.54f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.212f, 0.255f, 0.302f, 0.4f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.043f, 0.047f, 0.047f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.039f, 0.039f, 0.039f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.078f, 0.082f, 0.090f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.137f, 0.137f, 0.137f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.718f, 0.784f, 0.843f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.478f, 0.525f, 0.573f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.290f, 0.318f, 0.353f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.149f, 0.161f, 0.176f, 0.4f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.137f, 0.145f, 0.157f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.078f, 0.086f, 0.090f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.196f, 0.216f, 0.239f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.165f, 0.176f, 0.192f, 0.8f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.075f, 0.082f, 0.090f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.427f, 0.427f, 0.498f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.239f, 0.325f, 0.424f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.275f, 0.380f, 0.498f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.290f, 0.329f, 0.376f, 0.2f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.239f, 0.298f, 0.369f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.165f, 0.176f, 0.188f, 0.95f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.118f, 0.125f, 0.133f, 0.862f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.329f, 0.408f, 0.502f, 0.8f);
	style.Colors[ImGuiCol_TabSelected] = ImVec4(0.243f, 0.247f, 0.255f, 1.0f);
	style.Colors[ImGuiCol_TabDimmed] = ImVec4(0.067f, 0.102f, 0.145f, 0.972f);
	style.Colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.133f, 0.259f, 0.424f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.608f, 0.608f, 0.608f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.427f, 0.349f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.898f, 0.698f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.188f, 0.188f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.349f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.227f, 0.227f, 0.247f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.259f, 0.588f, 0.976f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.259f, 0.588f, 0.976f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
}

namespace {

using openlr2::arena_gui::BpScore;
using openlr2::arena_gui::ChartMaxEx;
using openlr2::arena_gui::ExRatePercent;
using openlr2::arena_gui::ExScore;
using openlr2::arena_gui::GaugeLabel;
using openlr2::arena_gui::OptionLabel;

bool gRequestItemSettingsPopup = false;
bool gRequestBindKeyPopup = false;

bool IsSdLayout() {
	int dx = 0, dy = 0;
	GetDrawScreenSize(&dx, &dy);
	return dy > 0 && dy < 720;
}

// HelpMarker
void HelpMarker(const char* desc) {
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip()) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void RenderLobby(
	game* g,
	const openlr2::ArenaRoomState& state,
	bool& itemModeEnabled,
	char* roomIdInput,
	size_t roomIdCap,
	char* chatInput,
	size_t chatCap,
	bool (*joinRoom)(game*, const char*),
	void (*leave)(game*)
) {
	const bool joined = state.connected;

	ImGui::SeparatorText("Connection");
	{
		const bool lan = openlr2::arena::IsTransportLan();
		if (ImGui::RadioButton("IR (CustomIR)", !lan)) {
			openlr2::arena::SetTransportIr(g);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("LAN", lan)) {
			openlr2::arena::SetTransportLan(g);
		}
	}
	if (joined) {
		if (ImGui::Button("Leave##Arena")) {
			if (leave) leave(g);
		}
	} else if (openlr2::arena::IsTransportLan()) {
		ImGui::TextColored(ImVec4(1.0f, 0.47f, 0.47f, 1.0f), "Not connected (LAN)");
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::InputTextWithHint(
			"##LanUser",
			"Username",
			openlr2::arena::LanUserInputBuf(),
			openlr2::arena::LanUserInputCap(),
			ImGuiInputTextFlags_CharsNoBlank
		);
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::InputTextWithHint(
			"##LanHost",
			"Server IP / hostname",
			openlr2::arena::LanHostInputBuf(),
			openlr2::arena::LanHostInputCap(),
			ImGuiInputTextFlags_CharsNoBlank
		);
		const bool canConnect = openlr2::arena::LanHostInputBuf()[0] != '\0'
			&& openlr2::arena::LanUserInputBuf()[0] != '\0';
		ImGui::BeginDisabled(!canConnect);
		if (ImGui::Button("Connect##Lan")) {
			openlr2::arena::ConnectLan(
				g,
				openlr2::arena::LanHostInputBuf(),
				openlr2::arena::LanUserInputBuf()
			);
		}
		ImGui::EndDisabled();
		ImGui::SeparatorText("Server");
		if (openlr2::arena::IsLanServerRunning()) {
			ImGui::TextColored(ImVec4(0.47f, 1.0f, 0.47f, 1.0f), "Listening on :2222");
			if (ImGui::Button("Stop server")) {
				openlr2::arena::StopLanServer(g);
			}
		} else if (ImGui::Button("Start server")) {
			openlr2::arena::StartLanServer(g);
		}
		ImGui::TextDisabled("ws://host:2222");
	} else {
		ImGui::TextColored(ImVec4(1.0f, 0.47f, 0.47f, 1.0f), "Not connected (IR)");
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::InputTextWithHint(
			"##RoomId",
			"Room id",
			roomIdInput,
			roomIdCap,
			ImGuiInputTextFlags_CharsNoBlank
		);
		const bool canJoin = roomIdInput[0] != '\0';
		ImGui::BeginDisabled(!canJoin);
		if (ImGui::Button("Join room")) {
			if (joinRoom) joinRoom(g, roomIdInput);
		}
		ImGui::EndDisabled();
	}

	if (!joined) return;

	ImGui::SeparatorText("Lobby");
	const ImVec2 userDim = IsSdLayout() ? ImVec2(60.f, 0.f) : ImVec2(150.f, 0.f);
	const ImVec2 mainDim = IsSdLayout() ? ImVec2(230.f, 200.f) : ImVec2(300.f, 400.f);
	{
		ImGui::BeginChild(
			"Users",
			ImVec2(userDim.x, mainDim.y),
			ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX
		);
		const bool selfIsHost =
			state.selfPlayerId != 0 && state.selfPlayerId == state.hostId;
		for (const auto& p : state.players) {
			std::string label;
			if (p.isHost || p.playerId == state.hostId) {
				label += "* ";
			}
			if (p.ready && !state.chartHash.empty() && p.selectedHash == state.chartHash) {
				label += "[R] ";
			}
			label += p.username;
			ImGui::Selectable(label.c_str());
			if (selfIsHost && p.playerId != state.selfPlayerId && ImGui::BeginPopupContextItem()) {
				ImGui::TextDisabled("Selected user: %s", p.username.c_str());
				if (ImGui::MenuItem("Give host") && g != nullptr) {
					openlr2::arena_backend::Active().SetHost(g, p.playerId);
				}
				if (ImGui::MenuItem("Kick") && g != nullptr) {
					openlr2::arena_backend::Active().KickUser(g, p.playerId);
				}
				ImGui::EndPopup();
			}
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();
	{
		ImGui::BeginGroup();
		ImGui::BeginChild(
			"MainView",
			mainDim,
			ImGuiChildFlags_Borders,
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
		);

		if (!state.chartTitle.empty() || !state.chartHash.empty()) {
			ImGui::TextWrapped(
				"Title: %s",
				state.chartTitle.empty() ? "-" : state.chartTitle.c_str()
			);
			ImGui::TextDisabled(
				"Artist: %s",
				state.chartArtist.empty() ? "-" : state.chartArtist.c_str()
			);
			{
				const std::string roomPath =
					openlr2::arena::LookupChartRelativePath(g, state.chartHash);
				if (!roomPath.empty()) {
					ImGui::TextDisabled("Path:");
					ImGui::SameLine();
					ImGui::TextWrapped("%s", roomPath.c_str());
				}
			}
			if (!state.chartHash.empty()) {
				ImGui::SameLine();
				if (ImGui::SmallButton("IR##lr2ir")) {
					char url[128];
					std::snprintf(
						url,
						sizeof(url),
						"http://www.dream-pro.info/~lavalse/LR2IR/search.cgi?mode=ranking&bmsmd5=%s",
						state.chartHash.c_str()
					);
					ImGui::SetClipboardText(url);
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Copy LR2IR link");
				}
			}
		} else {
			ImGui::TextDisabled("Room chart: (host has not started a chart yet)");
		}

		ImGui::Separator();
		if (ImGui::BeginTabBar("##LobbyTabs")) {
			if (ImGui::BeginTabItem("Chat")) {
				const float footer =
					ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
				if (ImGui::BeginChild(
						"ChatScroll",
						ImVec2(0.0f, -footer),
						ImGuiChildFlags_None
					)) {
					for (const auto& line : state.chatLog) {
						ImGui::TextWrapped("%s", line.c_str());
					}
					if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 4.f) {
						ImGui::SetScrollHereY(1.0f);
					}
				}
				ImGui::EndChild();
				auto sendChat = [&]() {
					if (chatInput[0] == '\0' || g == nullptr) return;
					openlr2::arena_backend::Active().SendChat(g, chatInput);
					chatInput[0] = '\0';
				};
				ImGui::SetNextItemWidth(-1.0f);
				if (ImGui::InputText(
						"##Chat",
						chatInput,
						chatCap,
						ImGuiInputTextFlags_EnterReturnsTrue
					)) {
					sendChat();
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Settings##Lobby")) {
				const bool selfIsHost =
					state.selfPlayerId != 0 && state.selfPlayerId == state.hostId;
				if (!selfIsHost) ImGui::BeginDisabled();
				ImGui::Checkbox("Enable item / ojama mode", &itemModeEnabled);
				if (!selfIsHost) ImGui::EndDisabled();
				ImGui::SameLine();
				HelpMarker(
					"Host flag sent with chart select. Combo earns items; "
					"use-item key sends ojama to peers."
				);
				ImGui::TextDisabled("Item mode from room: %s", state.itemModeEnabled ? "on" : "off");
				{
					bool flip = openlr2::arena::IsRandomFlipEnabled();
					if (ImGui::Checkbox("Enable random flip", &flip)) {
						openlr2::arena::SetRandomFlipEnabled(flip);
						openlr2::arena_gui::SaveRandomFlip(flip);
					}
				}
				ImGui::BeginDisabled(!selfIsHost);
				if (ImGui::Button("Item settings...")) {
					gRequestItemSettingsPopup = true;
				}
				ImGui::EndDisabled();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndChild();
		ImGui::EndGroup();
	}
}

void RenderGraphWindow(game* g, bool& graphVisible, const openlr2::ArenaRoomState& state) {
	if (!graphVisible) return;
	ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (!ImGui::Begin("Graph", &graphVisible, flags)) {
		ImGui::End();
		return;
	}

	if (!state.connected || state.players.empty()) {
		ImGui::Text("Not connected to any server...");
		ImGui::End();
		return;
	}

	std::vector<openlr2::ArenaPeer> peers = state.players;
	enum class GraphType { Score, Bp, MaxCombo };
	static GraphType sortType = GraphType::Score;

	auto sortPeers = [&](GraphType t) {
		std::sort(peers.begin(), peers.end(), [&](const openlr2::ArenaPeer& a, const openlr2::ArenaPeer& b) {
			int va = 0, vb = 0;
			switch (t) {
			case GraphType::Score: va = ExScore(a.score); vb = ExScore(b.score); break;
			case GraphType::Bp: va = BpScore(a.score); vb = BpScore(b.score); break;
			case GraphType::MaxCombo: va = a.score.maxCombo; vb = b.score.maxCombo; break;
			}
			if (va != vb) return va > vb;
			return a.playerId < b.playerId;
		});
	};
	sortPeers(sortType);

	int maxScore = openlr2::arena::CachedChartMaxEx();
	if (maxScore <= 0) maxScore = (std::max)(0, ChartMaxEx(g, state));

	if (ImGui::BeginTabBar("##GraphTabs")) {
		auto drawBars = [&](GraphType type) {
			sortType = type;
			sortPeers(type);
			const int n = static_cast<int>(peers.size());
			if (n <= 0) return;
			std::vector<double> values(static_cast<std::size_t>(n * n), 0.0);
			std::vector<double> positions(static_cast<std::size_t>(n));
			std::vector<const char*> labels(static_cast<std::size_t>(n));
			for (int i = 0; i < n; ++i) {
				positions[static_cast<std::size_t>(i)] = i;
				labels[static_cast<std::size_t>(i)] = peers[static_cast<std::size_t>(i)].username.c_str();
				int v = 0;
				switch (type) {
				case GraphType::Score: v = ExScore(peers[static_cast<std::size_t>(i)].score); break;
				case GraphType::Bp: v = BpScore(peers[static_cast<std::size_t>(i)].score); break;
				case GraphType::MaxCombo: v = peers[static_cast<std::size_t>(i)].score.maxCombo; break;
				}
				values[static_cast<std::size_t>(i * n + i)] = static_cast<double>(v);
			}
			const char* yLabel = type == GraphType::Score ? "EX" : type == GraphType::Bp ? "BP" : "Combo";
			ImVec2 plotDim = IsSdLayout() ? ImVec2(100.f, 200.f) : ImVec2(150.f, 400.f);
			if (n > 2) {
				plotDim.x *= static_cast<float>(n) * 0.5f;
			}
			if (ImPlot::BeginPlot(
					"##bars",
					plotDim,
					ImPlotFlags_NoFrame | ImPlotFlags_NoInputs | ImPlotFlags_NoTitle | ImPlotFlags_NoLegend
				)) {
				ImPlot::SetupAxes(
					"Players",
					yLabel,
					ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoLabel,
					ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoLabel
				);
				ImPlot::SetupAxisTicks(ImAxis_X1, positions.data(), n, labels.data());
				if (type == GraphType::Score && maxScore > 0) {
					const double rankPos[] = {
						std::ceil(static_cast<double>(maxScore) * 0.666),
						std::ceil(static_cast<double>(maxScore) * 0.777),
						std::ceil(static_cast<double>(maxScore) * 0.888),
					};
					const char* rankLabels[] = {"A", "AA", "AAA"};
					ImPlot::SetupAxisTicks(ImAxis_Y1, rankPos, 3, rankLabels);
					ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, static_cast<double>(maxScore), ImPlotCond_Always);
				} else if (type == GraphType::MaxCombo && maxScore > 0) {
					ImPlot::SetupAxisLimits(
						ImAxis_Y1,
						0.0,
						static_cast<double>(maxScore) / 2.0,
						ImPlotCond_Always
					);
				}
				ImPlot::PlotBarGroups(
					labels.data(),
					values.data(),
					n,
					n,
					0.67,
					0,
					ImPlotBarGroupsFlags_Stacked
				);
				ImPlot::EndPlot();
			}
		};

		if (ImGui::BeginTabItem("Score")) {
			drawBars(GraphType::Score);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("BP")) {
			drawBars(GraphType::Bp);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("MAX COMBO")) {
			drawBars(GraphType::MaxCombo);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::SeparatorText("Details");
	if (ImGui::BeginTable("##ScoreDetails", 8, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)) {
		ImGui::TableSetupColumn("Player");
		ImGui::TableSetupColumn("EX");
		ImGui::TableSetupColumn("Rate");
		ImGui::TableSetupColumn("BP");
		ImGui::TableSetupColumn("PG");
		ImGui::TableSetupColumn("GR");
		ImGui::TableSetupColumn("GD");
		ImGui::TableSetupColumn("BD/PR");
		ImGui::TableHeadersRow();
		for (const auto& p : peers) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::TextUnformatted(p.username.c_str());
			ImGui::TableNextColumn();
			ImGui::Text("%d", ExScore(p.score));
			ImGui::TableNextColumn();
			ImGui::Text("%.2f%%", ExRatePercent(p.score));
			ImGui::TableNextColumn();
			ImGui::Text("%d", BpScore(p.score));
			ImGui::TableNextColumn();
			ImGui::TextColored(ImVec4(1.f, 0.85f, 0.2f, 1.f), "%d", p.score.pgreat);
			ImGui::TableNextColumn();
			ImGui::TextColored(ImVec4(0.4f, 1.f, 0.4f, 1.f), "%d", p.score.great);
			ImGui::TableNextColumn();
			ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.f, 1.f), "%d", p.score.good);
			ImGui::TableNextColumn();
			ImGui::Text("%d/%d", p.score.bad, p.score.poor);
		}
		ImGui::EndTable();
	}

	ImGui::Separator();
	{
		unsigned opt = 0;
		unsigned gauge = 0;
		for (const auto& p : peers) {
			if (p.playerId == state.selfPlayerId || p.isHost) {
				opt = p.option;
				gauge = p.gauge;
				if (p.playerId == state.selfPlayerId) break;
			}
		}
		ImGui::TextDisabled("Option %s  Gauge %s", OptionLabel(opt), GaugeLabel(gauge));
	}
	ImGui::End();
}

} // namespace

void RenderArenaGui(
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
	if (menuVisible) {
		ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (ImGui::Begin("OpenLR2 Arena", &menuVisible, flags)) {
			if (ImGui::BeginTabBar("ArenaTabs")) {
				if (ImGui::BeginTabItem("Lobby")) {
					RenderLobby(
						g,
						state,
						itemModeEnabled,
						roomIdInput,
						roomIdCap,
						chatInput,
						chatCap,
						joinRoom,
						leave
					);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Settings")) {
					ImGui::SeparatorText("Settings");
					if (ImGui::Checkbox("Disable game inputs when overlay is shown", &muteGameInputs)) {
						openlr2::arena_gui::SaveMuteGameInputs(muteGameInputs);
					}
					ImGui::SameLine();
					HelpMarker("Does not affect the graph display in-game");

					if (ImGui::Checkbox(
							"Hide/show graph window automatically on scene transitions",
							&automaticGraph
						)) {
						openlr2::arena_gui::SaveAutomaticGraph(automaticGraph);
					}

					{
						static int volTmp = openlr2::arena_maniac::SfxVolume();
						if (!ImGui::IsAnyItemActive()) {
							volTmp = openlr2::arena_maniac::SfxVolume();
						}
						ImGui::SliderInt(
							"Item sounds volume",
							&volTmp,
							0,
							100,
							"%d%%",
							ImGuiSliderFlags_AlwaysClamp
						);
						if (ImGui::IsItemDeactivated() && volTmp != openlr2::arena_maniac::SfxVolume()) {
							openlr2::arena_maniac::SetSfxVolume(volTmp);
							openlr2::arena_gui::SaveItemSfxVolume(volTmp);
						}
					}

					if (ImGui::CollapsingHeader("Item SFX settings")) {
						const auto slots = openlr2::arena_maniac::ListSfx();
						if (ImGui::BeginTable(
								"ItemSfxTable",
								3,
								ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg
									| ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV
									| ImGuiTableFlags_BordersH,
								ImVec2(ImGui::GetFontSize() * 25.0f, ImGui::GetTextLineHeightWithSpacing() * 7)
							)) {
							ImGui::TableSetupScrollFreeze(1, 0);
							ImGui::TableSetupColumn("");
							ImGui::TableSetupColumn("Name");
							ImGui::TableSetupColumn("File");
							ImGui::TableHeadersRow();
							for (const auto& s : slots) {
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::PushID(s.id.c_str());
								if (ImGui::Button("...")) {
									char path[MAX_PATH]{};
									path[0] = '\0';
									OPENFILENAMEA ofn{};
									ofn.lStructSize = sizeof(ofn);
									ofn.hwndOwner = GetMainWindowHandle();
									ofn.lpstrFile = path;
									ofn.nMaxFile = MAX_PATH;
									ofn.lpstrFilter = "Audio files\0*.wav;*.ogg;*.mp3\0All\0*.*\0";
									ofn.nFilterIndex = 1;
									ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
									if (GetOpenFileNameA(&ofn) == TRUE) {
										openlr2::arena_maniac::SetSfxCustomPath(s.id.c_str(), path);
										openlr2::arena_gui::SaveItemSfxPath(s.id.c_str(), path);
										// toast on custom SFX load.
										ImGui::InsertNotification(ImGuiToast(
											ImGuiToastType::Success,
											3000,
											"Succesfully loaded file: %s",
											path
										));
									}
								}
								ImGui::SameLine();
								if (ImGui::Button(">")) {
									openlr2::arena_maniac::PlaySfx(s.id.c_str());
								}
								ImGui::PopID();
								ImGui::TableNextColumn();
								ImGui::TextUnformatted(s.name.c_str());
								ImGui::TableNextColumn();
								if (s.customPath.empty()) {
									ImGui::TextDisabled("(Default)");
								} else {
									ImGui::TextUnformatted(s.customPath.c_str());
								}
							}
							ImGui::EndTable();
						}
						if (ImGui::Button("Reset all SFXs")) {
							openlr2::arena_maniac::ResetAllCustomSfx();
							openlr2::arena_gui::ClearItemSfxPaths();
							ImGui::InsertNotification(ImGuiToast(
								ImGuiToastType::Info,
								3000,
								"All sound effects reset to default sounds!"
							));
						}
					}

					static openlr2::arena_keys::Binding waiting{openlr2::arena_keys::Binding::Count};
					static openlr2::arena_keys::Key alreadyBound{};
					if (ImGui::CollapsingHeader("Bindings settings")) {
						const float bindH = ImGui::GetTextLineHeightWithSpacing() * 5
							+ (IsSdLayout() ? 3.f : -1.f);
						if (ImGui::BeginTable(
								"BindingsTable",
								3,
								ImGuiTableFlags_ScrollX | ImGuiTableFlags_RowBg
									| ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV
									| ImGuiTableFlags_BordersH,
								ImVec2(ImGui::GetFontSize() * 25.0f, bindH)
							)) {
							ImGui::TableSetupColumn("Name");
							ImGui::TableSetupColumn("Key");
							ImGui::TableSetupColumn("");
							ImGui::TableHeadersRow();
							for (std::size_t i = 0; i < openlr2::arena_keys::BindingCount; ++i) {
								const auto b = static_cast<openlr2::arena_keys::Binding>(i);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::TextUnformatted(openlr2::arena_keys::Label(b));
								ImGui::TableNextColumn();
								ImGui::TextUnformatted(
									openlr2::arena_keys::ToString(openlr2::arena_keys::Get(b)).c_str()
								);
								ImGui::TableNextColumn();
								ImGui::PushID(static_cast<int>(i));
								if (ImGui::Button("Bind")) {
									waiting = b;
									alreadyBound = {};
									gRequestBindKeyPopup = true;
									openlr2::arena_imgui::SetWaitingForKeyBind(true);
								}
								ImGui::PopID();
							}
							ImGui::EndTable();
						}
					}

					if (gRequestBindKeyPopup) {
						ImGui::OpenPopup("Bind key");
						gRequestBindKeyPopup = false;
					}
					{
						int dx = 0, dy = 0;
						GetDrawScreenSize(&dx, &dy);
						if (dx > 0 && dy > 0) {
							ImGui::SetNextWindowPos(
								ImVec2(static_cast<float>(dx) * 0.5f, static_cast<float>(dy) * 0.5f),
								ImGuiCond_Appearing,
								ImVec2(0.5f, 0.5f)
							);
						}
					}
					if (ImGui::BeginPopupModal("Bind key", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
						ImGui::Text(
							"Binding key for: %s\nPress any button...",
							openlr2::arena_keys::Label(waiting)
						);
						if (alreadyBound.type != openlr2::arena_keys::DeviceType::None) {
							ImGui::TextColored(
								ImVec4(1.f, 1.f, 0.f, 1.f),
								"%s is already bound, please select another key",
								openlr2::arena_keys::ToString(alreadyBound).c_str()
							);
						}
						ImGui::Separator();
						bool done = false;
						if (waiting != openlr2::arena_keys::Binding::Count) {
							const auto pressed = openlr2::arena_keys::PollNewPress(g);
							if (pressed.type != openlr2::arena_keys::DeviceType::None) {
								if (!openlr2::arena_keys::Bind(waiting, pressed)) {
									alreadyBound = pressed;
								} else {
									openlr2::arena_keys::Save();
									done = true;
								}
							}
						}
						if (done || ImGui::Button("Cancel")) {
							waiting = openlr2::arena_keys::Binding::Count;
							alreadyBound = {};
							openlr2::arena_imgui::SetWaitingForKeyBind(false);
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					} else {
						openlr2::arena_imgui::SetWaitingForKeyBind(false);
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			{
				static auto itemsTmp = openlr2::arena_maniac::Items();
				static int thresholdTmp = static_cast<int>(openlr2::arena_maniac::ThresholdMult() * 100.f);
				static auto itemsDefault = openlr2::arena_maniac::Items();
				static int thresholdDefault = 10;
				if (openlr2::arena_maniac::SettingsRemoteUpdated()) {
					itemsTmp = openlr2::arena_maniac::Items();
					thresholdTmp = static_cast<int>(openlr2::arena_maniac::ThresholdMult() * 100.f);
					openlr2::arena_maniac::SettingsRemoteUpdated() = false;
				}
				if (gRequestItemSettingsPopup) {
					ImGui::OpenPopup("Item settings");
					gRequestItemSettingsPopup = false;
				}
				{
					int dx = 0, dy = 0;
					GetDrawScreenSize(&dx, &dy);
					if (dx > 0 && dy > 0) {
						ImGui::SetNextWindowPos(
							ImVec2(static_cast<float>(dx) * 0.5f, static_cast<float>(dy) * 0.5f),
							ImGuiCond_Appearing,
							ImVec2(0.5f, 0.5f)
						);
					}
				}
				if (ImGui::BeginPopupModal("Item settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
					if (itemsTmp.empty()) itemsTmp = openlr2::arena_maniac::Items();
					ImGui::SeparatorText("Items");
					if (ImGui::BeginTable("itemTable", 5, ImGuiTableFlags_BordersInnerV)) {
						ImGui::TableSetupColumn("Item");
						ImGui::TableSetupColumn("Lv1", ImGuiTableColumnFlags_WidthStretch);
						ImGui::TableSetupColumn("Lv2", ImGuiTableColumnFlags_WidthStretch);
						ImGui::TableSetupColumn("Lv3", ImGuiTableColumnFlags_WidthStretch);
						ImGui::TableSetupColumn("Weight", ImGuiTableColumnFlags_WidthStretch);
						ImGui::TableHeadersRow();
						for (auto& item : itemsTmp) {
							ImGui::TableNextRow();
							ImGui::PushID(item.name.c_str());
							ImGui::TableNextColumn();
							ImGui::Text("%s %s", item.icon.c_str(), item.name.c_str());
							ImGui::TableNextColumn();
							if (item.name == "Acceleration") {
								const char* accel[] = {"None", "Accel", "Decel", "Random"};
								int a1 = static_cast<int>(item.lv1);
								int a2 = static_cast<int>(item.lv2);
								int a3 = static_cast<int>(item.lv3);
								ImGui::PushItemWidth(-1);
								ImGui::Combo("##a1", &a1, accel, 4);
								ImGui::PopItemWidth();
								item.lv1 = static_cast<unsigned>(a1);
								ImGui::TableNextColumn();
								ImGui::PushItemWidth(-1);
								ImGui::Combo("##a2", &a2, accel, 4);
								ImGui::PopItemWidth();
								item.lv2 = static_cast<unsigned>(a2);
								ImGui::TableNextColumn();
								ImGui::PushItemWidth(-1);
								ImGui::Combo("##a3", &a3, accel, 4);
								ImGui::PopItemWidth();
								item.lv3 = static_cast<unsigned>(a3);
							} else {
								int l1 = static_cast<int>(item.lv1);
								int l2 = static_cast<int>(item.lv2);
								int l3 = static_cast<int>(item.lv3);
								ImGui::PushItemWidth(-1);
								ImGui::DragInt("##l1", &l1, 1.f, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);
								ImGui::PopItemWidth();
								item.lv1 = static_cast<unsigned>(l1);
								ImGui::TableNextColumn();
								ImGui::PushItemWidth(-1);
								ImGui::DragInt("##l2", &l2, 1.f, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);
								ImGui::PopItemWidth();
								item.lv2 = static_cast<unsigned>(l2);
								ImGui::TableNextColumn();
								ImGui::PushItemWidth(-1);
								ImGui::DragInt("##l3", &l3, 1.f, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);
								ImGui::PopItemWidth();
								item.lv3 = static_cast<unsigned>(l3);
							}
							ImGui::TableNextColumn();
							int w = static_cast<int>(item.weight);
							ImGui::PushItemWidth(-1);
							ImGui::DragInt("##w", &w, 1.f, 0, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
							ImGui::PopItemWidth();
							item.weight = static_cast<unsigned>(w);
							ImGui::PopID();
						}
						ImGui::EndTable();
					}
					ImGui::SeparatorText("Threshold");
					ImGui::SliderInt("Item threshold", &thresholdTmp, 1, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);
					ImGui::SameLine();
					HelpMarker(
						"Indicates after how much combo a player gets an item; given as a percentage "
						"of the total number of notes in the selected chart"
					);
					ImGui::Separator();
					if (ImGui::Button("Save")) {
						if (std::all_of(itemsTmp.begin(), itemsTmp.end(), [](const auto& i) {
								return i.weight == 0;
							})) {
							ImGui::InsertNotification(
								{ImGuiToastType::Error, 3000, "Item weights must not be all zeroes!"}
							);
						} else {
							openlr2::arena_maniac::Items() = itemsTmp;
							openlr2::arena_maniac::ThresholdMult() = static_cast<float>(thresholdTmp) / 100.f;
							openlr2::arena_maniac::UpdateItemWeights();
							openlr2::arena_maniac::SendItemSettings(g);
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::SameLine();
					if (ImGui::Button("Cancel")) {
						itemsTmp = openlr2::arena_maniac::Items();
						thresholdTmp = static_cast<int>(openlr2::arena_maniac::ThresholdMult() * 100.f);
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
					const char* resetText = "Reset settings";
					const float resetWidth =
						ImGui::CalcTextSize(resetText).x + ImGui::GetStyle().FramePadding.x * 2.f;
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - resetWidth);
					if (ImGui::Button(resetText)) {
						itemsTmp = itemsDefault;
						thresholdTmp = thresholdDefault;
					}
					ImGui::EndPopup();
				}
			}
		}
		ImGui::End();
	}

	if (graphVisible) {
		RenderGraphWindow(g, graphVisible, state);
	}

	if (graphVisible && state.connected && openlr2::arena_maniac::IsItemModeEnabled()) {
		const ImVec2 btn = IsSdLayout() ? ImVec2(50.f, 50.f) : ImVec2(75.f, 75.f);
		ImGui::SetNextWindowSize(ImVec2(0.f, 0.f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(
				"Item",
				nullptr,
				ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize
					| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing
					| ImGuiWindowFlags_NoBringToFrontOnFocus
			)) {
			auto& held = openlr2::arena_maniac::HeldItem();
			const auto& items = openlr2::arena_maniac::Items();
			const int heldId = held.rolledItemId;
			const unsigned heldLevel = held.level;
			ImGui::BeginChild(
				"##ItemsMain",
				ImVec2(0, 0),
				ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY
			);
			if (heldId < 0 || heldId >= static_cast<int>(items.size())) {
				ImGui::BeginDisabled();
				ImGui::Button("Empty", btn);
				ImGui::EndDisabled();
			} else {
				const float hue = (heldLevel + 2) / 7.f;
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
				const char* icon = items[static_cast<std::size_t>(heldId)].icon.c_str();
				ImFont* big = openlr2::arena_imgui::BigIconFont();
				bool clicked = false;
				if (big != nullptr) {
					ImGui::PushFont(big);
					clicked = ImGui::Button(icon, btn);
					ImGui::PopFont();
				} else {
					clicked = ImGui::Button(icon, btn);
				}
				if (clicked) {
					(void)openlr2::arena_maniac::UseItem(g);
				}
				ImGui::PopStyleColor(3);
			}
			{
				const unsigned combo = openlr2::arena_maniac::CurrentCombo();
				const unsigned thr = openlr2::arena_maniac::Threshold();
				float progress = thr > 0 ? static_cast<float>(combo) / static_cast<float>(thr) : 0.f;
				int histI = 5;
				if (held.level < 3) {
					progress -= static_cast<float>(held.level);
					histI = static_cast<int>(held.level) + 3;
				}
				ImGui::PushStyleColor(
					ImGuiCol_PlotHistogram,
					(ImVec4)ImColor::HSV(histI / 7.0f, 0.6f, 0.6f)
				);
				ImGui::ProgressBar(progress, ImVec2(btn.x, 5.f), "");
				ImGui::PopStyleColor();
			}
			ImGui::EndChild();
			const auto& active = openlr2::arena_maniac::ActiveItems();
			if (!active.empty()) {
				ImGui::SameLine();
				ImGui::BeginChild(
					"##ItemsDetails",
					ImVec2(0, 0),
					ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY
				);
				ImGui::SeparatorText("Current effects");
				for (const auto& [key, remMs] : active) {
					if (key.rolledItemId < 0 || key.rolledItemId >= static_cast<int>(items.size())) {
						continue;
					}
					std::string icon = items[static_cast<std::size_t>(key.rolledItemId)].icon;
					for (unsigned i = 1; i < key.level; ++i) {
						icon += items[static_cast<std::size_t>(key.rolledItemId)].icon;
					}
					ImGui::Text("%s (%02d:%02d)", icon.c_str(), remMs / 60000, (remMs / 1000) + 1);
				}
				ImGui::EndChild();
			}
		}
		ImGui::End();
	}
}

} // namespace openlr2::arena_imgui
