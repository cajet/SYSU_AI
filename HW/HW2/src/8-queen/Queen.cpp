#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

bool board[8][8];  //棋盘
int SearchNodeNum; //记录搜索的节点个数
void Init();
int Estimate(bool maze[8][8]);
bool M_HillClim_Search();
bool F_HillClim_Search();
bool R_HillClim_Search();
bool Sannealing_Search();

struct node
{
    bool maze[8][8];
    int h;
    node(bool m[8][8]) {
        memcpy(maze, m, sizeof(bool)*64);
        h = Estimate(maze);
    }
    node(){}
};

bool com(const node& n1, const node& n2) {
    return n1.h < n2.h;
}

void Init() {
    memset(board, false, sizeof(board));

    for (int j = 0; j< 8; j++) {
        int n = rand()%8;
        board[n][j] = true;
    }
}

int main() {
    int testcase = 100;               //20个测试样例
    srand((unsigned)(time(NULL)));
    bool tmpboard[8][8];            //记录棋盘信息
    double suc1, suc2, suc3, suc4; //记录成功的个数
    double p1, p2, p3, p4;         //记录搜索耗散
    suc1 = suc2 = suc3 = suc4 = 0;
    p1 = p2 = p3 = p4 = 0;

    for (int i = 0; i < testcase; i++) {
        cout << "正在进行第"<< i+1 << "个测试..."<< endl;
        Init();                                  //生成初始棋盘
        memcpy(tmpboard, board, sizeof(bool)*64); //保存初始棋盘

       //最陡上升爬山算法
        if (M_HillClim_Search()) {
            suc1++;
            p1 += SearchNodeNum;
        }

        //首选爬山法
        memcpy(board, tmpboard, sizeof(bool)*64); 
        if (F_HillClim_Search()) {
            suc2++;
            p2 += SearchNodeNum;
        }

        //随机重启爬山法
        memcpy(board, tmpboard, sizeof(bool)*64); 
        if(R_HillClim_Search()) {
            suc3++;
            p3 += SearchNodeNum;
        }

        //模拟退火算法
        memcpy(board, tmpboard, sizeof(bool)*64); 
        if (Sannealing_Search()) {
            suc4++;
            p4 += SearchNodeNum;
        }
    }

    //打印信息
    cout<< endl<< "--------------------------------测试结果-----------------------------------------"<< endl<< endl;
    cout << "算法             解决率             耗散" << endl;
    if (suc1)
        printf("最陡爬山法:       %f         %f\n", suc1/testcase, p1/suc1);
    else
        printf("最陡爬山法:        %f         NAN\n", suc1/testcase);

    if (suc2)
        printf("首选爬山法:       %f         %f\n", suc2/testcase, p2/suc2);
    else
        printf("首选爬山法:       %f         NAN\n", suc2/testcase);

    if(suc3) 
        printf("随机重启爬山法:  %f         %f\n", suc3/testcase, p3/suc3);
    else
        printf("随机重启爬山法:  %f         NAN\n", suc3/testcase);
    
    if (suc4)
        printf("模拟退火法:      %f         %f\n", suc4/testcase, p4/suc4);
    else
        printf("模拟退火法:      %f         NAN\n", suc4/testcase);
    
}


int Estimate(bool board[8][8]) {
    int h = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j]) {
                //同行
                for (int k = j+1; k < 8; k++)
                    if (board[i][k])
                        h++;
                //同列
                for (int k = i+1; k < 8;k++)
                    if(board[k][j])
                        h++;
                //同对角线
                int n1 = j-1;
                int n2 = j+1;
                int k = i+1;
                for (; k < 8; k++, n1--, n2++) {
                    if(n1>=0)
                        if (board[k][n1])
                            h++;
                    if (n2 < 8)
                        if (board[k][n2])
                            h++;
                }
            }
        }
    }
    return h;
}

//最陡上升爬山算法
bool M_HillClim_Search() {
    bool tmp[8][8];
    SearchNodeNum = 0;
    memcpy(tmp, board, sizeof(bool)*64);
    vector<node> neighbor;
    //一次移动一列
    int index = 0;
    while (Estimate(tmp) != 0 && SearchNodeNum < 30000) {
        //找出所有后继点
        for (int j = 0; j < 8; j++) {
            for (int i = 0; i < 8; i++) {
                if (tmp[i][j]) {  //找到当前列放置皇后的位置
                    index = i;
                    tmp[i][j] = false;
                    break;
                }
            }
            for (int i = 0; i < 8; i++) {
                if (i != index) { //把皇后放在其他7个新位置 
                    tmp[i][j] = true;
                    neighbor.push_back(node(tmp));  //产生7个新节点
                    tmp[i][j] = false;
                }
            }
            tmp[index][j] = true;
        }

        sort(neighbor.begin(), neighbor.end(),com);
        //当最优后继节点次于当前节点，到达最高点，搜索结束
        if (Estimate(tmp) <= neighbor[0].h)
            break;
        memcpy(tmp, neighbor[0].maze, sizeof(bool)*64);
        SearchNodeNum++;
        neighbor.clear();
    }
    if (Estimate(tmp))  //估价值不为0则找到的不符合要求，搜索失败
        return false;
    return true;
}

//首选爬山算法
bool F_HillClim_Search() {
    bool tmp[8][8];
    SearchNodeNum = 0;
    vector<node> neighbor;
    memcpy(tmp, board, sizeof(bool)*64);
    //一次移动一列
    int index = 0;
    while (Estimate(tmp) != 0 && SearchNodeNum < 30000) {
        //首先找出所有后继节点
        for (int j = 0; j < 8; j++) {
            for (int i = 0; i < 8; i++) {
                if (tmp[i][j]) {
                    index = i;
                    tmp[i][j] = false;
                    break;
                }
            }
            for (int i = 0; i < 8; i++) {
                if (i != index) {
                    tmp[i][j] = true;
                    neighbor.push_back(node(tmp));
                    tmp[i][j] = false;
                }
            }
            tmp[index][j] = true;
        }

        //随机找出一个后继节点，直到该节点优于当前节点
        //找不到该节点则结束搜索
        int num = 56;
        while (!neighbor.empty()) {
            index = rand()%num;  //随机选择后继节点中的第一个出现的优于当前的节点
            if (Estimate(tmp) > neighbor[index].h) {
                memcpy(tmp, neighbor[index].maze, sizeof(bool)*64);
                break;
            }
            neighbor.erase(neighbor.begin()+index);
            num--;
        }
        if (num==0)  //找不到优于当前的节点
            break;
        neighbor.clear();
        SearchNodeNum++;
        
    }
    if (Estimate(tmp))
        return false;
    return true;
}

//随机重启爬山算法
bool R_HillClim_Search() {
    bool tmp[8][8];
    SearchNodeNum = 0;
    vector<node> neighbor;
    memcpy(tmp, board, sizeof(bool)*64);
    //一次移动一列
    int index = 0;
    while (Estimate(tmp) != 0) {
        //若此时当条路径搜索耗散大于3000则认为该搜索不能找到目标
        //此时随机重启
        if (SearchNodeNum%(30000+1) >= 30000) {
            Init();
            memcpy(tmp, board, sizeof(bool)*64);
        }
        //首先找出所有后继节点
        for (int j = 0; j < 8; j++) {
            for (int i = 0; i < 8; i++) {
                if (tmp[i][j]) {
                    index = i;
                    tmp[i][j] = false;
                    break;
                }
            }
            for (int i = 0; i < 8; i++) {
                if (i != index) {
                    tmp[i][j] = true;
                    neighbor.push_back(node(tmp));
                    tmp[i][j] = false;
                }
            }
            tmp[index][j] = true;
        }

        //随机找出一个后继节点，直到该节点优于当前节点
        int num = 56;
        while (!neighbor.empty()) {
            index = rand()%num;
            if (Estimate(tmp) > neighbor[index].h) {
                memcpy(tmp, neighbor[index].maze, sizeof(bool)*64);
                break;
            }
            neighbor.erase(neighbor.begin()+index);
            num--;
        }
        //此时没有找到优于当前状态的后继节点，则重启
        if (num==0) {
            Init();
            memcpy(tmp, board, sizeof(bool)*64);
        }
        neighbor.clear();
        SearchNodeNum++;
    }

    if (Estimate(tmp))
        return false;
    return true;

}

//模拟退火算法
bool Sannealing_Search(){
    bool tmp[8][8];
    SearchNodeNum = 0;
    vector<node> neighbor;
    memcpy(tmp, board, sizeof(bool)*64);
    double T;        //温度
    double cold = 0.95;   //冷却因子，0<cold<1 。cold越大，降温越慢；cold越小，降温越快
    double e = 2.71828;   //概率计算的底数
    //一次移动一列
    int index = 0;
    while (Estimate(tmp) != 0 && SearchNodeNum < 30000) {
        //首先找出所有后继节点
        for (int j = 0; j < 8; j++) {
            for (int i = 0; i < 8; i++) {
                if (tmp[i][j]) {
                    index = i;
                    tmp[i][j] = false;
                    break;
                }
            }
            for (int i = 0; i < 8; i++) {
                if (i != index) {
                    tmp[i][j] = true;
                    neighbor.push_back(node(tmp));
                    tmp[i][j] = false;
                }
            }
            tmp[index][j] = true;
        }

        //随机找出一个后继节点，直到该节点优于当前节点
        //找不到该节点则结束搜索
        int delta;
        int num = 56;
		T= 300;  //初始温度为300
        while (!neighbor.empty()) {
            index = rand()%num;
            delta = Estimate(tmp) - neighbor[index].h;
			T= T*cold;  //温度逐渐下降
            if (delta>0) {  //优于当前节点，移动被接受
                memcpy(tmp, neighbor[index].maze, sizeof(bool)*64);
                break;
            } else {    //劣于当前节点，以一定概率pow(e, delta/T)接受
                if (rand()%100000/100000.0 < pow(e, delta/T)) {
                    memcpy(tmp, neighbor[index].maze, sizeof(bool)*64);
                    break;
                }
            }
            num--;
            neighbor.erase(neighbor.begin()+index);
        }
        if (num==0)
            break;
        neighbor.clear();
        SearchNodeNum++;
        
    }
    if (Estimate(tmp))
        return false;
    return true;
}
