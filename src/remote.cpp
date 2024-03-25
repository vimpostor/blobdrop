#include "remote.hpp"

#include <iostream>

#ifdef Q_OS_UNIX
#include <netdb.h>
#include <unistd.h>
#endif

bool Remote::rewire_url(QUrl &url) {
	if (!init_done) {
		init();
	}
	if (!ok) {
		return false;
	}

	url.setScheme("sftp");
	url.setUserName(username);
	url.setHost(host);
	url.setPort(port);

	return true;
}

void Remote::init() {
	init_done = true;

	username = get_username();
	if (username.isEmpty()) {
		std::cerr << "Could not read username" << std::endl;
		return;
	}

	host = get_local_domain();
	if (host.isEmpty()) {
		std::cerr << "Could not read host" << std::endl;
		return;
	}

	// not a big deal if this fails, usually it still works without an explicit port
	port = get_port();

	ok = true;
}

QString Remote::get_local_domain() {
#ifndef Q_OS_UNIX
	return {};
#else
	char host[HOST_NAME_MAX];
	if (gethostname(host, HOST_NAME_MAX)) {
		std::cerr << "gethostname failed" << std::endl;
		return {};
	}

	// got the host, try to get the FQDN
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_CANONNAME;

	int err = getaddrinfo(host, "http", &hints, &res);
	if (err) {
		std::cerr << "getaddrinfo failed: " << gai_strerror(err) << std::endl;
		return {};
	}

	const auto fqdn = res->ai_canonname;
	std::string result;

	if (!strncmp(fqdn, host, HOST_NAME_MAX)) {
		std::cerr << "failed to retreive a proper FQDN for " << host << std::endl;
	} else {
		result = fqdn;

		// remove possible leading dot
		if (result.starts_with('.')) {
			result = result.substr(1);
		}
	}

	freeaddrinfo(res);
	return QString::fromStdString(result);
#endif
}

QString Remote::get_username() {
#ifdef Q_OS_UNIX
	return getlogin();
#else
	return {};
#endif
}

int Remote::get_port() {
	const auto env = std::getenv("SSH_CONNECTION");
	if (env) {
		std::string ssh_connection {env};
		const auto n = ssh_connection.rfind(' ');
		if (n != std::string::npos) {
			try {
				return std::stoi(ssh_connection.substr(n + 1));
			} catch (std::invalid_argument const &) {
				std::cerr << "Failed to parse port number " << ssh_connection << std::endl;
			}
		}
	}
	return -1;
}
