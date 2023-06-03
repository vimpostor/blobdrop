#include "mimedb.hpp"

std::string MimeDb::getIcon(const std::filesystem::path &p) const {
	std::string result = getMimetype(p).iconName().toStdString();
	if (result.empty()) {
		result = "text-x-generic";
	}
	return result;
}


QMimeType MimeDb::getMimetype(const std::filesystem::path &p) const {
	return db.mimeTypeForFile(QString::fromStdString(p.string()), QMimeDatabase::MatchExtension);
}
