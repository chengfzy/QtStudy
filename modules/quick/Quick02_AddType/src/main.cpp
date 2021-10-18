#include <QCoreApplication>
#include <QQmlComponent>
#include <QQmlEngine>
#include "Person.h"

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl("qrc:Person.qml"));
    auto person = qobject_cast<Person*>(component.create());
    if (person) {
        qWarning() << "Person.name = " << person->name();
        qWarning() << "Person.age = " << person->age();
    } else {
        qWarning() << "error: " << component.errors();
    }

    return 0;
}