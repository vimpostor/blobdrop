#pragma once

#include <QCoreApplication>
#include <QTimer>
#include <chrono>

#include "Util/util.hpp"

#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)
#include "xcb.hpp"
#endif

class Backend {
public:
	SINGLETON(Backend)

	void quit_delayed(const std::chrono::milliseconds delay = 100ms, bool force = false);
	void send_drag_notification(const QList<QString> &uris);

	void hide_terminal();
	void restore_terminal();
private:
#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)
	Xcb xcb;
	xcb_window_t last_window = 0;
#endif
};
