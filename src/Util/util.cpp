#include "util.hpp"

#include <QGuiApplication>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

#ifdef Q_OS_UNIX
#include <netdb.h>
#include <pwd.h>
#include <unistd.h>
#endif

namespace Util {

const char *home_dir() {
	static const char *result = nullptr;
	if (!result) {
		result = getenv("HOME");
#ifdef Q_OS_UNIX
		if (!result) {
			result = getpwuid(getuid())->pw_dir;
		}
#endif
	}

	return result;
}

std::string pwd() {
	static std::string result;
	if (result.empty()) {
		result = std::filesystem::current_path().string();
	}
	return result;
}

bool get_local_domain(std::string &result) {
	static std::string cached_result;
	if (!cached_result.empty()) {
		result = cached_result;
		return true;
	}

#ifndef Q_OS_UNIX
	return false;
#else
	char host[HOST_NAME_MAX];
	if (gethostname(host, HOST_NAME_MAX)) {
		std::cerr << "gethostname failed" << std::endl;
		return false;
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
		return false;
	}

	const auto fqdn = res->ai_canonname;
	bool success = true;

	if (!strncmp(fqdn, host, HOST_NAME_MAX)) {
		std::cerr << "failed to retreive a proper FQDN for " << host << std::endl;
		success = false;
	} else {
		result = fqdn;

		// remove possible leading dot
		if (result.starts_with('.')) {
			result = result.substr(1);
		}

		cached_result = result;
	}

	freeaddrinfo(res);
	return success;
#endif
}

std::string get_username() {
#ifdef Q_OS_UNIX
	return getlogin();
#else
	return {};
#endif
}
}
