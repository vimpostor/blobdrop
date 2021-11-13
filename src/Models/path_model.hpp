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

	Q_PROPERTY(QString foldedUriList MEMBER folded_uri_list NOTIFY foldedUriListChanged)
public:
	PathModel(QObject* parent = nullptr);

	virtual int rowCount(const QModelIndex&) const override;
	virtual QVariant data(const QModelIndex& index, int role) const override;
	virtual QHash<int, QByteArray> roleNames() const override;

	Q_INVOKABLE void taint_used(int i);
	Q_INVOKABLE void taint_all_used();
	Q_INVOKABLE void refresh_folded_paths();
	void add_path(Path p);
signals:
	void foldedUriListChanged(QString foldedUriList);
private:
	enum RoleNames {
		PathRole = Qt::UserRole,
		UriRole,
		UsedRole,
		ThumbnailRole,
		ExistsRole,
	};
	QHash<int, QByteArray> role_names {{PathRole, "path"}, {UriRole, "uri"}, {UsedRole, "used"}, {ThumbnailRole, "thumbnail"}, {ExistsRole, "exists"}};
	std::vector<Path> paths;
	QString folded_uri_list;
	void check_should_quit();
};
