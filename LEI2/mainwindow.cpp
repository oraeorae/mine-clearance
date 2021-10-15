#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     uigrade = 1;            //初始化等级为1
     newGame(1);             //初始化游戏，生成游戏
}

MainWindow::~MainWindow()
{
    delete ui;
}

//显示游戏时间（计时）
void MainWindow::showtime(){
    // ui->textEdit_2->setText("str");
    seconds++;
    QString str;
    //前位补0：https://www.cnblogs.com/zzzsj/p/14566157.html
    str = QString("%01").arg(seconds, 3, 10, QLatin1Char('0'));
    ui->lcdNumber->display(str);
}

//开始计时
void MainWindow::startTime(){
    Timer1 = startTimer(1000);
}

//清空本来生成的东西
void MainWindow::empty(){
    //killTimer(Timer1);          //先把计时停了
    //qDebug()<<"12121";
    //在重新初始化之前，先删除之前的new出来的东西，否则会出现虚幻的雷之类的
    //把new出来的按钮delete掉
    for(int j = 0 ; j < mine->y_N ; j ++ ){
        for(int i = 0 ; i < mine->x_N ; i ++ ){
            delete MainButton[j][i];
        }
    }
    //把雷delete掉
    delete mine;
}

//点击笑脸按钮
void MainWindow::on_pushButton_clicked()
{
    //qDebug()<<"尝试重新开始游戏";
    if( isStart != 0 ){         //设置必须失败才能重新开始
        empty();                //清空本来的东西
        newGame(uigrade);       //生成新的游戏
    }
}

//双击的展示（用递归实现最佳）
void MainWindow::chordingShow(int x,int y){
    if( x < 0 || y < 0 || x >= mine->x_N || y >= mine->y_N )    return;
    if( mine->map[y][x] == 0 && mine->w_map[y][x] == 0 ){                 //只有0且还没打开才能双击
        leftShow(x,y);
        mine->left(x,y);
        //对四周（注意加上还没打开的限制，否则会报错）
        chordingShow(x+1,y);
        chordingShow(x,y+1);
        chordingShow(x,y-1);
        chordingShow(x-1,y);

    }else if( mine->map[y][x] != -1 ){      //如果是其他，就只能左键点击
        leftShow(x,y);
        mine->left(x,y);
    }
    return;
}

//扫雷失败会把所有的雷位置展示出来
void MainWindow::failShow(int ii,int jj)          //参数为点击的位置（标红）
{
    isStart = -1;                   //表示为游戏失败
    ui->pushButton->setText("🙁");
    killTimer(Timer1);
     //if (myTimer->isActive())           myTimer->stop();
     for(int j = 0 ; j < mine->y_N ; j ++ ){
        for(int i = 0 ; i < mine->x_N ; i ++ ){
            //失败还得把按钮都变成不可点击
            if(mine->w_map[j][i] == 0 ){            //加上判断，避免失败后一些按钮会恢复
                MainButton[j][i]->setEnabled(false);
            }

            if( mine->map[j][i] == -1  ){
                 MainButton[j][i]->setStyleSheet("background-color:#c2d0ea;font-size:10px;font-weight:bold;");
                if( ii == i && jj == j ){           //点击的位置标红
                    MainButton[j][i]->setStyleSheet("color:#1212f9;background-color:#ff0000;font-size:10px;font-weight:bold;");
                }
                mine->w_map[j][i] = -1;
                this->MainButton[j][i]->setDown(true);
                this->MainButton[j][i]->setText("💣");
            }
        }
    }


}

//初始化游戏
void MainWindow::newGame(int difficult){
    // 添加图标
    QIcon icon(":/new/prefix1/img/lei.ico");
    this->setWindowIcon(icon);
    seconds=0;
    int move_x,move_y;
    if( difficult == 1 ){
        this->resize(220,260);
        move_x = 20,move_y = 65;
        ui->lcdNumber_2->move(20,10);
        ui->pushButton->move(90,10);
        ui->lcdNumber->move(130,10);

    }else if( difficult == 2 ){
        this->resize(360,400);
        move_x = 20,move_y = 65;
        ui->lcdNumber_2->move(20,10);
        ui->pushButton->move(160,10);
        ui->lcdNumber->move(270,10);

    }else if( difficult == 3 ){
        this->resize(640,400);
        move_x = 20,move_y = 65;
        ui->lcdNumber_2->move(20,10);
        ui->pushButton->move(300,10);
        ui->lcdNumber->move(550,10);

    }

    //qDebug()<<"重新游戏";
    isStart = 0;           //尚未点击开始按钮

   //设置背景颜色(#dadfec;)#91a0b3;
    this->setStyleSheet("background-color:#cfd8df");

    ui->lcdNumber->setStyleSheet("background-color:#000000;color:#ff0000;");
    ui->lcdNumber_2->setStyleSheet("background-color:#000000;color:#ff0000;");

    mine = new Mine(difficult);            //生成地雷
   //Qt按钮设置：https://baijiahao.baidu.com/s?id=1691291899564946968&wfr=spider&for=pc

   int size = 20;

   //注意是[y][x]对应[j][i]

   for(int j = 0 ; j < mine->y_N ; j++ ){
        for(int i = 0 ; i < mine->x_N ; i++ ){
            MainButton[j][i] = new QPushButton(" ",this);

            MainButton[j][i]->setCheckable(true);
            MainButton[j][i]->setAutoExclusive(true);

            //设置按钮颜色border:2px solid #808080 rgb(192,192,192);#77a7f0 #3547b2;
            //阴影感：https://blog.csdn.net/guorong520/article/details/107126554/
            MainButton[j][i]->setStyleSheet("background-color:#6fabee;border-bottom:4px solid #619fec;border-right:4px solid #619fec;");
            MainButton[j][i]->resize(size,size);
            //MainButton[j][i]->setEnabled(false);
            MainButton[j][i]->move(move_x+size*(i),move_y+size*(j));     //将按钮b1移动到点（50，50）的位置
           // MainButton[j][i]->setParent("1",this);

            MainButton[j][i]->show();
            //给connect传参数的方法：https://blog.csdn.net/qq_38188725/article/details/80686793
            //解决鼠标左键无法响应鼠标事件，和connect无法响应
            connect(MainButton[j][i], &QPushButton::clicked,this,
                    [=]{ButtonClick(i,j);
            });
       }
   }

   ui->pushButton->setText("🙂");
   ui->pushButton->resize(30,30);

   //设置lcd位数
   ui->lcdNumber->setDigitCount(4);
   ui->lcdNumber_2->setDigitCount(3);
   QString tmp = QString("%1").arg(mine->lei, 3, 10, QLatin1Char('0'));               //整型转字符串);               //整型转字符串;
   ui->lcdNumber_2->display(tmp);

   ui->lcdNumber->display("000");

   /*
   //显示游戏时间
   myTimer = new QTimer(this);
   connect(myTimer,&QTimer::timeout, this, [=]{
       showtime();});
    */


   //解决鼠标单击还是双击的
   ClickedState = 0; //初始化不点击
   /*
   timer = new QTimer(this);
   connect(timer, &QTimer::timeout, this,[ = ]{
       timerSingleClick();
   });*/
}

//鼠标单击事件
void MainWindow::mousePressEvent(QMouseEvent *event){
    //Timer2 = startTimer(180);
    //timer->start(300);

    //鼠标事件参考链接：https://www.cnblogs.com/aiguona/p/9999507.html
    //qDebug("鼠标点击");
       // 如果是鼠标左键按下
       // 如果是鼠标右键按下
      if(event->button() == Qt::RightButton)
      {
           //qDebug("鼠标右键点击");
           killTimer(Timer2);
           //timer->stop();
           for( int j = 0 ; j < mine->y_N ; j ++ ){
                for( int i = 0 ; i < mine->x_N ; i ++ ){
                    if(this->MainButton[j][i]->geometry().contains(this->mapFromGlobal(QCursor::pos()))){
                        //qDebug("%d,%d",i,j);
                        showButton(i,j,2);
                    }

               }
           }

      }
}

//鼠标双击事件
void MainWindow::mouseDoubleClickEvent(QMouseEvent *event){
    // 如果是鼠标左键按下（双击）
    if(event->button() == Qt::LeftButton){
        //qDebug("鼠标左键双击");
        for( int j = 0 ; j < mine->y_N ; j ++ ){
             for( int i = 0 ; i < mine->x_N ; i ++ ){
                 if(this->MainButton[j][i]->geometry().contains(this->mapFromGlobal(QCursor::pos()))){
                     //qDebug("%d,%d",i,j);
                     chordingShow(i,j);
                     showButton(i,j,3);
                 }

            }
        }
   }
}

//左键点击的按钮显示
void MainWindow::leftShow(int x,int y){         //左键单击的显示
    int i = x,j = y;
    if( mine->map[j][i] == 0 ){         //rgb(192,192,192)
        MainButton[j][i]->setStyleSheet("background-color:#c2d0ea;font-size:10px;font-weight:bold;");
        this->MainButton[j][i]->setText("");
    }else if( mine->map[j][i] == 1 ){
        //设置按钮颜色border:1px solid #999999;background-color:#c0c0c0
        MainButton[j][i]->setStyleSheet("color:#1212f9;background-color:#c2d0ea;;font-size:10px;font-weight:bold;");
        this->MainButton[j][i]->setText(QString::number(mine->map[j][i]));
    }else if( mine->map[j][i] == 2 ){
        MainButton[j][i]->setStyleSheet("color:#008000;background-color:#c2d0ea;;font-size:10px;font-weight:bold;");
        this->MainButton[j][i]->setText(QString::number(mine->map[j][i]));
    }else if( mine->map[j][i] == 3 ){
        MainButton[j][i]->setStyleSheet("color:#fe0202;background-color:#c2d0ea;;font-size:10px;font-weight:bold;");
        this->MainButton[j][i]->setText(QString::number(mine->map[j][i]));
    }else{
        MainButton[j][i]->setStyleSheet("color:#131386;background-color:#c2d0ea;;font-size:10px;font-weight:bold;");
        this->MainButton[j][i]->setText(QString::number(mine->map[j][i]));
    }
    this->MainButton[j][i]->setDown(true);

}

//按钮点击触发的事件
void MainWindow::showButton(int i, int j, int model){
    if( i < 0 && j < 0 )    return;

    if( isStart == 0 ){                 //如果还没未开始，则现在开始计时
        startTime();
    }
    this->MainButton[j][i]->setEnabled(false);          //已点击过的按钮无法再点击
    //注意这里的i和j对应x,y
    //qDebug("%d,%d",i,j);
    if( model == 1 ){               //左键点击
        //w_map为0说明没打开，没打开的才能点击
        if( mine->w_map[j][i] == 0 ){
            //qDebug("1111");

            //this->MainButton[j][i]->setText(QString::number(mine->map[j][i]));
            if( mine->map[j][i] == -1 ){                //踩到雷
                MainButton[j][i]->setStyleSheet("color:#1212f9;background-color:rgb(192,192,192);font-size:10px;font-weight:bold;");
                //此处应该插入游戏失败的代码
                //this->MainButton[j][i]->setText("💣");
                failShow(i,j);             //失败+显示所有的雷
            }else{
                if( mine->map[j][i] == 0 ){             //如果点击0相当于双击
                     chordingShow(i,j);
                }else{
                    leftShow(i,j);
                }
            }
            if(!this->MainButton[j][i]->isDown())
                this->MainButton[j][i]->setDown(true);
            mine->left(i,j);
            //设置按钮为按下状态：https://blog.csdn.net/qiangzi4646/article/details/80135120

        }else{
            if(!this->MainButton[j][i]->isDown())
                this->MainButton[j][i]->setDown(true);
        }

       // this->MainButton[j][i]->setText("");
    }else if( model == 2 ){         //右键点击（标记）
        //w_map为0说明没打开，没打开的才能点击
        if( mine->w_map[j][i] == 0 ){
            //qDebug("2222");
            if( mine->lei > 0 ){
                mine->right(i,j);
                this->MainButton[j][i]->setText("🚩");
            }
            //前位补0：https://www.cnblogs.com/zzzsj/p/14566157.html
            QString L = QString("%1").arg(mine->lei, 3, 10, QLatin1Char('0'));               //整型转字符串
            ui->lcdNumber_2->display(L);
        }else{
            if(!this->MainButton[j][i]->isDown())
                this->MainButton[j][i]->setDown(true);
        }
    }else if( model == 3 ){
        //qDebug("3333");
        //mine->chording(i,j);        //双击
        this->MainButton[j][i]->setText(" ");
    }
}

//定时器事件
void MainWindow::timerEvent(QTimerEvent *event){
    //QTimerEvent:https://www.cnblogs.com/Yu-nan/p/12091264.html
    //用两个QTimer不对劲
    if(event->timerId() == Timer1) {
        //do Timer1_Fuc
        showtime();
     }
     if(event->timerId() == Timer2) {
        //do Timer2_Fuc
        timerSingleClick();
     }
}

//单击双击操作
void MainWindow::ButtonClick(int i,int j){
       ClickedState ++;
       retainx = i ,retainy = j;
       if(ClickedState == 1)
           Timer2 = startTimer(180);    //180ms为自己设置
       if(ClickedState == 2)
       {
           ClickedState = 0; //初始化
           //qDebug()<<Timer2<<"222";
           killTimer(Timer2);
           //按钮双击响应代码放这里
           //QMessageBox::warning(this,tr("警告"),tr("我是双击！！！"),tr("确定"));
           showButton(i,j,3);
       }

}

//单击双击：https://blog.csdn.net/wdmzslh/article/details/53301495
void MainWindow::timerSingleClick()
{
    ClickedState = 0; //初始化
    //qDebug()<<Timer2<<"111";
    killTimer(Timer2);
    //按钮单击响应代码放这里
    //QMessageBox::warning(this,tr("警告"),tr("单击"),tr("确定"));
    showButton(retainx,retainy,1);
    retainx = -1;
    retainy = -1;
}

//初级被点击
void MainWindow::on_action_2_triggered()
{
    empty();
    uigrade = 1;
    newGame(1);
}

//中级被点击
void MainWindow::on_action_4_triggered()
{
    empty();
    uigrade = 2;
    newGame(2);
}

//高级被点击
void MainWindow::on_action_5_triggered()
{
    empty();
    uigrade = 3;
    newGame(3);
}

//游戏说明被点击
void MainWindow::on_action_6_triggered()
{
    QMessageBox::about(NULL, "游戏说明", "本游戏制作 ——by：orall\n游戏的基本操作包括左键单击、右键单击、双击三种。\n其中左键用于打开安全的格子，推进游戏进度；\n右键用于标记地雷，以辅助判断，或为接下来的双击做准备；\n双击在一个数字周围的地雷标记完时，相当于对数字周围未打开的方块均进行一次左键单击操作。");
}

