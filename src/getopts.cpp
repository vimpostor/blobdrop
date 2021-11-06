#include "getopts.hpp"

namespace Getopts {

void parse(QCoreApplication& app) {
	QCommandLineParser p;
	p.setApplicationDescription("Quickly drag and drop files from the terminal to applications.");
	p.addHelpOption();
	p.addVersionOption();
	p.process(app);

	const auto paths = p.positionalArguments();
	for (auto& i : paths) {
		PathRegistry::get()->add_path(i.toStdString());
	}
}

}
