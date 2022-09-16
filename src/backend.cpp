#include "backend.hpp"

void Backend::quit_delayed() {
	QTimer::singleShot(1, []() { QCoreApplication::quit(); });
}
