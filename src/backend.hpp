#pragma once

#include <QCoreApplication>
#include <QTimer>

#include "Util/util.hpp"
#include "xcb.hpp"

class Backend {
public:
	SINGLETON(Backend)

	void quit_delayed(const int delay = 100);
	void send_drag_notification(const QList<QString> &uris);

	void hide_terminal();
	void restore_terminal();
private:
	Xcb xcb;
	xcb_window_t last_window = 0;
};
