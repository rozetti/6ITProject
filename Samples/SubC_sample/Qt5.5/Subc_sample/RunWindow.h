#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SubCWorker;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void printString(QString const &s);

private slots:
    void on_btnRun_clicked();
    void userInteracted();
    void runFinished();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
