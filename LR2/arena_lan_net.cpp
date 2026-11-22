#include "arena_lan_net.h"

#include <mutex>
#include <unordered_map>

namespace openlr2::arena_lan {
namespace {

std::mutex gMu;
std::unordered_map<int, Address> gIdToAddr;
std::unordered_map<Address, int> gAddrToId;

} // namespace

int AddressToPlayerId(const Address& a) {
	std::lock_guard lock(gMu);
	auto it = gAddrToId.find(a);
	if (it != gAddrToId.end()) return it->second;
	const size_t h = std::hash<Address>{}(a);
	int id = static_cast<int>(h & 0x7fffffff);
	if (id == 0) id = 1;
	while (gIdToAddr.count(id) != 0 && !(gIdToAddr[id] == a)) {
		++id;
		if (id <= 0) id = 1;
	}
	gAddrToId[a] = id;
	gIdToAddr[id] = a;
	return id;
}

Address PlayerIdToAddress(int playerId) {
	std::lock_guard lock(gMu);
	auto it = gIdToAddr.find(playerId);
	if (it == gIdToAddr.end()) return {};
	return it->second;
}

void RememberAddress(const Address& a) {
	(void)AddressToPlayerId(a);
}

void ClearAddressMap() {
	std::lock_guard lock(gMu);
	gIdToAddr.clear();
	gAddrToId.clear();
}

} // namespace openlr2::arena_lan
