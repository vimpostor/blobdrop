#pragma once

#include <qqml.h>

#include "Util/util.hpp"

class Settings : public QObject {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(bool alwaysOnTop MEMBER always_on_top NOTIFY alwaysOnTopChanged)
	Q_PROPERTY(bool keepDroppedFiles MEMBER keep_dropped_files NOTIFY keepDroppedFilesChanged)
public:
	enum class AutoQuitBehavior {
		Never,
		First,
		All,
	};

	STRICT_SINGLETON(Settings)

	AutoQuitBehavior auto_quit_behavior = Settings::AutoQuitBehavior::All;
	bool always_on_top = false;
	bool keep_dropped_files = false;
signals:
	void alwaysOnTopChanged(bool alwaysOnTop);
	void keepDroppedFilesChanged(bool keepDroppedFiles);
};
