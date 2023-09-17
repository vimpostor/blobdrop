#include "path_registry.hpp"

void PathRegistry::add_path(Path p) {
	paths.emplace_back(p);
	if (Settings::get()->frontend == Settings::Frontend::Auto && paths.size() == 1) {
		Settings::get()->hide_gui_flag = true;
	} else {
		Settings::get()->hide_gui_flag = false;
	}
	emit pathAdded(p);
}
