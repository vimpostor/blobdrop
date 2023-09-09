#pragma once

#include <QCoreApplication>
#include <QTimer>

#include "Util/util.hpp"

class Backend {
public:
	SINGLETON(Backend)

	void quit_delayed(const int delay = 100);
	void send_drag_notification(const QList<QString> &uris);
};
