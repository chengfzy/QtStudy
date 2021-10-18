#include <QtQml/qqml.h>
#include <QObject>

class Person : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int age READ age WRITE setAge)
    QML_ELEMENT

  public:
    Person(QObject* parent = nullptr);

    QString name() const;
    int age() const;

    void setName(const QString& name);
    void setAge(int age);

  private:
    QString name_;
    int age_;
};