#include <QApplication>

#include "Model/Model.h"
#include "View/MainWindow.h"
#include "SoundView.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Model model;
    MainWindow window(&model);
    SoundView soundView(&model);

    window.start();

    return a.exec();
}
