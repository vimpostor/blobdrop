#include "util.hpp"

#include <cstdlib>
#include <pwd.h>
#include <unistd.h>

namespace Util {

const char *home_dir() {
	static const char *result = nullptr;
	if (!result) {
		result = getenv("HOME");
		if (!result) {
			result = getpwuid(getuid())->pw_dir;
		}
	}

	return result;
}

}
