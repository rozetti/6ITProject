#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void run();

private slots:
    void moveScrollBarToBottom(int min, int max);
    void on_btnRun_clicked();
    void userInteracted();
    void runFinished();

private:
    Ui::MainWindow *ui;

    bool m_autoScroll;
};

#endif // MAINWINDOW_H
