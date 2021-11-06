#include "path_model.hpp"

PathModel::PathModel(QObject* parent) {
	paths = PathRegistry::get()->paths;
	connect(PathRegistry::get(), &PathRegistry::pathAdded, this, &PathModel::add_path);
}

int PathModel::rowCount(const QModelIndex&) const
{
	return paths.size();
}

QVariant PathModel::data(const QModelIndex& index, int role) const
{
	const auto& p = paths[index.row()];
	switch (role) {
	case PathRole: return QString::fromStdString(p.path);
	default: return QVariant();
	};
}

QHash<int, QByteArray> PathModel::roleNames() const
{
	return role_names;
}

void PathModel::add_path(Path p)
{
	beginInsertRows(QModelIndex(), paths.size(), paths.size() + 1);
	paths.emplace_back(p);
	endInsertRows();
}
