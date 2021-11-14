#pragma once

#include <qqml.h>

#include "Util/util.hpp"

class Settings : public QObject {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(bool keepDroppedFiles MEMBER keep_dropped_files NOTIFY keepDroppedFilesChanged)
public:
	enum class AutoQuitBehavior {
		Never,
		First,
		All,
	};

	STRICT_SINGLETON(Settings)

	AutoQuitBehavior auto_quit_behavior = Settings::AutoQuitBehavior::All;
	bool keep_dropped_files = false;
signals:
	void keepDroppedFilesChanged(bool keepDroppedFiles);
};
