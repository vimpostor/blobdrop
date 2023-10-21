#include "path_registry.hpp"

void PathRegistry::add_path(Path p) {
	paths.emplace_back(p);
	if (!Util::is_wayland()) {
		Settings::get()->hide_gui_flag = true;
	}
	emit pathAdded(p);
}
