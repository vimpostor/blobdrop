#include "backend.hpp"

#include <QDBusInterface>
#include <QGuiApplication>
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>

#include "settings.hpp"

void Backend::quit_delayed(const int delay) {
	Settings::get()->disable_always_on_bottom();
	QTimer::singleShot(delay, []() { QCoreApplication::quit(); });
}

void Backend::send_drag_notification(const QList<QString> &uris) {
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
}

void Backend::hide_terminal() {
	if (Settings::get()->supress_always_on_bottom) {
		return;
	}

	const auto qt_x11 = qGuiApp->nativeInterface<QNativeInterface::QX11Application>();
	xcb_connection_t *xcb_conn = nullptr;
	if (qt_x11) {
		xcb_conn = qt_x11->connection();
	} else {
		xcb_conn = xcb_connect(nullptr, nullptr);
	}

	if (xcb_connection_has_error(xcb_conn) > 0) {
		qWarning() << "Cannot connect to xcb";
	} else {
		xcb_ewmh_connection_t ewmh;
		xcb_ewmh_init_atoms(xcb_conn, &ewmh);
		if (!xcb_ewmh_init_atoms_replies(&ewmh, xcb_ewmh_init_atoms(xcb_conn, &ewmh), nullptr)) {
			qWarning() << "Cannot connect to ewmh";
		} else {
			if (!xcb_ewmh_get_active_window_reply(&ewmh, xcb_ewmh_get_active_window_unchecked(&ewmh, 0), &last_window, nullptr)) {
				qWarning() << "Cannot get active window";
			} else {
				std::ignore = xcb_ewmh_request_change_wm_state(&ewmh, 0, last_window, XCB_EWMH_WM_STATE_ADD, ewmh._NET_WM_STATE_BELOW, XCB_ATOM_NONE, XCB_EWMH_CLIENT_SOURCE_TYPE_NORMAL);
				std::ignore = xcb_get_window_attributes_reply(xcb_conn, xcb_get_window_attributes(xcb_conn, last_window), nullptr);
				xcb_flush(xcb_conn);
			}
		}
	}

	if (!qt_x11) {
		xcb_disconnect(xcb_conn);
	}
}

void Backend::restore_terminal() {
	if (!last_window) {
		return;
	}

	const auto qt_x11 = qGuiApp->nativeInterface<QNativeInterface::QX11Application>();
	xcb_connection_t *xcb_conn = nullptr;
	if (qt_x11) {
		xcb_conn = qt_x11->connection();
	} else {
		xcb_conn = xcb_connect(nullptr, nullptr);
	}

	if (xcb_connection_has_error(xcb_conn) > 0) {
		qWarning() << "Cannot connect to xcb";
	} else {
		xcb_ewmh_connection_t ewmh;
		xcb_ewmh_init_atoms(xcb_conn, &ewmh);
		if (!xcb_ewmh_init_atoms_replies(&ewmh, xcb_ewmh_init_atoms(xcb_conn, &ewmh), nullptr)) {
			qWarning() << "Cannot connect to ewmh";
		} else {
			std::ignore = xcb_ewmh_request_change_wm_state(&ewmh, 0, last_window, XCB_EWMH_WM_STATE_REMOVE, ewmh._NET_WM_STATE_BELOW, XCB_ATOM_NONE, XCB_EWMH_CLIENT_SOURCE_TYPE_NORMAL);
			std::ignore = xcb_get_window_attributes_reply(xcb_conn, xcb_get_window_attributes(xcb_conn, last_window), nullptr);
			xcb_flush(xcb_conn);
		}
	}

	if (!qt_x11) {
		xcb_disconnect(xcb_conn);
	}
}
