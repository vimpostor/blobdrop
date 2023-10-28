#include "path_model.hpp"

#include <QDrag>
#include <QIcon>
#include <QMimeData>
#include <QPixmap>

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
	case PrettyRole:
		return QString::fromStdString(p.pretty_print());
	case UsedRole:
		return p.used;
	case IconRole:
		return QString::fromStdString(p.iconName);
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

void PathModel::open(int i) const {
	if (!paths[i].open()) {
		qDebug() << "Failed to open path" << paths[i].get_uri();
	};
}

void PathModel::finish_init() {
	const auto f = Settings::get()->effective_frontend();
	if (f == Settings::Frontend::Immediate) {
		drag_immediately();
	} else if (f == Settings::Frontend::Stdout) {
		print_hyperlinks();
	} else if (f == Settings::Frontend::Notification) {
		send_notification();
	}

	if (f == Settings::Frontend::Stdout || f == Settings::Frontend::Notification) {
		taint_all_used();
	}
}

void PathModel::add_path(Path p) {
	beginInsertRows(QModelIndex(), paths.size(), paths.size());
	paths.emplace_back(p);
	endInsertRows();
}

void PathModel::drag_immediately() {
	Backend::get()->hide_terminal();

	/**
	 * Qt takes ownership both over the QDrag as well as the QMimeData
	 * So no, this is not a memory leak.
	 */
	auto drag = new QDrag(this);
	auto mimedata = new QMimeData();

	QList<QUrl> urls;
	for (auto &i : paths) {
		urls.push_back(i.get_url());
		i.used = true;
	}

	mimedata->setUrls(urls);
	drag->setMimeData(mimedata);

	constexpr const int cursor_size = 24;
	QPixmap pixmap;
	if (paths.size() == 1) {
		const auto p = paths.front();
		if (!p.thumbnail.isEmpty()) {
			// try using the thumbnail first
			constexpr const int max_size = 128;
			pixmap = QPixmap(p.thumbnail.toLocalFile());
			if (std::max(pixmap.width(), pixmap.height()) > max_size) {
				pixmap = pixmap.scaled(QSize(max_size, max_size), Qt::KeepAspectRatio, Qt::SmoothTransformation);
			}
		}
		if (pixmap.isNull()) {
			// fallback to mime type icon
			pixmap = QIcon::fromTheme(QString::fromStdString(p.iconName)).pixmap(cursor_size);
		}
	} else {
		// show a collective pseudo thumbnail of all files
		pixmap = QIcon::fromTheme("emblem-documents").pixmap(cursor_size);
	}
	if (!pixmap.isNull()) {
		drag->setPixmap(pixmap);
	}

	// The object is destroyed by Qt as soon as the drag is finished
	connect(drag, &QObject::destroyed, this, [this]() {
		Backend::get()->restore_terminal();
		check_should_quit();
	});

	drag->exec();
}

void PathModel::print_hyperlinks() {
	for (auto &i : paths) {
		std::cout << Util::print_osc8_link(i.get_uri(), i.pretty_print()) << std::endl;
	}
}

void PathModel::send_notification() {
	refresh_folded_paths();
	const auto uri_list = folded_uri_list.split(QChar::LineFeed, Qt::SkipEmptyParts);
	Backend::get()->send_drag_notification(uri_list);
}

void PathModel::check_should_quit() {
	const auto quit = Settings::get()->auto_quit_behavior;
	if (quit == Settings::AutoQuitBehavior::First || (quit == Settings::AutoQuitBehavior::All && std::ranges::all_of(paths, [](auto p) { return p.used; }))) {
		Backend::get()->quit_delayed();
	}
}
