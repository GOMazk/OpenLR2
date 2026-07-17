#pragma once

#include <vector>

constexpr size_t CUSTOM_PANELS_MAX = 50;

struct DSTdraw { /* 80bytes,4*0x14 */
	float x{ 0 };
	float y{ 0 };
	float w{ 0 };
	float h{ 0 };
	int sortID{ 0 };
	int time{ -1 };
	int acc{ 0 };
	int blend{ 0 };
	int filter{ 0 };
	int a{ 0 };
	int r{ 0 };
	int g{ 0 };
	int b{ 0 };
	float angle{ 0 };
	int center{ 0 };
	int grHandle{ -1 };
	int fontHandle{ -1 };
	int subHandle{ -1 };
	int align{ 0 };
	char isDrawBackbox{ 0 };
};

struct DSTstruct { /* 44bytes.4*0x0b */
	int n; /* (NULL) on file */
	int opt1; /* dst_option */
	int opt2; /* and dst_option */
	int opt3; /* and dst_option */
	int opt4; /* scratch */
	int opt5;
	int timer;
	struct DSTdraw* draw;
	int dataSize;
	int loop;
	int dstCount;
};

struct SRCstruct { /* 68bytes,4*0x11 */
	int n; /* (NULL) in file */
	int* grHandles; /* =fontHandle */
	int graphcount;
	int cycle; /* =font */
	int op1;
	int op2;
	int op3;
	int op4;
	int op5;
	int count;
	int timer;
	int fontHandle;
	int align;
	int st;
	int inArray;
	int sx;
	int sy;
};

class PanelManager {
private:
	class Panel {
	public:
		const Panel* mMaster = nullptr;
		std::vector<const Panel*> mSlaves;
		std::vector<const DSTstruct*> mButtonSlaves;
		SRCstruct mSRC{};
		DSTstruct mDST{};
		double mTimer = 0.;
		bool mIsActive = false;

		Panel() = default;
		Panel(const Panel&) = delete;
		Panel& operator=(const Panel&) = delete;
		Panel& operator=(Panel&&) = delete;
		Panel(Panel&&) noexcept;
		~Panel();
	};
	size_t GetIdx(Panel* ptr);
	std::vector<Panel> mPanels;
	Panel* mCurrentMaster = nullptr;
	Panel* mCurrentPanel = nullptr;
	bool mIsActive = false;
	bool mIsIgnoreDST = true;

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

	bool IsPanelOpen(size_t id); // 'panel' param in 10-59 range
	double GetTimer(size_t id); // 'timer' param in 500-549 range

	bool AddPanel(const SRCstruct& src);
	DSTstruct* GetLastDST();
	void Draw();
	void ProcessInput();
};