#include "getopts.hpp"
#include "settings.hpp"

#include <ranges>

#include "remote.hpp"

namespace Getopts {

QStringList setup_args(int argc, char *argv[]) {
	const auto *env = std::getenv("BLOBDROP_ARGS");
	return quartz::getopts::prepend_args(argc, argv, env);
}

bool parse(const QStringList &args) {
	QCommandLineParser p;
	p.setApplicationDescription("Quickly drag and drop files from the terminal to applications.");
	p.addHelpOption();
	p.addVersionOption();

	const auto make_descr = [](const std::ranges::range auto &opts) { return std::ranges::fold_left(opts, std::string(), [](const auto &l, const auto &r) { return l + " " + r; }); };
	// must be in the same order as the enum
	constexpr std::array frontend_opts = {"auto", "gui", "immediate", "notify", "clipboard", "stdout"};
	const std::string frontends_descr = make_descr(frontend_opts);
	constexpr std::array auto_quit_opts = {"never", "first", "all"};
	const std::string auto_quit_descr = make_descr(auto_quit_opts);

	QCommandLineOption frameless_opt(QStringList() << "b"
												   << "frameless",
		"Show a frameless window.");
	QCommandLineOption cursor_opt(QStringList() << "c"
												<< "cursor",
		"Spawn window at the mouse cursor.");
	QCommandLineOption frontend_opt(QStringList() << "f"
												  << "frontend",
		"Selects the frontend. Must be one of:" + QString::fromStdString(frontends_descr) + " (default auto).",
		"frontend");
	QCommandLineOption intercept_opt(QStringList() << "i"
												   << "intercept",
		"Intercept another drag and drop.");
	QCommandLineOption icononly_opt(QStringList() << "I"
												  << "icon-only",
		"Only show icons.");
	QCommandLineOption keep_opt(QStringList() << "k"
											  << "keep",
		"Keep dropped files around in sink mode.");
	QCommandLineOption persistent_opt(QStringList() << "p"
													<< "persistent",
		"Do not auto-hide the window while dragging.");
	QCommandLineOption prefix_opt(QStringList() << "P"
												<< "prefix",
		"Specify a remote prefix.",
		"prefix");
	QCommandLineOption remote_opt(QStringList() << "R"
												<< "remote",
		"Enable ssh remote transparency.");
	QCommandLineOption thumbnailsize_opt(QStringList() << "s"
													   << "thumb-size",
		"Set thumbnail size (default 64).",
		"size");
	QCommandLineOption ontop_opt(QStringList() << "t"
											   << "ontop",
		"Keep the window on top of other windows.");
	QCommandLineOption auto_quit_opt(QStringList() << "x"
												   << "auto-quit",
		"The amount of drags after which the program should automatically close. Must be one of:" + QString::fromStdString(auto_quit_descr) + " (default all).",
		"behaviour");

	p.addOptions({frameless_opt, cursor_opt, frontend_opt, intercept_opt, icononly_opt, keep_opt, persistent_opt, prefix_opt, remote_opt, thumbnailsize_opt, ontop_opt, auto_quit_opt});
	p.process(args);

	if (p.isSet(auto_quit_opt)) {
		const auto opt = p.value(auto_quit_opt);
		int choice = std::ranges::find(auto_quit_opts, opt.toStdString()) - auto_quit_opts.cbegin();
		if (static_cast<Settings::AutoQuitBehavior>(choice) > Settings::AutoQuitBehavior::All) {
			std::cerr << "auto-quit needs to be one of:" << auto_quit_descr << std::endl;
			return false;
		}
		Settings::get()->auto_quit_behavior = static_cast<Settings::AutoQuitBehavior>(choice);
	}
	Settings::get()->remote = p.isSet(remote_opt);
	if (p.isSet(thumbnailsize_opt)) {
		const auto v = p.value(thumbnailsize_opt).toInt();
		if (v) {
			Settings::get()->thumbnail_size = v;
		} else {
			std::cerr << "Thumbnail size must be an integer." << std::endl;
			return false;
		}
	}
	if (p.isSet(prefix_opt)) {
		if (!Settings::get()->remote) {
			std::cerr << "This option has no effect if remote support is not enabled" << std::endl;
			return false;
		}
		Remote::get()->hardcode_prefix(p.value(prefix_opt));
	}
	Settings::get()->always_on_top = p.isSet(ontop_opt);
	Settings::get()->keep_dropped_files = p.isSet(keep_opt);

	if (p.isSet(frontend_opt)) {
		// find frontend, even if only a prefix matches
		auto frontend_selection = std::views::zip(frontend_opts, std::views::iota(0UZ, frontend_opts.size() - 1)) | std::views::filter([&](const auto &i) { return std::string(std::get<0>(i)).starts_with(p.value(frontend_opt).toStdString()); });
		if (frontend_selection.empty()) {
			// match must be unique
			std::cerr << "frontend needs to be one of the following:" << frontends_descr << std::endl;
			return false;
		}
		const auto c = static_cast<Settings::Frontend>(std::get<1>(frontend_selection.front()));
		if (c == Settings::Frontend::Immediate && quartz::util::is_wayland()) {
			std::cerr << "Wayland does not have support for this frontend, as the spec requires an implicit grab for native wl_data_device::start_drag() operations, thus making it impossible to implement this workflow on Wayland." << std::endl
					  << "This frontend might work over XWayland (force it with QT_QPA_PLATFORM=xcb) but will likely be very buggy. Please switch to X11 to get the optimal usability experience or use another frontend." << std::endl;
			return false;
		}
		Settings::get()->frontend = c;
	}

	Settings::get()->suppress_always_on_bottom = p.isSet(persistent_opt);
	Settings::get()->frameless = p.isSet(frameless_opt);
	Settings::get()->spawn_on_cursor = p.isSet(cursor_opt);
	Settings::get()->intercept = p.isSet(intercept_opt);
	Settings::get()->icon_only = p.isSet(icononly_opt);

	// add all trailing arguments to the path list
	std::ranges::for_each(p.positionalArguments(), [](auto i) { PathRegistry::get()->add_path(i.toStdString()); });
	return true;
}
}
