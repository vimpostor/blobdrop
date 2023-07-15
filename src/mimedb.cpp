#include "mimedb.hpp"

std::string MimeDb::getIcon(const std::filesystem::path &p) const {
	if (std::filesystem::is_directory(p)) {
		/**
		 * Early return for directories:
		 * mimeTypeForFile incorrectly returns application-octet-stream for them
		*/
		return "inode-directory";
	}

	std::string result = getMimetype(p).iconName().toStdString();
	if (result.empty()) {
		result = "text-x-generic";
	}
	return result;
}


QMimeType MimeDb::getMimetype(const std::filesystem::path &p) const {
	return db.mimeTypeForFile(QString::fromStdString(p.string()), QMimeDatabase::MatchExtension);
}
