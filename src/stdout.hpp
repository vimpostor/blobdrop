#pragma once

#include <iostream>
#include <qqml.h>

#include "settings.hpp"
#include "path_registry.hpp"

class Stdout : public QObject {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON
public:
	Q_INVOKABLE void print_urls(const QList<QUrl>& urls);
};
