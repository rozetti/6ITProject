#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "subc_worker.h"

#include <QDir>
#include <QThread>
#include <QObject>
#include <QFile>
#include <QtWidgets>
#include <QScroller>

extern "C"
{
#include "6IT.h"
#include "../../SubC_tester.h"
}

Ui::MainWindow *_ui;

void MainWindow::printString(QString const &s)
{
    qDebug(s.toStdString().c_str());

    auto text = _ui->lblConsoleOutput->text();

//    if (text.length() > 2000)
//    {
//        text = text.right(1000);
//    }

    text.append(s);
    _ui->lblConsoleOutput->setText(text);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _ui = ui;

    ui->setupUi(this);

    ui->scrollAreaWidgetContents->setStyleSheet("background-color : black; color: white;");
    QObject::connect(ui->scrollArea, SIGNAL(userInteracted()), this, SLOT(userInteracted()), Qt::QueuedConnection);

    ui->scrollArea->allowHorizontalScrolling(false);

    auto cwd = QDir::currentPath();

    construct_6IOS(&_Bios, 0);
    //_Bios.print_string = print_string;

    printString(cwd);

    //print_string(QSysInfo::productType().toStdString().c_str());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnRun_clicked()
{
    ui->btnRun->setEnabled(false);

    ui->lblConsoleOutput->setText("");
    ui->scrollArea->clampToBottom(true);

    auto thread = new QThread(this);
    auto worker = new SubCWorker();

    worker->moveToThread(thread);

    QObject::connect(thread, SIGNAL(started()), worker, SLOT(run()));
    QObject::connect(worker, SIGNAL(finished()), thread, SLOT(quit()));

    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    QObject::connect(worker, SIGNAL(finished()), this, SLOT(runFinished()));
    QObject::connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

    QObject::connect(worker, SIGNAL(printString(QString const &)), this, SLOT(printString(QString const &)), Qt::QueuedConnection);
    thread->start();
}

void MainWindow::userInteracted()
{
    ui->scrollArea->clampToBottom(false);
}

void MainWindow::runFinished()
{
    ui->btnRun->setEnabled(true);
}
