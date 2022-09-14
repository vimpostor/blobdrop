#pragma once

#include <QSocketNotifier>
#include <iostream>
#include <qqml.h>

#include "path_registry.hpp"

class Stdin : public QObject {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON
	Q_PROPERTY(bool closed MEMBER m_closed NOTIFY closedChanged)
public:
	Stdin(QObject *parent = nullptr);
signals:
	void closedChanged();
private:
	void read();
	bool m_closed = true;
	void setClosed(bool closed);
	std::unique_ptr<QSocketNotifier> socket;
};
