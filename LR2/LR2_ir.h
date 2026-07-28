#pragma once

#include "strclass.h"
#include <optional>
#include <string>

struct Url
{
	// Minimal RFC 3986 validation https://www.rfc-editor.org/info/rfc3986/#section-2
	// To reject obviously bad URLs, like those containing non-ASCII symbols or double quotes.
	static std::optional<Url> parse(std::string value);
	std::string value;
};

int CheckRivaldataNew(int rivalID);
CSTR UrlEncode(CSTR in);
[[nodiscard]] std::string LR2IR_GetWebRankingUrl(CSTR songmd5);
int OpenUrl(const Url& url);

int SaveIRID(int IRID, CSTR ID);
