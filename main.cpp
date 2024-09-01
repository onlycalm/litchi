#include <QApplication>
#include <QMessageBox>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMessageBox::about(nullptr, "Title", "Hello World!");

    return app.exec();
}
