#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<tchar.h>
#include<QDebug>
#include<QFile>
#include<QFileDialog>
#include<QMessageBox>
#include<QMouseEvent>
#include<QCursor>
#include<QPoint>
#include<QTime>
#include<QCoreApplication>


static HHOOK keyHook = nullptr,mouseHook = nullptr;     //钩子对象
static QString filename = "";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //退出系统
    connect(ui->pB_exit,&QPushButton::clicked,this,[=](){
        exit(0);
    });

    //保存，选择将录制的数据放在哪个文件
    connect(ui->pB_save,&QPushButton::clicked,this,[=](){
        QString curPath=QDir::currentPath();//获取系统当前目录
        QString dlgTitle="请选择存放文件的位置"; //对话框标题
        QString filter="文本文件(*.txt)"; //文件过滤器
         QString aFileName=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
        if (aFileName.isEmpty())
        {
            QMessageBox::warning(this,"警告","保存文件路径为空！");
            return ;
        }
        //清空文件
        filename = aFileName;
        QFile tempFile(filename);
        tempFile.open(QIODevice::WriteOnly);
        tempFile.close();
        ui->lineEdit->setText(filename);
        if(filename == NULL)
        {
            QMessageBox::warning(this,"警告","记录文件路径为空！！！");
        }
    });

    //录制
    connect(ui->pB_transcribe,&QPushButton::clicked,this,[=](){
        this->installHook();

    });

    //停止
    connect(ui->pB_stop,&QPushButton::clicked,this,[=](){
        this->unInstallHook();
    });

    //重放
    connect(ui->pB_reset,&QPushButton::clicked,this,[=](){
        this->doPressAct();
    });
}

//键盘钩子处理函数
LRESULT CALLBACK keyProc(int nCode,WPARAM wParam,LPARAM lParam) //钩子消息函数，系统消息队列信息会返回到该函数中
{
    if(!keyHook)
        return CallNextHookEx(keyHook,nCode,wParam,lParam);
    if(wParam == WM_KEYDOWN)    //wParam用于判断事件类型，当前为按键按下事件
    {
        KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;  //lParam用于判断按键类型
        qDebug()<<"key down:"<<QString::number(pkbhs->vkCode,10);
        QFile tempFile(filename);
        tempFile.open(QIODevice::Append);
        QString str = "key down:" + QString::number(pkbhs->vkCode,10) + "\n";
        QByteArray  strBytes=str.toUtf8();//转换为字节数组
        tempFile.write(strBytes,strBytes.length());  //写入文件
        tempFile.close();
        //可添加一些自定义信号发出去，如：emit hookClass->keyPress

    }
    else if(wParam == WM_KEYUP)
    {
        KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;  //lParam用于判断按键类型
        qDebug()<<"key up:"<<QString::number(pkbhs->vkCode,10);
        QFile tempFile(filename);
        tempFile.open(QIODevice::Append);
        QString str = "key up:" + QString::number(pkbhs->vkCode,10) + "\n";
        QByteArray  strBytes=str.toUtf8();//转换为字节数组
        tempFile.write(strBytes,strBytes.length());  //写入文件
        tempFile.close();
    }
    return CallNextHookEx(keyHook,nCode,wParam,lParam);     //继续原有的事件队列
}

//鼠标钩子处理函数
LRESULT CALLBACK mouseProc(int nCode,WPARAM wParam,LPARAM lParam) //钩子消息函数，系统消息队列信息会返回到该函数中
{
    if(!mouseHook)
        return CallNextHookEx(mouseHook,nCode,wParam,lParam);
    //鼠标点击右键
    if (WM_RBUTTONDOWN == wParam)
    {
        MOUSEHOOKSTRUCT *mhookstruct = (MOUSEHOOKSTRUCT*)lParam; //鼠标HOOK结构体
        POINT pt = mhookstruct->pt;         //将当前鼠标坐标点的x，y坐标作为参数向主程序窗口发送消息
        qDebug()<<QString("mouse right:(%1,%2)").arg(pt.x).arg(pt.y);
        QFile tempFile(filename);
        tempFile.open(QIODevice::Append);
        QString str = QString("mouse right:(%1,%2)").arg(pt.x).arg(pt.y) + "\n";
        QByteArray  strBytes=str.toUtf8();//转换为字节数组
        tempFile.write(strBytes,strBytes.length());  //写入文件
        tempFile.close();
        //可添加一些自定义信号发出去，如：emit hookClass->mousePress
    }
    else if(WM_LBUTTONDOWN == wParam)
    {
        MOUSEHOOKSTRUCT *mhookstruct = (MOUSEHOOKSTRUCT*)lParam; //鼠标HOOK结构体
        POINT pt = mhookstruct->pt;         //将当前鼠标坐标点的x，y坐标作为参数向主程序窗口发送消息
        qDebug()<<QString("mouse left:(%1,%2)").arg(pt.x).arg(pt.y);
        QFile tempFile(filename);
        tempFile.open(QIODevice::Append);
        QString str = QString("mouse left:(%1,%2)").arg(pt.x).arg(pt.y) + "\n";
        QByteArray  strBytes=str.toUtf8();//转换为字节数组
        tempFile.write(strBytes,strBytes.length());  //写入文件
        tempFile.close();
    }
    else if(WM_MOUSEMOVE == wParam)
    {
        MOUSEHOOKSTRUCT *mhookstruct = (MOUSEHOOKSTRUCT*)lParam; //鼠标HOOK结构体
        POINT pt = mhookstruct->pt;         //将当前鼠标坐标点的x，y坐标作为参数向主程序窗口发送消息
        qDebug()<<QString("mouse move:(%1,%2)").arg(pt.x).arg(pt.y);
        QFile tempFile(filename);
        tempFile.open(QIODevice::Append);
        QString str = QString("mouse move:(%1,%2)").arg(pt.x).arg(pt.y) + "\n";
        QByteArray strBytes=str.toUtf8();//转换为字节数组
        tempFile.write(strBytes,strBytes.length());  //写入文件
        tempFile.close();
    }
    return CallNextHookEx(mouseHook,nCode,wParam,lParam);     //继续原有的事件队列
}


//int move(int x, int y){
//    HINSTANCE hDll;
//    typedef bool (*Fun1)(int,int);
//    hDll = LoadLibrary(_T("user32.dll"));
//    if(NULL == hDll)
//  {
//      fprintf(stderr, "load dll 'user32.dll' fail.");
//    return -1;
//  }

//  Fun1 SetCursorPos = (Fun1)GetProcAddress(hDll, "SetCursorPos");
//  if(NULL == SetCursorPos)
//  {
//      fprintf(stderr, "call function 'SetCursorPos' fail.");
//    FreeLibrary(hDll);
//    return -1;
//  }
//  SetCursorPos(x,y);
//  FreeLibrary(hDll);
//  return 0;
//}

void MainWindow::doPressAct()    //执行重放
{
    //打开文件
    QString curPath=QDir::currentPath();//获取系统当前目录
    QString dlgTitle="打开一个文件"; //对话框标题
    QString filter="文本文件(*.txt)"; //文件过滤器
    QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    if (aFileName.isEmpty())
        return;
    QFile tempfile(aFileName);
    if(!tempfile.open(QIODevice::ReadOnly))
        return;
    QTextStream aStream(&tempfile); //用文本流读取文件
    QString str;
    while(!aStream.atEnd())
    {
        str = aStream.readLine();
        QStringList tempList =  str.split(' ');
        QStringList tempList1 = tempList[1].split(':');
        if(tempList[0] == "mouse")
        {
            QStringList tempList2 = tempList1[1].split(',');
            tempList2[0].remove('(');
            tempList2[1].remove(')');
            int x = tempList2[0].toInt();
            int y = tempList2[1].toInt();
            if(tempList1[0] == "left")
            {
                qDebug()<<x<<'\t'<<y;
                mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, x, y, 0, 0);
            }
            else if(tempList1[0] == "right")
            {
                qDebug()<<x<<'\t'<<y;
                mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP,x, y, 0, 0);
            }
            else
            {
                qDebug()<<x<<'\t'<<y;
                SetCursorPos(x, y);
                QTime t;
                t.start();
                while(t.elapsed()<10*1)
                   QCoreApplication::processEvents();
            }
//            Sleep(1);

        }
        else
        {
            if(tempList1[0] == "down")
            {
                qDebug()<<tempList1[1];
                keybd_event(tempList1[1].toInt(), 0, 0, 0);
            }
            else
            {
                qDebug()<<tempList1[1];
                keybd_event(tempList1[1].toInt(), 0, KEYEVENTF_KEYUP, 0);
            }
//            Sleep(50);
            QTime t;
            t.start();
            while(t.elapsed()<50*1)
               QCoreApplication::processEvents();
        }
    }
    tempfile.close();
} 

void MainWindow::installHook()      //创建钩子
{
    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL,keyProc,nullptr,0);       //创建键盘钩子
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL,mouseProc,nullptr,0);      //创建鼠标钩子
}

void MainWindow::unInstallHook()    //卸载钩子
{
    UnhookWindowsHookEx(keyHook);   //卸载键盘钩子
    UnhookWindowsHookEx(mouseHook); //卸载鼠标钩子
    keyHook = nullptr;
    mouseHook = nullptr;
}


MainWindow::~MainWindow()
{
    delete ui;
}
