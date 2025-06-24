#include <iostream>
#include <QApplication>
#include "app_initializer.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    try {
        const std::unique_ptr<AppMainWindow> appMainWindow = AppInitializer::initialize(argc, argv);
        appMainWindow->showMaximized();
        return QApplication::exec();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
