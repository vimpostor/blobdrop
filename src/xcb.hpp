#pragma once

#include <qglobal.h>

#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)

// The following is really cursed:
// The intention is to use xcb, but we do not want to link to xcb (or require it to be installed),
// because then Wayland nerds will be angry.
// Actually, xcb is not the problem, that's installed on every Wayland system anyway, because they have XWayland.
// But we require much more specific stuff, specifically all the ewmh extra stuff from xcb-util-wm
//
// We resolve symbols at runtime with dlopen(3), but we need to store some structs.
// For some structs it is not sufficient to have the opaque representation, but we actually need the definition.
// Hence to not require xcb headers, we have to copy-paste some definitions here.
// Now go away, and go judge someone else, thank you!
typedef uint32_t xcb_atom_t;
typedef uint32_t xcb_window_t;
typedef struct xcb_connection_t xcb_connection_t;
typedef uint32_t xcb_colormap_t;
typedef uint32_t xcb_visualid_t;
typedef struct xcb_screen_t {
	xcb_window_t root;
	xcb_colormap_t default_colormap;
	uint32_t white_pixel;
	uint32_t black_pixel;
	uint32_t current_input_masks;
	uint16_t width_in_pixels;
	uint16_t height_in_pixels;
	uint16_t width_in_millimeters;
	uint16_t height_in_millimeters;
	uint16_t min_installed_maps;
	uint16_t max_installed_maps;
	xcb_visualid_t root_visual;
	uint8_t backing_stores;
	uint8_t save_unders;
	uint8_t root_depth;
	uint8_t allowed_depths_len;
} xcb_screen_t;
typedef struct {
	xcb_connection_t *connection;
	xcb_screen_t **screens;
	int nb_screens;
	xcb_atom_t *_NET_WM_CM_Sn;
	xcb_atom_t _NET_SUPPORTED;
	xcb_atom_t _NET_CLIENT_LIST;
	xcb_atom_t _NET_CLIENT_LIST_STACKING;
	xcb_atom_t _NET_NUMBER_OF_DESKTOPS;
	xcb_atom_t _NET_DESKTOP_GEOMETRY;
	xcb_atom_t _NET_DESKTOP_VIEWPORT;
	xcb_atom_t _NET_CURRENT_DESKTOP;
	xcb_atom_t _NET_DESKTOP_NAMES;
	xcb_atom_t _NET_ACTIVE_WINDOW;
	xcb_atom_t _NET_WORKAREA;
	xcb_atom_t _NET_SUPPORTING_WM_CHECK;
	xcb_atom_t _NET_VIRTUAL_ROOTS;
	xcb_atom_t _NET_DESKTOP_LAYOUT;
	xcb_atom_t _NET_SHOWING_DESKTOP;
	xcb_atom_t _NET_CLOSE_WINDOW;
	xcb_atom_t _NET_MOVERESIZE_WINDOW;
	xcb_atom_t _NET_WM_MOVERESIZE;
	xcb_atom_t _NET_RESTACK_WINDOW;
	xcb_atom_t _NET_REQUEST_FRAME_EXTENTS;
	xcb_atom_t _NET_WM_NAME;
	xcb_atom_t _NET_WM_VISIBLE_NAME;
	xcb_atom_t _NET_WM_ICON_NAME;
	xcb_atom_t _NET_WM_VISIBLE_ICON_NAME;
	xcb_atom_t _NET_WM_DESKTOP;
	xcb_atom_t _NET_WM_WINDOW_TYPE;
	xcb_atom_t _NET_WM_STATE;
	xcb_atom_t _NET_WM_ALLOWED_ACTIONS;
	xcb_atom_t _NET_WM_STRUT;
	xcb_atom_t _NET_WM_STRUT_PARTIAL;
	xcb_atom_t _NET_WM_ICON_GEOMETRY;
	xcb_atom_t _NET_WM_ICON;
	xcb_atom_t _NET_WM_PID;
	xcb_atom_t _NET_WM_HANDLED_ICONS;
	xcb_atom_t _NET_WM_USER_TIME;
	xcb_atom_t _NET_WM_USER_TIME_WINDOW;
	xcb_atom_t _NET_FRAME_EXTENTS;
	xcb_atom_t _NET_WM_PING;
	xcb_atom_t _NET_WM_SYNC_REQUEST;
	xcb_atom_t _NET_WM_SYNC_REQUEST_COUNTER;
	xcb_atom_t _NET_WM_FULLSCREEN_MONITORS;
	xcb_atom_t _NET_WM_FULL_PLACEMENT;
	xcb_atom_t UTF8_STRING;
	xcb_atom_t WM_PROTOCOLS;
	xcb_atom_t MANAGER;
	xcb_atom_t _NET_WM_WINDOW_TYPE_DESKTOP;
	xcb_atom_t _NET_WM_WINDOW_TYPE_DOCK;
	xcb_atom_t _NET_WM_WINDOW_TYPE_TOOLBAR;
	xcb_atom_t _NET_WM_WINDOW_TYPE_MENU;
	xcb_atom_t _NET_WM_WINDOW_TYPE_UTILITY;
	xcb_atom_t _NET_WM_WINDOW_TYPE_SPLASH;
	xcb_atom_t _NET_WM_WINDOW_TYPE_DIALOG;
	xcb_atom_t _NET_WM_WINDOW_TYPE_DROPDOWN_MENU;
	xcb_atom_t _NET_WM_WINDOW_TYPE_POPUP_MENU;
	xcb_atom_t _NET_WM_WINDOW_TYPE_TOOLTIP;
	xcb_atom_t _NET_WM_WINDOW_TYPE_NOTIFICATION;
	xcb_atom_t _NET_WM_WINDOW_TYPE_COMBO;
	xcb_atom_t _NET_WM_WINDOW_TYPE_DND;
	xcb_atom_t _NET_WM_WINDOW_TYPE_NORMAL;
	xcb_atom_t _NET_WM_STATE_MODAL;
	xcb_atom_t _NET_WM_STATE_STICKY;
	xcb_atom_t _NET_WM_STATE_MAXIMIZED_VERT;
	xcb_atom_t _NET_WM_STATE_MAXIMIZED_HORZ;
	xcb_atom_t _NET_WM_STATE_SHADED;
	xcb_atom_t _NET_WM_STATE_SKIP_TASKBAR;
	xcb_atom_t _NET_WM_STATE_SKIP_PAGER;
	xcb_atom_t _NET_WM_STATE_HIDDEN;
	xcb_atom_t _NET_WM_STATE_FULLSCREEN;
	xcb_atom_t _NET_WM_STATE_ABOVE;
	xcb_atom_t _NET_WM_STATE_BELOW;
	xcb_atom_t _NET_WM_STATE_DEMANDS_ATTENTION;
	xcb_atom_t _NET_WM_ACTION_MOVE;
	xcb_atom_t _NET_WM_ACTION_RESIZE;
	xcb_atom_t _NET_WM_ACTION_MINIMIZE;
	xcb_atom_t _NET_WM_ACTION_SHADE;
	xcb_atom_t _NET_WM_ACTION_STICK;
	xcb_atom_t _NET_WM_ACTION_MAXIMIZE_HORZ;
	xcb_atom_t _NET_WM_ACTION_MAXIMIZE_VERT;
	xcb_atom_t _NET_WM_ACTION_FULLSCREEN;
	xcb_atom_t _NET_WM_ACTION_CHANGE_DESKTOP;
	xcb_atom_t _NET_WM_ACTION_CLOSE;
	xcb_atom_t _NET_WM_ACTION_ABOVE;
	xcb_atom_t _NET_WM_ACTION_BELOW;
} xcb_ewmh_connection_t;

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

#endif
