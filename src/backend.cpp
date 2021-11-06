#include "backend.hpp"

void Backend::quit_delayed() {
	QTimer::singleShot(10, [](){ QCoreApplication::quit(); });
}
