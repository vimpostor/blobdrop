#pragma once

#include <filesystem>

class Path {
public:
	Path(std::string p);
	std::filesystem::path path;
	bool used = false;
};
