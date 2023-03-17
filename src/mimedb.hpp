#pragma once

#include <filesystem>
#include <string>

#include <QMimeDatabase>

#include "Util/util.hpp"

class MimeDb {
public:
	SINGLETON(MimeDb)
	std::string getIcon(const std::filesystem::path &p) const;
private:
	QMimeDatabase db;
};
