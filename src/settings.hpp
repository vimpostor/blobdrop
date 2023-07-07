#pragma once

#include <qqml.h>

#include "Util/util.hpp"

class Settings : public QObject {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(bool alwaysOnTop MEMBER always_on_top NOTIFY alwaysOnTopChanged)
	Q_PROPERTY(bool alwaysOnBottom MEMBER always_on_bottom WRITE setAlwaysOnBottom NOTIFY alwaysOnBottomChanged)
	Q_PROPERTY(bool keepDroppedFiles MEMBER keep_dropped_files NOTIFY keepDroppedFilesChanged)
public:
	enum class AutoQuitBehavior {
		Never,
		First,
		All,
	};

	STRICT_SINGLETON(Settings)

	static Settings *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
	AutoQuitBehavior auto_quit_behavior = Settings::AutoQuitBehavior::All;
	void prepare_quit();
	void setAlwaysOnBottom(const bool v);

	bool always_on_top = false;
	bool always_on_bottom = false;
	bool keep_dropped_files = false;
signals:
	void alwaysOnTopChanged(bool alwaysOnTop);
	void alwaysOnBottomChanged(bool alwaysOnBottom);
	void keepDroppedFilesChanged(bool keepDroppedFiles);
private:
	bool is_about_to_quit = false;
};
