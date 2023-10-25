#include "path_registry.hpp"

void PathRegistry::add_path(Path p) {
	paths.emplace_back(p);
	if (!Util::is_wayland()) {
		Settings::get()->can_drag_immediately = true;
	}
	emit pathAdded(p);
}
