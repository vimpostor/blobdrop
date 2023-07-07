#include "backend.hpp"

#include "settings.hpp"

void Backend::quit_delayed() {
	Settings::get()->prepare_quit();
	QTimer::singleShot(100, []() { QCoreApplication::quit(); });
}
