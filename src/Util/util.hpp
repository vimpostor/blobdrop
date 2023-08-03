#pragma once

#include <filesystem>
#include <string>

#define SINGLETON(TYPE) \
	static TYPE *get() { \
		static TYPE s; \
		return &s; \
	}

#define STRICT_SINGLETON(TYPE) \
	TYPE() = delete; \
	explicit TYPE(bool singleton_constructor) {}; \
	static TYPE *get() { \
		static TYPE s {true}; \
		return &s; \
	}

namespace Util {

const char *home_dir();
std::string pwd();
std::string print_osc8_link(const std::string &url, const std::string &text);
}
