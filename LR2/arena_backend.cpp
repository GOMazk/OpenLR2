#include "arena_backend.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

namespace openlr2::arena_backend {
namespace {

constexpr const char* kArenaIniPath = "LR2files/Config/arena.ini";
Transport gTransport{Transport::Ir};

} // namespace

Transport GetTransport() {
	return gTransport;
}

void SetTransport(Transport t) {
	gTransport = t;
}

void LoadTransportFromIni() {
	std::ifstream in(kArenaIniPath);
	if (!in) {
		gTransport = Transport::Ir;
		return;
	}
	std::string line;
	while (std::getline(in, line)) {
		if (line.rfind("transport=", 0) == 0) {
			const std::string v = line.substr(10);
			if (v == "lan" || v == "LAN" || v == "1") {
				gTransport = Transport::Lan;
			} else {
				gTransport = Transport::Ir;
			}
			return;
		}
	}
	gTransport = Transport::Ir;
}

void SaveTransportToIni() {
	std::vector<std::string> keep;
	{
		std::ifstream in(kArenaIniPath);
		std::string line;
		while (in && std::getline(in, line)) {
			if (line.rfind("transport=", 0) == 0) continue;
			if (!line.empty()) keep.push_back(line);
		}
	}
	std::ofstream out(kArenaIniPath, std::ios::trunc);
	if (!out) return;
	out << "transport=" << (gTransport == Transport::Lan ? "lan" : "ir") << '\n';
	for (const auto& l : keep) out << l << '\n';
}

Backend& Active() {
	return gTransport == Transport::Lan ? Lan() : Ir();
}

} // namespace openlr2::arena_backend
