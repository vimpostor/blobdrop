#include "path_registry.hpp"

void PathRegistry::add_path(Path p) {
	paths.emplace_back(p);
#if defined(Q_OS_UNIX) && !defined(Q_OS_DARWIN)
	if (!quartz::util::is_wayland()) {
		// if no other frontend was explicitly selected,
		// we could begin an immediate drag now (once startup is complete)
		Settings::get()->can_drag_immediately = true;
	}
#endif
	emit pathAdded(p);
}
