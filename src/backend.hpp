#pragma once

#include <QCoreApplication>
#include <QTimer>

#include "Util/util.hpp"

class Backend {
public:
	SINGLETON(Backend)

	void quit_delayed();
	void send_drag_notification(const QString &uri);
};
