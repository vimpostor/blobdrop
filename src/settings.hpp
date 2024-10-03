#pragma once

#include "Util/util.hpp"

class Settings : public QObject {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(bool alwaysOnTop MEMBER always_on_top CONSTANT)
	Q_PROPERTY(bool alwaysOnBottom MEMBER always_on_bottom WRITE setAlwaysOnBottom NOTIFY alwaysOnBottomChanged)
	Q_PROPERTY(bool keepDroppedFiles MEMBER keep_dropped_files NOTIFY keepDroppedFilesChanged)
	Q_PROPERTY(bool frameless MEMBER frameless CONSTANT)
	Q_PROPERTY(bool needsGui READ needs_gui NOTIFY hideGuiChanged)
	Q_PROPERTY(bool spawnOnCursor MEMBER spawn_on_cursor CONSTANT)
	Q_PROPERTY(bool intercept MEMBER intercept CONSTANT)
	Q_PROPERTY(int thumbnailSize MEMBER thumbnail_size CONSTANT)
	Q_PROPERTY(bool iconOnly MEMBER icon_only CONSTANT)
public:
	enum class AutoQuitBehavior {
		Never,
		First,
		All,
	};
	enum class Frontend {
		Auto, // choose automatically
		Gui, // show a window to drag files from
		Immediate, // perform drag immediately without needing to hold down the mouse
		Notification, // show a desktop notification to drag from
		Clipboard, // copy URI to clipboard
		Stdout, // print a link in the terminal using OSC8
	};

	QML_CPP_SINGLETON(Settings)

	AutoQuitBehavior auto_quit_behavior = Settings::AutoQuitBehavior::All;
	Frontend frontend = Settings::Frontend::Auto;
	void setAlwaysOnBottom(const bool v);
	Frontend effective_frontend(bool outgoing = false) const;
	void setHideGui(const bool h);

	bool always_on_top = false;
	bool always_on_bottom = false;
	bool keep_dropped_files = false;
	bool frameless = false;
	bool can_drag_immediately = false;
	bool suppress_always_on_bottom = false;
	bool spawn_on_cursor = false;
	bool intercept = false;
	int thumbnail_size = 64;
	bool icon_only = false;
	bool remote = false;
signals:
	void alwaysOnBottomChanged(bool alwaysOnBottom);
	void keepDroppedFilesChanged(bool keepDroppedFiles);
	void hideGuiChanged(bool hide);
private:
	bool needs_gui() const;
	bool hide_gui = false;
};
