#pragma once

#include <vector>

constexpr size_t CUSTOM_PANELS_MAX = 50;

struct inputStructure;
struct AUDIO;
struct Timer;
struct SRCstruct;
struct CSVbuf;

class PanelManager {
private:
	class Panel {
	public:
		const Panel* mMaster = nullptr;
		std::vector<const Panel*> mSlaves;
		std::vector<const SRCstruct*> mBoundButtons;
		double* mTimerOpen = nullptr;
		double* mTimerClose = nullptr;
		bool mIsActive = false;
		bool mIsSelectable = false;

		void Open();
		void Close();
	};
	size_t GetIdx(const Panel* ptr);
	std::vector<Panel> mPanels;
	Timer* mTimers = nullptr;
	Panel* mCurrentMaster = nullptr;
	Panel* mCurrentPanel = nullptr;
	bool mIsActive = false;

	bool IterateMaster(bool plus);
	bool IterateSlave(bool plus);
public:
	bool NextPanel();
	bool PrevPanel();
	bool NextSubPanel();
	bool PrevSubPanel();
	bool Toggle();
	bool Open();
	bool Close();

	bool IsPanelActive(size_t id); // 'panel' param in 10-59 range
	// PanelManager controls the timers its panels use internally,
	// but uses global timer struct to hold them, so they can be
	// accessed from the rest of the code normally.
	// Range 500-549 for open timers, 550-599 for close timers.
	Timer* GetTimersPtr();

	bool AddPanel(const CSVbuf& csv);
	bool BindButton(SRCstruct* src);
	int CheckButton(const SRCstruct* src, const inputStructure* input);
	bool RunSelectorInput(const inputStructure* input, AUDIO* audio);

	PanelManager() = default;
	PanelManager(Timer* timers) : mTimers(timers) {};
};