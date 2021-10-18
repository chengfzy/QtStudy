#include "Person.h"

Person::Person(QObject* parent) : QObject(parent), age_(0) {}

QString Person::name() const { return name_; }

int Person::age() const { return age_; }

void Person::setName(const QString& name) { name_ = name; }

void Person::setAge(int age) { age_ = age; }
