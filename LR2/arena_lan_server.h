#pragma once

#include "arena_lan_net.h"

namespace openlr2::arena_lan::server {

bool Start(const char* host = "0.0.0.0", unsigned short port = 2222);
bool Stop();
[[nodiscard]] bool IsRunning();

} // namespace openlr2::arena_lan::server
