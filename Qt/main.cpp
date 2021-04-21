#include "comparatore.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Comparatore w;

    w.setMinimumSize(700, 500);
    w.setGeometry(200, 200, 700, 500);
    //w.resize(700, 500);


    w.show();
    return a.exec();
}
