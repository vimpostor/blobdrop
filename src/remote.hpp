#pragma once

#include <QUrl>

#include "Util/util.hpp"

class Remote {
public:
	SINGLETON(Remote)
	bool rewire_url(QUrl &url);
	void hardcode_prefix(const QString &prefix);
private:
	void init();
	QString get_local_domain();
	QString get_username();
	int get_port();

	bool init_done = false;
	bool ok = false;

	QString scheme = "sftp";
	QString username;
	QString host;
	int port = -1;
};
