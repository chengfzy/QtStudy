#include "Window.h"
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include "Widget.h"

using namespace std;

Window::Window() {
    setWindowTitle("2D Painting on Native and OpenGL Widgets");

    helper_ = make_shared<Helper>();

    auto native = new Widget(helper_, this);
    auto nativeLabel = new QLabel("Native");

    auto layout = new QGridLayout;
    layout->addWidget(native, 0, 0);
    layout->addWidget(nativeLabel, 1, 0);
    setLayout(layout);

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, native, &Widget::animate);
    timer->start(50);
}
