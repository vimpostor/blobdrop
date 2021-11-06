#include "path_model.hpp"

PathModel::PathModel(QObject* parent) {
	paths = PathRegistry::get()->paths;
	connect(PathRegistry::get(), &PathRegistry::pathAdded, this, &PathModel::add_path);
}

int PathModel::rowCount(const QModelIndex&) const {
	return paths.size();
}

QVariant PathModel::data(const QModelIndex& index, int role) const {
	const auto& p = paths[index.row()];
	switch (role) {
		case PathRole: return QString::fromStdString(p.path);
		case UriRole: return "file://" + QString::fromStdString(p.path);
		case UsedRole: return p.used;
		default: return QVariant();
	};
}

QHash<int, QByteArray> PathModel::roleNames() const {
	return role_names;
}

void PathModel::taint_used(int i) {
	paths[i].used = true;
	emit dataChanged(index(i, 0), index(i, 0));
}

void PathModel::add_path(Path p) {
	beginInsertRows(QModelIndex(), paths.size(), paths.size() + 1);
	paths.emplace_back(p);
	endInsertRows();
}
