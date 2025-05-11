#include <QApplication>
#include "winMain.h"
#include "log.h"

// int main(int argc, char *argv[])
// {
//     LogTr("Enter main function.");
//
//     erInitLog("./log/test.log");
//
//     QApplication app(argc, argv);
//
//     winMain w;
//     w.show();
//
//     return app.exec();
// }

#include "doip.h"

int main(int argc, char *argv[])
{
    clDoipSer cDoipSer;

    cDoipSer.erLsn();
}
