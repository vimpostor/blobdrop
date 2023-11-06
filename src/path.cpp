#include "path.hpp"

#include <QDesktopServices>

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

	// If this is an image, initialize the thumbnail
	if (MimeDb::get()->getMimetype(path).name().startsWith("image")) {
		// Just reuse the file itself, QML itself can later create an image on demand from this
		thumbnail = get_url();
	}
}

std::string Path::get_uri() const {
	return get_url().toString().toStdString();
}

QUrl Path::get_url() const {
	return QUrl::fromLocalFile(QString::fromStdString(path.string()));
}

std::string Path::pretty_print() const {
	std::string result = path.string();

	const auto pwd = Util::pwd() + std::string(1, std::filesystem::path::preferred_separator);
	const auto home = Util::home_dir();
	if (result.starts_with(pwd)) {
		result = result.substr(pwd.length());
	} else if (home && result.starts_with(home)) {
		result.replace(0, std::strlen(home), "~");
	}

	return result;
}

bool Path::open() const {
	return QDesktopServices::openUrl(get_url());
}
