#pragma once

#include <vector>
#include <qqml.h>
#include <QAbstractListModel>

#include "path.hpp"
#include "path_registry.hpp"
#include "settings.hpp"
#include "backend.hpp"

class PathModel : public QAbstractListModel {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON
public:
	PathModel(QObject* parent = nullptr);

	virtual int rowCount(const QModelIndex&) const override;
	virtual QVariant data(const QModelIndex& index, int role) const override;
	virtual QHash<int, QByteArray> roleNames() const override;

	Q_INVOKABLE void taint_used(int i);
	void add_path(Path p);
private:
	enum RoleNames {
		PathRole = Qt::UserRole,
		UriRole,
		UsedRole,
		ThumbnailRole,
	};
	QHash<int, QByteArray> role_names {{PathRole, "path"}, {UriRole, "uri"}, {UsedRole, "used"}, {ThumbnailRole, "thumbnail"}};
	std::vector<Path> paths;
};
