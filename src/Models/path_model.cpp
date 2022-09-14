#include "path_model.hpp"
#include <bits/ranges_algo.h>

PathModel::PathModel(QObject *parent) {
	paths = PathRegistry::get()->paths;
	connect(PathRegistry::get(), &PathRegistry::pathAdded, this, &PathModel::add_path);
}

int PathModel::rowCount(const QModelIndex &) const {
	return paths.size();
}

QVariant PathModel::data(const QModelIndex &index, int role) const {
	const auto &p = paths[index.row()];
	switch (role) {
	case PathRole:
		return QString::fromStdString(p.path);
	case UriRole:
		return QString::fromStdString(p.get_uri());
	case UsedRole:
		return p.used;
	case ThumbnailRole:
		return p.thumbnail;
	case ExistsRole:
		return p.exists;
	default:
		return QVariant();
	};
}

QHash<int, QByteArray> PathModel::roleNames() const {
	return role_names;
}

void PathModel::taint_used(int i) {
	paths[i].used = true;
	emit dataChanged(index(i, 0), index(i, 0));
	check_should_quit();
}

void PathModel::taint_all_used() {
	std::ranges::for_each(paths, [&](auto &p) { p.used = true; });
	emit dataChanged(index(0, 0), index(paths.size() - 1, 0));
	check_should_quit();
}

void PathModel::refresh_folded_paths() {
	folded_uri_list = std::accumulate(paths.cbegin(), paths.cend(), QString(), [&](QString s, const auto p) { return s.append(QString::fromStdString(p.get_uri()) + "\n"); });
	emit foldedUriListChanged(folded_uri_list);
}

void PathModel::add_path(Path p) {
	beginInsertRows(QModelIndex(), paths.size(), paths.size());
	paths.emplace_back(p);
	endInsertRows();
}

void PathModel::check_should_quit() {
	const auto quit = Settings::get()->auto_quit_behavior;
	if (quit == Settings::AutoQuitBehavior::First || (quit == Settings::AutoQuitBehavior::All && std::ranges::all_of(paths, [](auto p) { return p.used; }))) {
		Backend::get()->quit_delayed();
	}
}
