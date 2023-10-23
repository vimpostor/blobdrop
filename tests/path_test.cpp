#include <QTest>

#include "path.hpp"

class PathTest : public QObject {
	Q_OBJECT
private:
	std::string pwd;
private slots:
	void initTestCase() {
		pwd = std::filesystem::current_path().native();
	}

	void test_path_uris() {
		const std::string filename = "test.txt";
		const auto path = pwd + "/" + filename;
		const auto uri = "file://" + path;

		// relative path
		QCOMPARE(Path(filename).get_uri(), uri);
		// absolute path
		QCOMPARE(Path(path).get_uri(), uri);
	}
};

QTEST_MAIN(PathTest)
#include "path_test.moc"
