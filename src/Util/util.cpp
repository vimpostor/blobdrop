#include "util.hpp"

#include <cstdlib>
#include <format>
#include <iostream>
#include <pwd.h>
#include <unistd.h>

namespace Util {

const char *home_dir() {
	static const char *result = nullptr;
	if (!result) {
		result = getenv("HOME");
#ifdef Q_OS_UNIX
		if (!result) {
			result = getpwuid(getuid())->pw_dir;
		}
#endif
	}

	return result;
}

std::string pwd() {
	static std::string result;
	if (result.empty()) {
		result = std::filesystem::current_path().string() + std::filesystem::path::preferred_separator;
	}
	return result;
}

std::string print_osc8_link(const std::string &url, const std::string &text) {
	return std::format("\e]8;;{}\e\\{}\e]8;;\e\\", url, text);
}
}
