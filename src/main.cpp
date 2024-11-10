#include <QApplication>
#include "winMain.h"
#include "log.h"

int main(int argc, char *argv[])
{
    LogTr("Enter main function.");

    QApplication app(argc, argv);

    winMain w;
    w.show();

    return app.exec();
}
