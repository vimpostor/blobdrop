#include "path.hpp"

Path::Path(std::string p) : path(std::filesystem::absolute(p)) {
}
