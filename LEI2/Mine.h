#ifndef MINE_H
#define MINE_H

#include <QMessageBox>
#include <iostream>
#include <stdlib.h>
#include <time.h>

class Mine{             //扫雷的类
public:
    int map[30][30];                //雷图，实际图（雷表示-1）
    int w_map[30][30];              //显示出来的地图
    int lei;                        //右键旗的次数
    int lei_all;                    //雷的个数
    int success;                    //成功全部雷即为胜利
    int y_N,x_N;                    //map[y_N][x_N]
    int grade;                      //等级(初级，中级，高级)
    //(初级为9*9个方块10个雷，中级为16*16个方块40个雷，高级为16*30个方块99个雷

    Mine(int _grade);
    ~Mine();

    void newMine();                 //产生新的雷
    void calcNum();                 //计算雷的数量
    void left(int x, int y);        //左击
    void right(int x, int y);       //右击
};


#endif // MINE_H
