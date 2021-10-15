#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>      
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>       //鼠标点击事件
#include <QTimer>            //定时器对象
#include "Mine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QPushButton *MainButton[30][30];    //雷按钮
    Mine *mine;                 //地雷
    int retainx,retainy;        //鼠标点击的位置
    int isStart;                //是否开始游戏
    int uigrade;                //难度
    int seconds;                //用时

    //定义需要用到的定时器（第一个用来计时，第二个用来鼠标点击）
    int Timer1;                 //用来计算时间
    int Timer2;                 //用来判断鼠标点击是单击还是双击
    int ClickedState;           //点击状态，常态不点击为0，单击对应1，双击对应2

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void newGame(int difficult);        //初始化游戏
    void startTime();                   //开始计时
    void empty();                       //清空之前创建的东西
    void chordingShow(int x,int y);     //双击的展示
    void leftShow(int x,int y);         //左键单击的显示
    void showtime();                    //更新时钟
    void failShow(int ii,int jj);       //失败的时候显示所有的雷
    void ButtonClick(int i,int j);       //按钮点击
    void showButton(int i,int j,int model); //点击按钮
    void timerSingleClick();                //定时器事件，处理单击任务
    int* reverseij(int i,int j);            //保存当前坐标
    void mousePressEvent(QMouseEvent *event);       //鼠标点击事件
    void mouseDoubleClickEvent(QMouseEvent *event); //鼠标双击事件
private slots:
    void timerEvent(QTimerEvent *event);
    void on_pushButton_clicked();           //笑脸按钮
    void on_action_2_triggered();           //菜单:初级
    void on_action_4_triggered();           //菜单:中级
    void on_action_5_triggered();           //菜单:高级
    void on_action_6_triggered();           //菜单:游戏说明

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
