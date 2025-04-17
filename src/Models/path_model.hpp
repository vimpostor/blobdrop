#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include <vector>

#include "path.hpp"

class PathModel : public QAbstractListModel {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(QString foldedUriList MEMBER folded_uri_list NOTIFY foldedUriListChanged)
	Q_PROPERTY(int multiSelected MEMBER multiselected NOTIFY foldedUriListChanged)
public:
	explicit PathModel(QObject *parent = nullptr);

	virtual int rowCount(const QModelIndex &) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual QHash<int, QByteArray> roleNames() const override;

	Q_INVOKABLE void taint_used(int i);
	Q_INVOKABLE void taint_all_used();
	Q_INVOKABLE void multiselect(int i);
	Q_INVOKABLE void refresh_folded_paths();
	Q_INVOKABLE void open(int i) const;
	Q_INVOKABLE void finish_init();
	void add_path(Path p);
signals:
	void foldedUriListChanged();
private:
	enum RoleNames {
		PathRole = Qt::UserRole,
		UriRole,
		PrettyRole,
		UsedRole,
		MultiselectRole,
		IconRole,
		ThumbnailRole,
		ExistsRole,
	};
	QHash<int, QByteArray> role_names {{PathRole, "path"}, {UriRole, "uri"}, {PrettyRole, "pretty"}, {UsedRole, "used"}, {MultiselectRole, "multiselect"}, {IconRole, "iconName"}, {ThumbnailRole, "thumbnail"}, {ExistsRole, "exists"}};
	std::vector<Path> paths;
	QString folded_uri_list;
	int multiselected = 0;
	void check_should_quit();
};
