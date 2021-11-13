#include "path.hpp"

Path::Path(const std::string& p) : path(std::filesystem::absolute(p)) {
	try {
		exists = std::filesystem::exists(path);
	} catch(const std::filesystem::filesystem_error& ex) {
		std::cerr << ex.what() << std::endl;
	}
}

std::string Path::get_uri() const {
	return "file://" + static_cast<std::string>(path);
}
