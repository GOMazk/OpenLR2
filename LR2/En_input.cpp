#include "En_input.h"
#include "En_timer.h"
#include "En_graphic.h"
#include <DxLib.h>

#ifndef _WIN32
#include "En_dxlibstub.h"
#endif // _WIN32

#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
struct HMIDIIN {};
#ifndef CALLBACK
#define CALLBACK
#endif // CALLBACK
#endif // _WIN32

typedef struct MIDI {
	byte input[260]; //0x101:ptich_minus 0x102:pitch_plus 0x103:pedal;
	int unusedFC;
	int controller_n;
	int controller_v;
	HMIDIIN phmiArray[16];
}MIDI;

static MIDI midi;

#ifndef _WIN32
#define LOWORD(l) ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l) ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#endif

constexpr unsigned int AXIS_RANGE_BEGIN = 0x100 + 32 * 4;
constexpr unsigned int AXIS_RANGE_END = AXIS_RANGE_BEGIN + 12 * 4;

#ifdef _WIN32
static void FixMousePointWithClientPosition(int* mouseX, int* mouseY) {
	if (GetWindowModeFlag() == TRUE && GetUseFullScreenResolutionMode() != DX_FSRESOLUTIONMODE_BORDERLESS_WINDOW) return;

	const HWND hWnd = GetMainWindowHandle();
	if (hWnd == nullptr) return;

	RECT clientRect{};
	if (GetClientRect(hWnd, &clientRect) == 0) return;
	const int clientWidth = clientRect.right - clientRect.left;
	const int clientHeight = clientRect.bottom - clientRect.top;

	POINT cursorPoint{};
	if (GetCursorPos(&cursorPoint) == 0) return;
	if (ScreenToClient(hWnd, &cursorPoint) == 0) return;

	auto scaleClientPointToSkin = [](int point, int clientSize, int skinSize) {
		if (clientSize <= 0 || skinSize <= 0) return point;
		if (point >= 0) return point * skinSize / clientSize;
		return -((-point * skinSize + clientSize - 1) / clientSize);
	};

	*mouseX = scaleClientPointToSkin(cursorPoint.x, clientWidth, skinSizeX);
	*mouseY = scaleClientPointToSkin(cursorPoint.y, clientHeight, skinSizeY);
}
#endif // _WIN32

// TODO structure array rework
int InitInputStructure2(inputStructure *is){ // Called on scene end
	is->is_doubleclick = 0;
	is->mousewheel = 0;
	is->mouse_buttonL = 0;
	is->mouse_buttonR = 0;
	is->mouse_buttonW = 0;
	is->mouse_button4 = 0;
	std::ranges::fill(is->p1_buttonInput, 0);
	std::ranges::fill(is->p2_buttonInput, 0);
	std::ranges::fill(is->otherbuttons, 0);
	std::ranges::fill_n(is->inputID, 0x600, 0);
	is->inputAxis = {};
	memset(is->inputAxisState, 0, sizeof(is->inputAxisState));
	memset(is->inputAxisLoopingDecay, 0, sizeof(is->inputAxisLoopingDecay));
	is->inputAxisFirstPoll = true;
	SetJoypadDefaultDeadZoneAll(0.); // should be 0 to detect full range of looping axis. Can't set per-axis.
	return 1;
}

static void EndMIDIInput(void){
#ifdef _WIN32
	UINT numDev;

	numDev = midiInGetNumDevs();
	if (numDev > 15) {
		numDev = 15;
	}

	for (int i = 0; i < numDev; i++) {
		midiInStop(midi.phmiArray[i]);
		midiInClose(midi.phmiArray[i]);
	}
#endif // _WIN32
}

static void GetMidiInput(dword msg, dword /*timestamp*/) {
	// http://www.gweep.net/~prefect/eng/reference/protocol/midispec.html
	byte status = (msg & 0xff);
	byte data1 = LOWORD(msg) >> 8;
	byte data2 = HIWORD(msg) & 0xff;

	if (data1 != 0xf8 && (data1 & 0xf0) != 0xf0) {
		switch (status >> 4) {
			case 8: //note off
				midi.input[data1] = 0;
				return;
			case 9: //note on
				midi.input[data1] = (data2 != 0);
				return;
			case 11: //control change
				midi.controller_n = data1;
				midi.controller_v = data2;
				if (data1 == 0x40) { //pedal
					if (data2 < 0x40) {
						midi.input[0x102] = 0; //pedal
					}
					else if (data2 > 0x40) {
						midi.input[0x102] = 1;
					}
				}
				return;
			case 14: //ptich wheel
				if (data2 < 0x40) {
					midi.input[0x100] = 1; //pitch_minus
					midi.input[0x101] = 0; //pitch_plus
				}
				else if (data2 > 0x40) {
					midi.input[0x100] = 0;
					midi.input[0x101] = 1;
				}
				else {
					midi.input[0x100] = 0;
					midi.input[0x101] = 0;
				}
				return;
		}
	}
}

int FindPressedKey(inputStructure *is){

	for (int i = 1; i < 0x600; i++) {
		if (is->inputID[i] == 1) {
			return i;
		}
	}
	return 0;
}

// Uses virtual button index, as if it was present in inputID array. Isn't actually there to not interfere in existing button logic, also it's simply more convenient this way.
static unsigned char GetAxisState(const inputStructure& is, int idx, AXISTYPE type) {
	size_t axis_i = idx - AXIS_RANGE_BEGIN;
	size_t padIdx = axis_i / 12;
	size_t axisIdx = axis_i % 12;
	size_t dirIdx = axisIdx % 2;
	axisIdx = axisIdx / 2;
	return is.inputAxisState[padIdx][axisIdx][dirIdx][type];
}

// Separate from FindPressedKey so it's not used in WaitInput calls.
int FindPressedAxis(const inputStructure& is, AXISTYPE type) {
	for (size_t i = AXIS_RANGE_BEGIN; i < AXIS_RANGE_END; i++) {
		if (GetAxisState(is, i, type) == 1) return i;
	}
	return 0;
}

int ResetPressCount(inputStructure *is){
	is->keyboard_presscount = 0;
	is->joypad_presscount = 0;
	is->MIDI_presscount = 0;
	return 1;
}

int DetermineResultPlayDevice(inputStructure *is){
	int &joy = is->joypad_presscount;
	int &key = is->keyboard_presscount;
	int &midi = is->MIDI_presscount;
	
	if (joy <= key && midi <= key) return 0;
	if (midi <= joy && key <= joy) return 1;
	if (key <= midi && joy <= midi) return 2;
	return 0;
}

int CloseMIDI(void){
	EndMIDIInput();
	return 1;
}

static void ProcessInput(inputStructure* is, int interval) {

	int mouseX, mouseY;
	uint new_joyInput[256];
	char new_keyInput[256];
	int keyError;

	if (GetWindowActiveFlag() == 0) return;

	GetMousePoint(&mouseX, &mouseY);
#ifdef _WIN32
	FixMousePointWithClientPosition(&mouseX, &mouseY);
#endif // _WIN32
	is->mouse_moveX = mouseX - is->mouse_oldX;
	is->mouse_moveY = mouseY - is->mouse_oldY;
	is->mouse_moveflag = 0;
	if (is->mouse_oldX != mouseX) {
		is->mouse_oldX = mouseX;
		is->mouse_moveflag = 1;
		is->mouse_recentMoveTime = GetTimeWrap();
	}
	if (is->mouse_oldY != mouseY) {
		is->mouse_oldY = mouseY;
		is->mouse_moveflag = 1;
		is->mouse_recentMoveTime = GetTimeWrap();
	}
	is->mousewheel = GetMouseWheelRotVol();
	if ((GetMouseInput() & 1) == 0) {
		is->is_doubleclick = 0;
		if (is->mouse_buttonL == 0 || is->mouse_buttonL == 3) {
			is->mouse_buttonL = 0;
		}
		else if (GetTimeWrap() - (double)is->drag_start_time > 32.0) {
			is->mouse_buttonL = 3;
		}
		else {
			is->mouse_buttonL = 2;
		}
	}
	else {
		if (is->mouse_buttonL != 1 && is->mouse_buttonL != 2) {
			is->mouse_buttonL = 1;
			if (GetTimeWrap() - (double)is->drag_start_time >= 300.0) {
				is->drag_start_time = GetTimeWrap();
			}
			else {
				is->is_doubleclick = 1;
			}
		}
		else {
			is->is_doubleclick = 0;
			is->mouse_buttonL = 2;
		}
	}

	if ((GetMouseInput() & 2) == 0) {
		if (is->mouse_buttonR == 0 || is->mouse_buttonR == 3) {
			is->mouse_buttonR = 0;
		}
		else {
			is->mouse_buttonR = 3;
		}
	}
	else if (is->mouse_buttonR == 1 || is->mouse_buttonR == 2) {
		is->mouse_buttonR = 2;
	}
	else {
		is->mouse_buttonR = 1;
	}

	if ((GetMouseInput() & 4) == 0) {
		if ((is->mouse_buttonW == 0) || (is->mouse_buttonW == 3)) {
			is->mouse_buttonW = 0;
		}
		else {
			is->mouse_buttonW = 3;
		}
	}
	else if (is->mouse_buttonW == 1 || is->mouse_buttonW == 2) {
		is->mouse_buttonW = 2;
	}
	else {
		is->mouse_buttonW = 1;
	}
	if ((is->mouse_buttonL == 0) && (is->is_drag_now != -1)) {
		is->is_drag_now = -1;
	}

	//key
	GetTimeWrap();
	keyError = GetHitKeyStateAll(new_keyInput);
	GetTimeWrap();

	GetTimeWrap();
	//joypad
	std::ranges::fill_n(new_joyInput, 0x100, 0);
	for (int padIdx = 0; padIdx < 4; padIdx++) {
		DINPUT_JOYSTATE state{};
		if (GetJoypadDirectInputState(padIdx + 1, &state)) continue;
		new_joyInput[0x20 * padIdx + 0] = state.POV[0] >= 13500 && state.POV[0] <= 22500 ? 1 : 0; // down
		new_joyInput[0x20 * padIdx + 1] = state.POV[0] >= 22500 && state.POV[0] <= 31500 ? 1 : 0; // left
		new_joyInput[0x20 * padIdx + 2] = state.POV[0] >= 4500 && state.POV[0] <= 13500 ? 1 : 0; // right
		new_joyInput[0x20 * padIdx + 3] = (state.POV[0] >= 0 && state.POV[0] <= 4500) || state.POV[0] == 31500 ? 1 : 0; // up
		for (int j = 4; j < 32; j++) {
			new_joyInput[0x20 * padIdx + j] = state.Buttons[j - 4] & 0x80; // wowza we support 4 more buttons because GetJoypadInputState only deals 24, but still not all 32 because LR2 said so.
		}
		auto& axis = is->inputAxis[padIdx];
		auto axisPrev = is->inputAxis[padIdx];
		axis.X = state.X;
		axis.Y = state.Y;
		axis.Z = state.Z;
		axis.Rx = state.Rx;
		axis.Ry = state.Ry;
		axis.Rz = state.Rz;
		for (int axisIdx = 0; axisIdx < 6; axisIdx++) {
			auto get_val = [&axis, &axisIdx](const inputStructure::axis& axis, int axisIdx) {
				switch (axisIdx) {
				case 0: return axis.X;
				case 1: return axis.Y;
				case 2: return axis.Z;
				case 3: return axis.Rx;
				case 4: return axis.Ry;
				case 5: return axis.Rz;
				}
				return 0;
			};
			int val = get_val(axis, axisIdx);
			int valPrev = is->inputAxisFirstPoll ? val : get_val(axisPrev, axisIdx); // avoids first poll since reset making an input that wasn't actually made.
			for (int dirIdx = 0; dirIdx < 2; dirIdx++) {
				bool anyTypePressed = false;
				auto advance_state = [](unsigned char& state, bool isPressed) {
					if (isPressed) state = state == 0 || state == 3 ? 1 : 2;
					else state = state == 0 || state == 3 ? 0 : 3;
				};
				// required since dinput doesn't handle deadzone anymore, as looping axis requires that to be set to 0.
				auto apply_deadzone = [](double val, double zone, double maxval) {
					if (val <= zone) return 0;
					double scaled = ((val - zone) / (maxval - zone)) * maxval;
					return static_cast<int>(scaled);
				};
				{
					int valtmp = dirIdx ? val : -val;
					valtmp = apply_deadzone(valtmp, 250, 1000);
					auto& state = is->inputAxisState[padIdx][axisIdx][dirIdx][AXISTYPE_STICK];
					advance_state(state, valtmp > 0);
					if (state == 1) anyTypePressed = true;
				}
				{
					int valtmp = dirIdx ? val + 1000 : -val + 1000;
					valtmp = apply_deadzone(valtmp, 250, 2000);
					auto& state = is->inputAxisState[padIdx][axisIdx][dirIdx][AXISTYPE_TRIGGER];
					advance_state(state, valtmp > 0);
					if (state == 1) anyTypePressed = true;
				}
				{
					int delta = val - valPrev;
					if (delta > 1000 || delta < -1000) {
						delta = val + valPrev;
						valPrev > val ? delta += 1 : delta -= 1;
					}
					auto& deltaAccum = is->inputAxisDelta[padIdx][axisIdx];
					deltaAccum += delta;
					if (!dirIdx) delta = -delta;
					auto& state = is->inputAxisState[padIdx][axisIdx][dirIdx][AXISTYPE_LOOPING];
					auto& stateOpposite = is->inputAxisState[padIdx][axisIdx][dirIdx ? 0 : 1][AXISTYPE_LOOPING];
					auto& decay = is->inputAxisLoopingDecay[padIdx][axisIdx][dirIdx];
					if (delta > 0) {
						decay = GetTimeWrap(); // decay is needed because at high polling rate the encoder is unlikely to update every poll to lack of resolution, unless spun ridiculously fast. This way the continuous motion can be detected.
						advance_state(state, true);
						advance_state(stateOpposite, false);
					}
					else {
						if (state == 1) state = 2;
						if (GetTimeWrap() - decay > 200.) advance_state(state, false);
					}
					if (state == 1) anyTypePressed = true;
				}
				if (anyTypePressed) is->joypad_presscount++;
			}
		}
	}
	is->inputAxisFirstPoll = false;
	//presscount
	if (keyError != -1) {
		for (int i = 0; i < 0x100; i++) {
			if (new_keyInput[i] == 0) {
				if (is->inputID[i] == 1 || is->inputID[i] == 2) {
					if(GetTimeWrap() - is->inputTime[i] <= interval) is->inputID[i] = 2;
					else is->inputID[i] = 3;
				}
				else {
					is->inputID[i] = 0;
				}
			}
			else if(is->inputID[i]==1 || is->inputID[i]==2 || is->inputID[i]==3){
				is->inputID[i] = 2;
			}
			else {
				is->inputID[i] = 1;
				is->inputTime[i] = (int)GetTimeWrap();
				is->keyboard_presscount++;
			}
		}
	}
	for (int i = 0x100; i < 0x200; i++) {
		if (new_joyInput[i-0x100] == 0) {
			if (is->inputID[i] == 1 || is->inputID[i] == 2) {
				if (GetTimeWrap() - is->inputTime[i] <= interval) is->inputID[i] = 2;
				else is->inputID[i] = 3;
			}
			else {
				is->inputID[i] = 0;
			}
		}
		else if (is->inputID[i] == 1 || is->inputID[i] == 2 || is->inputID[i] == 3) {
			is->inputID[i] = 2;
		}
		else {
			is->inputID[i] = 1;
			is->inputTime[i] = (int)GetTimeWrap();
			is->joypad_presscount++;
		}
	}
	for (int i = 0x200; i < 0x303; i++) {
		if (midi.input[i - 0x200] == 0) {
			if (is->inputID[i] == 1 || is->inputID[i] == 2) {
				if (GetTimeWrap() - is->inputTime[i] <= interval) is->inputID[i] = 2;
				else is->inputID[i] = 3;
			}
			else {
				is->inputID[i] = 0;
			}
		}
		else if (is->inputID[i] == 1 || is->inputID[i] == 2 || is->inputID[i] == 3) {
			is->inputID[i] = 2;
		}
		else {
			is->inputID[i] = 1;
			is->inputTime[i] = (int)GetTimeWrap();
			is->MIDI_presscount++;
		}
	}
}

static void CALLBACK MIDIInProc(HMIDIIN /*hMidiIn*/, uint wMsg, dword /*dwInstance*/, dword dwParam1, dword dwParam2){
	if (wMsg == 0x3c3) { // = 963
		GetMidiInput(dwParam1, dwParam2);
	}
}

int WaitInput(inputStructure *is){
	is->is_doubleclick = 0;
	is->mousewheel = 0;
	is->mouse_buttonL = 0;
	std::ranges::fill(is->p1_buttonInput, 0);
	std::ranges::fill(is->p2_buttonInput, 0);
	std::ranges::fill(is->otherbuttons, 0);
	std::ranges::fill_n(is->inputID, 0x600, 0);
	while (FindPressedKey(is) == 0 && is->mouse_buttonL != 1 && is->mouse_buttonR != 1) {
		WaitTimer(40);
		ProcessInput(is, 0);
		if (ProcessMessage() != 0) {
			return -1;
		}
	}
	return 1;
}

int InputToButton(inputStructure *is, CONFIG_INPUT *cfg_input, int player, int isReplay) {

	ProcessInput(is, cfg_input->sys_inputinterval);
	if (midi.controller_n > 0) {
		is->midi_n = midi.controller_n;
		is->midi_v = midi.controller_v;
		midi.controller_n = 0;
		midi.controller_v = 0;
	}

	if (isReplay == 0) {
		auto get_bound_state = [](const inputStructure& is, int boundIdx, AXISTYPE axisType) -> unsigned char {
			if (boundIdx == 0) return 0;
			if (boundIdx >= AXIS_RANGE_BEGIN && boundIdx < AXIS_RANGE_END)
				return GetAxisState(is, boundIdx, axisType);
			return is.inputID[boundIdx];
		};
		auto loopingDeltaStep = [](inputStructure& input) {
			std::array<std::array<int, 6>, 4> converted{};
			for (int padIdx = 0; padIdx < 4; padIdx++) {
				for (int axisIdx = 0; axisIdx < 6; axisIdx++) {
					auto& delta = input.inputAxisDelta[padIdx][axisIdx];
					auto& delta_as_step = converted[padIdx][axisIdx];
					constexpr int step = 60;
					delta_as_step = delta / step;
					delta -= delta_as_step * step;
				}
			}
			return converted;
		}(*is);
		auto calc_scratch_delta = [&cfg_input, &is, &loopingDeltaStep](int& delta, int side) {
			for (int j = 0; j < 16; j++) {
				for (int dir = 0; dir < 2; dir++) {
					int id = cfg_input->buttonMap[10 + 20 * side + dir][j];
					if (id < AXIS_RANGE_BEGIN || id >= AXIS_RANGE_END) continue;
					auto type = cfg_input->axisType[10 + 20 * side + dir][j];
					if (type != AXISTYPE_LOOPING) continue;
					size_t axis_i = id - AXIS_RANGE_BEGIN;
					size_t padIdx = axis_i / 12;
					size_t axisIdx = axis_i % 12;
					size_t dirIdx = axisIdx % 2;
					axisIdx = axisIdx / 2;
					auto axisState = is->inputAxisState[padIdx][axisIdx][dirIdx][AXISTYPE_LOOPING];
					if (axisState == 0 || axisState == 3) continue;
					auto axisDelta = loopingDeltaStep[padIdx][axisIdx];
					if (dir) delta += std::abs(axisDelta);
					else delta -= std::abs(axisDelta);
				}
			}
		};

		if (player == 0) {
			for (int i = 0; i < 20; i++) {
				unsigned char &button = is->p1_buttonInput[i];
				button = 0;
				for (int j = 0; j < 16; j++) {
					int boundState = get_bound_state(*is, cfg_input->buttonMap[i][j], cfg_input->axisType[i][j]);
					if (button < boundState) button = boundState;
				}
			}

			for (int i = 0; i < 20; i++) {
				unsigned char &button = is->p2_buttonInput[i];
				button = 0;
				for (int j = 0; j < 16; j++) {
					int boundState = get_bound_state(*is, cfg_input->buttonMap[i + 20][j], cfg_input->axisType[i + 20][j]);
					if (button < boundState) button = boundState;
				}
			}

			auto& scratchDeltaP1 = is->p1_scratchDelta;
			auto& scratchDeltaP2 = is->p2_scratchDelta;
			scratchDeltaP1 = 0;
			scratchDeltaP2 = 0;
			calc_scratch_delta(scratchDeltaP1, 0);
			calc_scratch_delta(scratchDeltaP2, 1);
		}
		else if (player == 1) {
			for (int i = 0; i < 20; i++) {
				unsigned char &button = is->p1_buttonInput[i];
				button = 0;
				for (int j = 0; j < 16; j++) {
					int boundState = get_bound_state(*is, cfg_input->buttonMap[i][j], cfg_input->axisType[i][j]);
					if (button < boundState) button = boundState;
				}
			}

			for (int i = 0; i < 20; i++) {
				unsigned char &button = is->p1_buttonInput[i];
				for (int j = 0; j < 16; j++) {
					int boundState = get_bound_state(*is, cfg_input->buttonMap[i + 20][j], cfg_input->axisType[i + 20][j]);
					if (button < boundState) button = boundState;
				}
			}

			auto& scratchDeltaP1 = is->p1_scratchDelta;
			scratchDeltaP1 = 0;
			calc_scratch_delta(scratchDeltaP1, 0);
			calc_scratch_delta(scratchDeltaP1, 1);
		}
		else if (player == 2) { //TOFIX: never called
			for (int i = 0; i < 20; i++) {
				unsigned char &button = is->p2_buttonInput[i];
				for (int j = 0; j < 16; j++) {
					int boundState = get_bound_state(*is, cfg_input->buttonMap[i][j], cfg_input->axisType[i][j]);
					if (button < boundState) button = boundState;
				}
			}

			for (int i = 0; i < 20; i++) {
				unsigned char &button = is->p2_buttonInput[i];
				button = 0; //TOFIX : button = 0 should be up there
				for (int j = 0; j < 16; j++) {
					int boundState = get_bound_state(*is, cfg_input->buttonMap[i + 20][j], cfg_input->axisType[i + 20][j]);
					if (button < boundState) button = boundState;
				}
			}

			auto& scratchDeltaP2 = is->p2_scratchDelta;
			scratchDeltaP2 = 0;
			calc_scratch_delta(scratchDeltaP2, 0);
			calc_scratch_delta(scratchDeltaP2, 1);
		}
	}
	else {
		for (unsigned char &button : is->p1_buttonInput) {
			if (button == 1) button = 2;
			else if (button == 3) button = 0;
		}
		for (unsigned char &button : is->p2_buttonInput) {
			if (button == 1) button = 2;
			else if (button == 3) button = 0;
		}
	}

	//scaratch input
	if (is->p1_buttonInput[10] == 1 || is->p1_buttonInput[11] == 1) {
		is->p1_buttonInput[0] = 1;
	}
	else if (is->p1_buttonInput[10] == 2 || is->p1_buttonInput[11] == 2) {
		is->p1_buttonInput[0] = 2;
	}
	else if (is->p1_buttonInput[10] == 3 || is->p1_buttonInput[11] == 3) {
		is->p1_buttonInput[0] = 3;
	}

	if (is->p2_buttonInput[10] == 1 || is->p2_buttonInput[11] == 1) {
		is->p2_buttonInput[0] = 1;
	}
	else if (is->p2_buttonInput[10] == 2 || is->p2_buttonInput[11] == 2) {
		is->p2_buttonInput[0] = 2;
	}
	else if (is->p2_buttonInput[10] == 3 || is->p2_buttonInput[11] == 3) {
		is->p2_buttonInput[0] = 3;
	}

	return 1;
}

static void InitMIDIInput(void){
#ifdef _WIN32
	midi.controller_v = 0;
	midi.controller_n = 0;
	midi.unusedFC = 0x7f;
	UINT numDev = midiInGetNumDevs();
	if (numDev > 15) {
		numDev = 15;
	}

	HMIDIIN phmi;
	for (int i = 0; i < numDev; i++) {
		midiInOpen(&phmi, i, (DWORD_PTR)MIDIInProc, NULL, CALLBACK_FUNCTION);
		midiInStart(phmi);
		midi.phmiArray[i] = phmi;
	}
#endif // _WIN32
}

int InitInputStructure(inputStructure *is){ // called on scene begin
	std::ranges::fill_n(is->inputID, 0x600, 0);
	is->inputAxis = {};
	memset(is->inputAxisState, 0, sizeof(is->inputAxisState));
	memset(is->inputAxisLoopingDecay, 0, sizeof(is->inputAxisLoopingDecay));
	is->inputAxisFirstPoll = true;
	is->mouse_buttonL = 0;
	is->mouse_buttonR = 0;
	is->mouse_buttonW = 0;
	is->mouse_button4 = 0;
	is->mouse_moveflag = 0;
	is->mouse_recentMoveTime = GetTimeWrap();
	is->drag_start_time = -1;
	is->is_doubleclick = 0;
	is->is_drag_now = -1;
	InitMIDIInput();
	SetJoypadDefaultDeadZoneAll(is->inputAxisDeadzoneAll); // required so dinput deadzone can be applied in keyconfig scene, to prevent registering resting axis jitter, detectable in looping mode.
	return 1;
}
