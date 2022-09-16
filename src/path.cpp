#include "path.hpp"

#include <QDebug>
#include <QMimeDatabase>
#include <QMimeType>
#include <cstring>
#include <iostream>

#include "Util/util.hpp"
#include "mimedb.hpp"

Path::Path(const std::string &p)
	: path(std::filesystem::absolute(p)) {
	try {
		exists = std::filesystem::exists(path);
	} catch (const std::filesystem::filesystem_error &ex) {
		std::cerr << ex.what() << std::endl;
	}
	iconName = MimeDb::get()->getIcon(path);
}

std::string Path::get_uri() const {
	return "file://" + static_cast<std::string>(path);
}

std::string Path::pretty_print() const {
	std::string result = path.string();
	const auto home = Util::home_dir();
	if (result.starts_with(home)) {
		result.replace(0, std::strlen(home), "~");
	}
	return result;
}
