#pragma once

#include <QObject>

#include "Util/util.hpp"
#include "path.hpp"

class PathRegistry : public QObject {
	Q_OBJECT
public:
	SINGLETON(PathRegistry)
	void add_path(Path p);
	std::vector<Path> paths;
signals:
	void pathAdded(Path p);
};
