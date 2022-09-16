#include "mimedb.hpp"

std::string MimeDb::getIcon(const std::filesystem::path p) const {
	std::string result = db.mimeTypeForFile(QString::fromStdString(p.string()), QMimeDatabase::MatchExtension).iconName().toStdString();
	if (result.empty()) {
		result = "text-x-generic";
	}
	return result;
}
