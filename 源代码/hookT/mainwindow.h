#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<windows.h>
#include<winuser.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void installHook();   //安装钩子函数，指针参数为了发送信号用
    void unInstallHook();        //删除钩子
    void doPressAct();    //执行重放

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
