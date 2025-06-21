#include <iostream>
#include <QApplication>
#include "app_initializer.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    try {
        const std::unique_ptr<MainWindow> mainWindow = AppInitializer::initialize(argc, argv);
        mainWindow->showMaximized();
        return a.exec();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
