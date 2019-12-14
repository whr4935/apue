#include <QtWidgets/QApplication>
#include "qtlearn.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QtLearn learn;
	learn.show();

	return a.exec();
}
