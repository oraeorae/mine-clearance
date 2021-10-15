#include "Mine.h"
#include "ui_mainwindow.h"
//注意是map[y][x]

//产生新的雷（map中-1表示雷）
void Mine::newMine() {

    //初始化
    for(int j = 0 ; j < 30 ; j++ ){
        for(int i = 0 ; i < 30 ; i++ ){
            map[j][i] = 0;
            w_map[j][i] = 0;                //注意这里w_map得初始化，否则会出错
        }
    }
    lei = lei_all;
    success = 0;           //找到的雷初始化为0

    int tmp = 0;           //随机生成对应雷
    srand((unsigned)time(NULL));
    while(1){
        int x = rand() % x_N;              //产生0到雷数的数字
        int y = rand() % y_N;
        if (map[y][x] != -1) {             //如果该位置没有雷
            map[y][x] = -1;                //埋下地雷
            tmp++;
        }
        if( tmp >= lei_all  )    break;    //埋下对应雷数就停止
    }
}

//根据雷填充map地图
void Mine::calcNum() {         //计算雷的数量(用map[j][i]更佳，不容易混)
    //根据展开方格的数字去推断其相邻九宫格内未展开方格下面是否是地雷

    //填入数字
    for(int j = 0 ; j < y_N ; j ++ ){
        for(int i = 0 ; i < x_N ; i++ ){
            if( map[j][i] != -1 ){          //如果不是雷就计算周围的雷
                int tmp = 0;
                //周围一圈
                if (i - 1 >= 0 && j + 1 < y_N && map[j + 1][i - 1] == -1)   tmp++;
                if (j + 1 < y_N && map[j + 1][i] == -1)   tmp++;
                if (j + 1 < y_N && i + 1 < x_N && map[j + 1][i + 1] == -1)   tmp++;

                if (i - 1 >= 0 && map[j][i - 1] == -1)   tmp++;
                if (i + 1 < x_N && map[j][i + 1] == -1)   tmp++;

                if (i - 1 >= 0 && j - 1 >= 0 && map[j - 1][i - 1] == -1)   tmp++;
                if (j - 1 >= 0 && map[j - 1][i] == -1)   tmp++;
                if (i + 1 < x_N && j - 1 >= 0 && map[j - 1][i + 1] == -1)   tmp++;
                map[j][i] = tmp;
            }
        }
    }
}

void Mine::left(int x, int y) {
    //🙂🙁
    //左键用于打开安全的格子，推进游戏进度；
    if (map[y][x] == -1) {
        //信息框（https://www.cnblogs.com/zhoug2020/p/10094855.html，注意加上头文件）
         QMessageBox::information(NULL, "游戏结束", "很遗憾，你已经踩到雷了，游戏结束！");
    }
    else {
        if (map[y][x] == 0) {
            w_map[y][x] = -3;           //-3在表地图表示为空
        }
        else {
            w_map[y][x] = map[y][x];
        }
    }
}

void Mine::right(int x, int y) {
    //后续补充再次右键可取消
    if( lei > 0 ){      //如果标记够用的话
        lei--;          //标记雷的数量减1
        //右键用于标记地雷，以辅助判断，或为接下来的双击做准备；(-2表示标记)
        if( w_map[y][x] == -2 ){
            w_map[y][x] = 0;
        }else{
            w_map[y][x] = -2;
        }
        if( map[y][x] == -1 ){          //如果找到的是雷
            success++;
        }
    }else{
        if( success == lei_all ){            //成功找到所有的雷
            //此处应该贴胜利代码
            QMessageBox::information(NULL, "游戏结束", "恭喜你，游戏胜利！");
        }

    }
}

Mine::Mine(int _grade):grade(_grade){
     //(初级为9*9个方块10个雷，中级为16*16个方块40个雷，高级为16*30个方块99个雷
    if( grade == 1 ){
        y_N = 9;
        x_N = 9;
        lei = lei_all =  10;               //旗的个数和雷的个数
    }else if( grade == 2 ){
        y_N = 16;
        x_N = 16;
        lei = lei_all =  40;               //旗的个数和雷的个数

    }else if( grade == 3 ){
        y_N = 16;
        x_N = 30;
        lei = lei_all =  99;               //旗的个数和雷的个数
    }

    newMine();
    calcNum();

    //显示地图
    for(int j = 0 ; j < y_N ; j ++ ){
        for(int i = 0 ; i < x_N ; i++ ){
            std::cout<<map[j][i]<<" ";
        }
        std::cout<<std::endl;
    }

    //显示地图
    for(int j = 0 ; j < y_N ; j ++ ){
        for(int i = 0 ; i < x_N ; i++ ){
            std::cout<<w_map[j][i]<<" ";
        }
        std::cout<<std::endl;
    }

}

Mine::~Mine(){}
