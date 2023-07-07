#include "backend.hpp"

#include "settings.hpp"

void Backend::quit_delayed() {
	Settings::get()->disable_always_on_bottom();
	QTimer::singleShot(100, []() { QCoreApplication::quit(); });
}
