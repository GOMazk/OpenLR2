#include "LR2_panels.h"
#include <DxLib.h>
#include <ranges>
#include <optional>
#include <algorithm>

static auto is_pointer_legit = []<typename T>(const T* ptr, const std::vector<T>& arr) {
	if (ptr < arr.data()) return false;
	if (ptr >= arr.data() + arr.size()) return false;
	if ((reinterpret_cast<uintptr_t>(ptr) - reinterpret_cast<uintptr_t>(arr.data())) % sizeof(T) != 0) return false;
	return true;
};

size_t PanelManager::GetIdx(Panel* ptr) {
	if (is_pointer_legit(ptr, mPanels)) return ptr - mPanels.data();
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
	mCurrentMaster = next;
	mCurrentPanel = mCurrentMaster;
	return true;
}

bool PanelManager::IterateSlave(bool plus) {
	if (mCurrentMaster->mSlaves.empty()) return false;
	if (mCurrentPanel == mCurrentMaster) {
		mCurrentPanel = const_cast<Panel*>(mCurrentMaster->mSlaves[0]);
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
	if (next == mCurrentPanel) {
		mCurrentPanel = mCurrentMaster;
	}
	else {
		mCurrentPanel = const_cast<Panel*>(next);
	}
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
	if (mCurrentMaster == nullptr) return false;
	mIsActive = true;
	return true;
}

bool PanelManager::Close() {
	mIsActive = false;
	return true;
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
			if (masterIdx > 49) return ret;
			if (masterIdx >= mPanels.size()) return ret;
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

PanelManager::Panel::Panel(Panel&& other) noexcept {
	std::swap(mMaster, other.mMaster);
	std::swap(mSlaves, other.mSlaves);
	std::swap(mButtonSlaves, other.mButtonSlaves);
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
