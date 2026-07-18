#include "LR2_panels.h"
#include "LR2_skindraw.h"
#include "En_timer.h"
#include <DxLib.h>
#include <ranges>
#include <optional>
#include <algorithm>

static auto is_ptr_legit = []<typename T>(const T* ptr, const std::vector<T>& arr) {
	if (ptr < arr.data()) return false;
	if (ptr >= arr.data() + arr.size()) return false;
	if ((reinterpret_cast<uintptr_t>(ptr) - reinterpret_cast<uintptr_t>(arr.data())) % sizeof(T) != 0) return false;
	return true;
};

static auto is_idx_legit = []<typename T>(size_t idx, const std::vector<T>&arr) {
	if (idx >= arr.size()) return false;
	return true;
};

void PanelManager::Panel::Open() {
	mIsActive = true;
	mTimer = GetTimeWrap();
}

void PanelManager::Panel::Close() {
	mIsActive = false;
	mTimer = -1.;
}

size_t PanelManager::GetIdx(const Panel* ptr) {
	if (is_ptr_legit(ptr, mPanels)) return ptr - mPanels.data();
	return 0;
}

bool PanelManager::IterateMaster(bool plus) {
	if (mPanels.empty()) return false;
	auto get_next = []<typename T>(T* now, std::vector<T>&arr, bool plus) {
		auto iterate_plus = [&](T* now) {
			if (now == arr.data() + arr.size() - 1) return &arr[0];
			return now + 1;
		};
		auto iterate_minus = [&](T* now) {
			if (now == arr.data()) return arr.data() + arr.size() - 1;
			return now - 1;
		};
		auto next = plus ? iterate_plus(now) : iterate_minus(now);
		return next;
	};
	auto next = get_next(mCurrentMaster, mPanels, plus);
	while (next->mMaster) next = get_next(mCurrentMaster, mPanels, plus);
	if (next == mCurrentMaster) return false;
	mCurrentMaster->Close();
	mCurrentPanel->Close();
	next->Open();
	mCurrentMaster = next;
	mCurrentPanel = mCurrentMaster;
	return true;
}

bool PanelManager::IterateSlave(bool plus) {
	if (mCurrentMaster->mSlaves.empty()) return false;
	if (mCurrentPanel == mCurrentMaster) {
		mCurrentPanel = const_cast<Panel*>(mCurrentMaster->mSlaves[0]);
		mCurrentPanel->Open();
		return true;
	}
	auto get_next = []<typename T>(T* now, std::vector<const T*>&arr, bool plus) {
		auto iterate_plus = [&](T* now) {
			if (now == *(arr.data() + arr.size() - 1)) return arr[0];
			return *(std::ranges::find(arr, now) + 1);
		};
		auto iterate_minus = [&](T* now) {
			if (now == *arr.data()) return *(arr.data() + arr.size() - 1);
			return *(std::ranges::find(arr, now) - 1);
		};
		auto next = plus ? iterate_plus(now) : iterate_minus(now);
		return next;
	};
	auto next = get_next(mCurrentPanel, mCurrentMaster->mSlaves, plus);
	mCurrentPanel->Close();
	if (next == mCurrentPanel) {
		mCurrentPanel = mCurrentMaster;
	}
	else {
		mCurrentPanel = const_cast<Panel*>(next);
	}
	mCurrentPanel->Open();
	return true;
}

bool PanelManager::NextPanel() {
	return IterateMaster(true);
}

bool PanelManager::PrevPanel() {
	return IterateMaster(false);
}

bool PanelManager::NextSubPanel() {
	return IterateSlave(true);
}

bool PanelManager::PrevSubPanel() {
	return IterateSlave(false);
}

bool PanelManager::Toggle() {
	if (!mIsActive) {
		return Open();
	}
	else {
		Close();
		return false;
	}
}

bool PanelManager::Open() {
	if (mCurrentMaster == nullptr || mCurrentPanel == nullptr) return false;
	mIsActive = true;
	mCurrentMaster->Open();
	mCurrentPanel->Open();
	return true;
}

bool PanelManager::Close() {
	mIsActive = false;
	if (mCurrentMaster) mCurrentMaster->Close();
	if (mCurrentPanel) mCurrentPanel->Close();
	return true;
}

bool PanelManager::IsPanelActive(size_t id) {
	id -= 10;
	if (!is_idx_legit(id, mPanels)) return false;
	return mPanels[id].mIsActive;
}

double PanelManager::GetTimer(size_t id) {
	id -= 10;
	if (!is_idx_legit(id, mPanels)) return -1.;
	return mPanels[id].mTimer;
}

bool PanelManager::AddPanel(const SRCstruct& src) {
	auto fail = [&]() {
		mIsIgnoreDST = true;
		return false;
	};
	if (mPanels.size() == CUSTOM_PANELS_MAX) return fail();
	auto masterIdx = [&]() {
		std::optional<size_t> ret = {};
		if (src.op1) {
			size_t masterIdx = src.op1 - 10;
			if (!is_idx_legit(masterIdx, mPanels)) return ret;
			if (mPanels[masterIdx].mMaster) return ret;
			return ret = masterIdx;
		}
		return ret;
	}();
	if (src.op1 && !masterIdx) return fail();
	auto& panel = mPanels.emplace_back();
	panel.mSRC = src;
	if (masterIdx) {
		auto& master = mPanels[*masterIdx];
		panel.mMaster = &master;
		master.mSlaves.push_back(&panel);
	}
	if (mPanels.size() == 1) {
		mCurrentMaster = &panel;
		mCurrentPanel = &panel;
	}
	mIsIgnoreDST = false;
	return true;
}

bool PanelManager::BindButton(SRCstruct* src) {
	size_t idx = src->op3 - 10;
	if (!is_idx_legit(idx, mPanels)) return false;
	auto& panel = mPanels[idx];
	if (panel.mMaster) src->op3 = GetIdx(panel.mMaster) + 10;
	mPanels[idx].mBoundButtons.push_back(src);
	return true;
}

PanelManager::Panel::Panel(Panel&& other) noexcept {
	std::swap(mMaster, other.mMaster);
	std::swap(mSlaves, other.mSlaves);
	std::swap(mBoundButtons, other.mBoundButtons);
	std::swap(mSRC, other.mSRC);
	std::swap(mDST, other.mDST);
	std::swap(mTimer, other.mTimer);
	std::swap(mIsActive, other.mIsActive);
}

PanelManager::Panel::~Panel() {
	for (int i = 0; i < mSRC.graphcount; i++) {
		DeleteGraph(mSRC.grHandles[i]);
	}
	free(mSRC.grHandles);
	free(mDST.draw);
}

DSTstruct* PanelManager::GetLastDST() {
	if (mIsIgnoreDST) return nullptr;
	return &mPanels.back().mDST;
}

void PanelManager::Draw(DrawingBuf* drb) {
	auto AddDrawingBuffer_Panel = [&](const SRCstruct& src, const DSTstruct& dst, double timer) {
		if (dst.dstCount <= 0 || dst.dataSize <= 0) return;
		double timeLapse = timer >= 0. ? GetTimeWrap() - timer : -1.;
		DSTdraw tDstd = SetDSTdrawByTime(dst, timeLapse);

		if (tDstd.time != -1 && src.inArray < src.graphcount && src.inArray > -1) {
			AddDrawingBuffer(drb, src.grHandles[src.inArray], &tDstd);
		}
	};
	for (const auto& panel : mPanels) {
		AddDrawingBuffer_Panel(panel.mSRC, panel.mDST, panel.mMaster ? panel.mMaster->mTimer : panel.mTimer);
	}
}

int PanelManager::CheckInput(const SRCstruct* src, const inputStructure* input) {
	if (mCurrentPanel == nullptr) return 0; 
	if (!mCurrentPanel->mIsActive) return 0;
	auto it = std::ranges::find(mCurrentPanel->mBoundButtons, src);
	if (it == mCurrentPanel->mBoundButtons.end()) return 0;
	auto idx = std::distance(mCurrentPanel->mBoundButtons.begin(), it);
	if (idx > 7) return 0;
	return input->p1_buttonInput[idx] | input->p2_buttonInput[idx];
}
