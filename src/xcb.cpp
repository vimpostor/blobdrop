#include "xcb.hpp"

#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)

#include <QGuiApplication>

// TODO: Actually resolve the symbols with dlopen(), or by using QLibrary

bool Xcb::init() {
	if (ok) {
		// don't need to init twice
		return true;
	}

	const auto qt_x11 = qGuiApp->nativeInterface<QNativeInterface::QX11Application>();
	if (!qt_x11) {
		return false;
	}
	// reuse the Qt xcb connection
	conn = qt_x11->connection();
	if (!conn) {
		return false;
	}

	// init ewmh connection
	ok = xcb_ewmh_init_atoms_replies(&ewmh, xcb_ewmh_init_atoms(conn, &ewmh), nullptr);
	return ok;
}

xcb_window_t Xcb::active_window() {
	xcb_window_t res;
	// try to get the currently active/focused window
	// Notice that we pass 0 for screen_nbr, which is not quite correct.
	// We should pass the default screen number instead, which we can obtain via the second parameter to xcb_connect().
	// https://xcb.freedesktop.org/xlibtoxcbtranslationguide/
	// Unfortunately, we don't have that as we just grab the connection from Qt.
	//
	// Apparently previously Qt could return the default screen number with QX11Info::appScreen(),
	// but that is not available in Qt6 anymore.
	if (!xcb_ewmh_get_active_window_reply(&ewmh, xcb_ewmh_get_active_window_unchecked(&ewmh, 0), &res, nullptr)) {
		qWarning() << "Cannot get active window";
		return 0;
	}
	return res;
}

void Xcb::set_keep_window_below(xcb_window_t window, const bool value) {
	const auto v = value ? XCB_EWMH_WM_STATE_ADD : XCB_EWMH_WM_STATE_REMOVE;

	// add the corresponding enum value to the window's _NET_WM_STATE
	// this method can set two values at once (e.g. to maximize both vertically and horizontally in one call),
	// but we don't need that, so we set the second parameter to XCB_ATOM_NONE
	std::ignore = xcb_ewmh_request_change_wm_state(&ewmh, 0, window, v, ewmh._NET_WM_STATE_BELOW, XCB_ATOM_NONE, XCB_EWMH_CLIENT_SOURCE_TYPE_NORMAL);
	// this is needed so that the call actually takes effect
	std::ignore = xcb_get_window_attributes_reply(conn, xcb_get_window_attributes(conn, window), nullptr);

	// write pending data to the socket
	xcb_flush(conn);
}

#endif
