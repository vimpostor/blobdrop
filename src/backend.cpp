#include "backend.hpp"

void Backend::quit_delayed() {
	QTimer::singleShot(100, []() { QCoreApplication::quit(); });
}
