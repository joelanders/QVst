#ifndef QVSTAPPLICATION_H
#define QVSTAPPLICATION_H

#include <QApplication>
#include <QVstConsole.h>

class QVstApplication : public QApplication
{
    Q_OBJECT
public:

    static QVstApplication* createInstance(Qt::HANDLE handle);
    static QVstApplication* instance();

    ~QVstApplication();

    QVstConsole* console() const;

private:

    struct Private;

    QVstApplication(int &argc, char **argv, Qt::HANDLE handle);

    Private *m;
};

#endif // QVSTAPPLICATION_H

