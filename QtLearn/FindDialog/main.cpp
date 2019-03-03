#include "FindDialog.h"
#include "SortDialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    FindDialog w;
    SortDialog w;
    w.setColumnRange('A', 'F');
    w.show();

    return a.exec();
}
