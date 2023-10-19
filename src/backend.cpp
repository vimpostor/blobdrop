#include "backend.hpp"

#include <QDBusInterface>
#include <QGuiApplication>

#include "settings.hpp"

void Backend::quit_delayed(const std::chrono::milliseconds delay) {
	Settings::get()->disable_always_on_bottom();
	QTimer::singleShot(delay, []() { QCoreApplication::quit(); });
}

void Backend::send_drag_notification(const QList<QString> &uris) {
#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)
	auto session = QDBusConnection::sessionBus();
	auto msg = QDBusMessage::createMethodCall("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications", "Notify");
	QVariantMap hints;
	hints["x-kde-urls"] = uris;
	QString title = "Drag file";
	if (uris.size() > 1) {
		title += "s";
	}
	msg.setArguments({"blobdrop", 0U, "cursor-arrow", title, "", QStringList(), hints, 5000});
	session.call(msg);
#endif
}

void Backend::hide_terminal() {
#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)
	if (Settings::get()->supress_always_on_bottom || !xcb.init()) {
		return;
	}

	last_window = xcb.active_window();
	if (last_window) {
		xcb.set_keep_window_below(last_window);
	}
#endif
}

void Backend::restore_terminal() {
#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)
	if (!last_window) {
		return;
	}

	xcb.set_keep_window_below(last_window, false);
	last_window = 0;
#endif
}
