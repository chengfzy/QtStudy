#include "MyGLWidget.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // set OpenGL version information
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);

    // set window up
    MyGLWidget window;
    window.setFormat(format);
    window.resize(800, 600);
    window.show();

    return app.exec();
}