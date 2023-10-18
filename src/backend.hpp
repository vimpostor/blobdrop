#pragma once

#include <QCoreApplication>
#include <QTimer>
#include <chrono>

#include "Util/util.hpp"
#include "xcb.hpp"

class Backend {
public:
	SINGLETON(Backend)

	void quit_delayed(const std::chrono::milliseconds delay = 100ms);
	void send_drag_notification(const QList<QString> &uris);

	void hide_terminal();
	void restore_terminal();
private:
	Xcb xcb;
	xcb_window_t last_window = 0;
};
