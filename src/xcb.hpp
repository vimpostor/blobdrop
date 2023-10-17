#pragma once

#include <xcb/xcb_ewmh.h>
#include <xcb/xproto.h>

class Xcb {
public:
	bool init();
	xcb_window_t active_window();
	void set_keep_window_below(xcb_window_t window, const bool value = true);
private:
	bool ok = false;
	xcb_connection_t *conn = nullptr;
	xcb_ewmh_connection_t ewmh;
};
