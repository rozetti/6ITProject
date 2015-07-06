#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "worker.h"

#include <QDir>
#include <QThread>
#include <QObject>
#include <QFile>
#include <QtWidgets>

extern "C"
{
#include "6IT.h"
#include "../../SubC_tester.h"
}

Ui::MainWindow *_ui;

static void print_string(char const *s)
{
	qDebug(s);

    /*
    auto rect =_ui->lblConsoleOutput->frameRect();
    auto h = rect.height();
    auto line_height = _ui->lblConsoleOutput->fontInfo().pixelSize();
    auto number_of_lines = (h / line_height) - 1;

	if (number_of_lines < 1) return;
*/

    auto text = _ui->lblConsoleOutput->text();
    text.append(s);
/*
    auto lines = text.split('\n');

    while(lines.count() > number_of_lines)
    {
        lines.removeFirst();
    }

    text.clear();
    for(int i = 0; i < lines.count(); ++i)
    {
        if (lines[i].length() > 0)
        {
            text.append(lines[i].trimmed());
            text.append('\n');
        }
    }
*/
    _ui->lblConsoleOutput->setText(text);
}

void Worker::process()
{
    construct_6IOS(&_Bios, 0);
    _Bios.print_string = print_string;

    SubC_tester_main(0);

    emit finished();
}

void MainWindow::moveScrollBarToBottom(int min, int max)
{
    if (!m_autoScroll) return;

    Q_UNUSED(min);
    ui->scrollArea->verticalScrollBar()->setValue(max);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_autoScroll(true)
{
    _ui = ui;

    ui->setupUi(this);

    ui->scrollAreaWidgetContents->setStyleSheet("background-color : black; color: white;");
    QScrollBar* scrollbar = ui->scrollArea->verticalScrollBar();
    QObject::connect(scrollbar, SIGNAL(rangeChanged(int,int)), this, SLOT(moveScrollBarToBottom(int, int)));
    QObject::connect(ui->scrollArea, SIGNAL(userInteracted()), this, SLOT(userInteracted()));

    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

    auto cwd = QDir::currentPath();
	print_string(cwd.toStdString().c_str());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::run()
{
    ui->btnRun->setEnabled(false);

    QThread* thread = new QThread;
    Worker* worker = new Worker();
    worker->moveToThread(thread);

    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), this, SLOT(runFinished()));

    thread->start();
}


void MainWindow::on_btnRun_clicked()
{
    m_autoScroll = true;
    ui->lblConsoleOutput->setText("");
    run();
}

void MainWindow::userInteracted()
{
    m_autoScroll = false;
}

void MainWindow::runFinished()
{
    ui->btnRun->setEnabled(true);
}
