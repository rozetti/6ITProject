#ifndef WORKER_H
#define WORKER_H

#include <QThread>

class QThread;

class SubCWorker : public QObject
{
    Q_OBJECT

public:
    SubCWorker();
    ~SubCWorker();
    void print_string(char const *s);

public slots:
    void run();

signals:
    void finished();
    void printString(QString const &s);

private:
};

#endif // WORKER_H
