#pragma once

#include <qqml.h>

#include "Util/util.hpp"

class Settings : public QObject {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(bool alwaysOnTop MEMBER always_on_top CONSTANT)
	Q_PROPERTY(bool alwaysOnBottom MEMBER always_on_bottom WRITE setAlwaysOnBottom NOTIFY alwaysOnBottomChanged)
	Q_PROPERTY(bool keepDroppedFiles MEMBER keep_dropped_files NOTIFY keepDroppedFilesChanged)
	Q_PROPERTY(bool frameless MEMBER frameless CONSTANT)
	Q_PROPERTY(bool needsGui READ needs_gui CONSTANT)
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
		Stdout, // print a link in the terminal using OSC8
	};

	STRICT_SINGLETON(Settings)

	static Settings *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
	AutoQuitBehavior auto_quit_behavior = Settings::AutoQuitBehavior::All;
	Frontend frontend = Settings::Frontend::Auto;
	void disable_always_on_bottom();
	void setAlwaysOnBottom(const bool v);
	Frontend effective_frontend() const;

	bool always_on_top = false;
	bool always_on_bottom = false;
	bool keep_dropped_files = false;
	bool frameless = false;
	bool can_drag_immediately = false;
	bool suppress_always_on_bottom = false;
signals:
	void alwaysOnBottomChanged(bool alwaysOnBottom);
	void keepDroppedFilesChanged(bool keepDroppedFiles);
private:
	bool needs_gui() const;
};
