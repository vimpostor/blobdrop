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
	Q_PROPERTY(bool printHyperlinks MEMBER print_hyperlinks CONSTANT)
	Q_PROPERTY(bool frameless MEMBER frameless CONSTANT)
	Q_PROPERTY(bool sendNotification MEMBER send_notification CONSTANT)
	Q_PROPERTY(bool immediateDrag MEMBER immediate_drag CONSTANT)
public:
	enum class AutoQuitBehavior {
		Never,
		First,
		All,
	};

	STRICT_SINGLETON(Settings)

	static Settings *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
	AutoQuitBehavior auto_quit_behavior = Settings::AutoQuitBehavior::All;
	void disable_always_on_bottom();
	void setAlwaysOnBottom(const bool v);

	bool always_on_top = false;
	bool always_on_bottom = false;
	bool keep_dropped_files = false;
	bool print_hyperlinks = false;
	bool frameless = false;
	bool send_notification = false;
	bool immediate_drag = false;
signals:
	void alwaysOnBottomChanged(bool alwaysOnBottom);
	void keepDroppedFilesChanged(bool keepDroppedFiles);
private:
	bool supress_always_on_bottom = false;
};
