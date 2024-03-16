#include "path.hpp"

#include <QDesktopServices>

#include "Util/util.hpp"
#include "mimedb.hpp"
#include "settings.hpp"

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
	auto res = QUrl::fromLocalFile(QString::fromStdString(path.string()));

	if (Settings::get()->remote) {
		std::string host;
		if (Util::get_local_domain(host)) {
			res.setScheme("sftp");
			res.setUserName(QString::fromStdString(Util::get_username()));
			res.setHost(QString::fromStdString(host));
			// TODO: Consider supporting custom ports, e.g. parse $SSH_CLIENT
		} else {
			std::cerr << "Failed deducing remote prefix" << std::endl;
		}
	}

	return res;
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
