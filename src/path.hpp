#pragma once

#include <filesystem>
#include <QUrl>

class Path {
public:
	Path(std::string p);
	std::filesystem::path path;
	bool used = false;
	QUrl thumbnail;
};
