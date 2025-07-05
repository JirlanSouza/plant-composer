#include <iostream>
#include <QApplication>

#include "plant_composer_application.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    try {
        const auto plantComposerApplication = new PlantComposerApplication();
        plantComposerApplication->initialize(argc, argv);
        plantComposerApplication->getMainWindow()->showMaximized();
        return QApplication::exec();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
