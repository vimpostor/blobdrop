#include "path.hpp"

Path::Path(std::string p) : path(std::filesystem::absolute(p)) {
	exists = std::filesystem::exists(path);
}
