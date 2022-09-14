#pragma once

#include <QUrl>
#include <filesystem>
#include <iostream>

class Path {
public:
	Path(const std::string &p);
	std::filesystem::path path;
	bool used = false;
	QUrl thumbnail;
	bool exists = false;

	std::string get_uri() const;
};
