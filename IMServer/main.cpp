#include "imserverdialog.h"
#include <QApplication>
#include"ckernel.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    IMServerDialog w;
//    w.show();
    CKernel kernel;

    return a.exec();
}
