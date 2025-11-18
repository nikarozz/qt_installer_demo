#include <QApplication>
#include <QProcess>
#include <QStringList>
#include <QDebug>

#include <unistd.h>

#include "main_window.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    qRegisterMetaType<PackageDescriptor>("PackageDescriptor");

    MainWindow w;
    w.show();

    return app.exec();
}
