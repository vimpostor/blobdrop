#include "getopts.hpp"

namespace Getopts {

void parse(QCoreApplication& app) {
	QCommandLineParser p;
	p.setApplicationDescription("Quickly drag and drop files from the terminal to applications.");
	p.addHelpOption();
	p.addVersionOption();
	p.process(app);

	std::ranges::for_each(p.positionalArguments(), [](auto i){ PathRegistry::get()->add_path(i.toStdString()); });
}

}
