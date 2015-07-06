#include "subc_worker.h"

#include <QThread>
#include <QCoreApplication>

extern "C"
{
#include "6IT.h"
#include "../../SubC_tester.h"
}

static SubCWorker *_worker;

static void _print_string(char const *s)
{
    _worker->print_string(s);
}

SubCWorker::SubCWorker()
{
    _worker = this;
}

SubCWorker::~SubCWorker()
{
}

void SubCWorker::print_string(const char *s)
{
    emit printString(QString(s));

    QCoreApplication::processEvents();
    //QThread::msleep(10);
}

void SubCWorker::run()
{
    _Bios.print_string = _print_string;

    char s[10];

    int i = 0;
    while(1)
    {
        auto rv = SubC_tester_main(0);
        sprintf(s, "%d\n", ++i);
        qDebug(s);
        print_string(s);
        break;
    }

    emit finished();
}
