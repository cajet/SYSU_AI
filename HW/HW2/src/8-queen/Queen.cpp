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

bool board[8][8];  //����
int SearchNodeNum; //��¼�����Ľڵ����
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
    int testcase = 100;               //20����������
    srand((unsigned)(time(NULL)));
    bool tmpboard[8][8];            //��¼������Ϣ
    double suc1, suc2, suc3, suc4; //��¼�ɹ��ĸ���
    double p1, p2, p3, p4;         //��¼������ɢ
    suc1 = suc2 = suc3 = suc4 = 0;
    p1 = p2 = p3 = p4 = 0;

    for (int i = 0; i < testcase; i++) {
        cout << "���ڽ��е�"<< i+1 << "������..."<< endl;
        Init();                                  //���ɳ�ʼ����
        memcpy(tmpboard, board, sizeof(bool)*64); //�����ʼ����

       //�������ɽ�㷨
        if (M_HillClim_Search()) {
            suc1++;
            p1 += SearchNodeNum;
        }

        //��ѡ��ɽ��
        memcpy(board, tmpboard, sizeof(bool)*64); 
        if (F_HillClim_Search()) {
            suc2++;
            p2 += SearchNodeNum;
        }

        //���������ɽ��
        memcpy(board, tmpboard, sizeof(bool)*64); 
        if(R_HillClim_Search()) {
            suc3++;
            p3 += SearchNodeNum;
        }

        //ģ���˻��㷨
        memcpy(board, tmpboard, sizeof(bool)*64); 
        if (Sannealing_Search()) {
            suc4++;
            p4 += SearchNodeNum;
        }
    }

    //��ӡ��Ϣ
    cout<< endl<< "--------------------------------���Խ��-----------------------------------------"<< endl<< endl;
    cout << "�㷨             �����             ��ɢ" << endl;
    if (suc1)
        printf("���ɽ��:       %f         %f\n", suc1/testcase, p1/suc1);
    else
        printf("���ɽ��:        %f         NAN\n", suc1/testcase);

    if (suc2)
        printf("��ѡ��ɽ��:       %f         %f\n", suc2/testcase, p2/suc2);
    else
        printf("��ѡ��ɽ��:       %f         NAN\n", suc2/testcase);

    if(suc3) 
        printf("���������ɽ��:  %f         %f\n", suc3/testcase, p3/suc3);
    else
        printf("���������ɽ��:  %f         NAN\n", suc3/testcase);
    
    if (suc4)
        printf("ģ���˻�:      %f         %f\n", suc4/testcase, p4/suc4);
    else
        printf("ģ���˻�:      %f         NAN\n", suc4/testcase);
    
}


int Estimate(bool board[8][8]) {
    int h = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j]) {
                //ͬ��
                for (int k = j+1; k < 8; k++)
                    if (board[i][k])
                        h++;
                //ͬ��
                for (int k = i+1; k < 8;k++)
                    if(board[k][j])
                        h++;
                //ͬ�Խ���
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

//�������ɽ�㷨
bool M_HillClim_Search() {
    bool tmp[8][8];
    SearchNodeNum = 0;
    memcpy(tmp, board, sizeof(bool)*64);
    vector<node> neighbor;
    //һ���ƶ�һ��
    int index = 0;
    while (Estimate(tmp) != 0 && SearchNodeNum < 30000) {
        //�ҳ����к�̵�
        for (int j = 0; j < 8; j++) {
            for (int i = 0; i < 8; i++) {
                if (tmp[i][j]) {  //�ҵ���ǰ�з��ûʺ��λ��
                    index = i;
                    tmp[i][j] = false;
                    break;
                }
            }
            for (int i = 0; i < 8; i++) {
                if (i != index) { //�ѻʺ��������7����λ�� 
                    tmp[i][j] = true;
                    neighbor.push_back(node(tmp));  //����7���½ڵ�
                    tmp[i][j] = false;
                }
            }
            tmp[index][j] = true;
        }

        sort(neighbor.begin(), neighbor.end(),com);
        //�����ź�̽ڵ���ڵ�ǰ�ڵ㣬������ߵ㣬��������
        if (Estimate(tmp) <= neighbor[0].h)
            break;
        memcpy(tmp, neighbor[0].maze, sizeof(bool)*64);
        SearchNodeNum++;
        neighbor.clear();
    }
    if (Estimate(tmp))  //����ֵ��Ϊ0���ҵ��Ĳ�����Ҫ������ʧ��
        return false;
    return true;
}

//��ѡ��ɽ�㷨
bool F_HillClim_Search() {
    bool tmp[8][8];
    SearchNodeNum = 0;
    vector<node> neighbor;
    memcpy(tmp, board, sizeof(bool)*64);
    //һ���ƶ�һ��
    int index = 0;
    while (Estimate(tmp) != 0 && SearchNodeNum < 30000) {
        //�����ҳ����к�̽ڵ�
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

        //����ҳ�һ����̽ڵ㣬ֱ���ýڵ����ڵ�ǰ�ڵ�
        //�Ҳ����ýڵ����������
        int num = 56;
        while (!neighbor.empty()) {
            index = rand()%num;  //���ѡ���̽ڵ��еĵ�һ�����ֵ����ڵ�ǰ�Ľڵ�
            if (Estimate(tmp) > neighbor[index].h) {
                memcpy(tmp, neighbor[index].maze, sizeof(bool)*64);
                break;
            }
            neighbor.erase(neighbor.begin()+index);
            num--;
        }
        if (num==0)  //�Ҳ������ڵ�ǰ�Ľڵ�
            break;
        neighbor.clear();
        SearchNodeNum++;
        
    }
    if (Estimate(tmp))
        return false;
    return true;
}

//���������ɽ�㷨
bool R_HillClim_Search() {
    bool tmp[8][8];
    SearchNodeNum = 0;
    vector<node> neighbor;
    memcpy(tmp, board, sizeof(bool)*64);
    //һ���ƶ�һ��
    int index = 0;
    while (Estimate(tmp) != 0) {
        //����ʱ����·��������ɢ����3000����Ϊ�����������ҵ�Ŀ��
        //��ʱ�������
        if (SearchNodeNum%(30000+1) >= 30000) {
            Init();
            memcpy(tmp, board, sizeof(bool)*64);
        }
        //�����ҳ����к�̽ڵ�
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

        //����ҳ�һ����̽ڵ㣬ֱ���ýڵ����ڵ�ǰ�ڵ�
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
        //��ʱû���ҵ����ڵ�ǰ״̬�ĺ�̽ڵ㣬������
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

//ģ���˻��㷨
bool Sannealing_Search(){
    bool tmp[8][8];
    SearchNodeNum = 0;
    vector<node> neighbor;
    memcpy(tmp, board, sizeof(bool)*64);
    double T;        //�¶�
    double cold = 0.95;   //��ȴ���ӣ�0<cold<1 ��coldԽ�󣬽���Խ����coldԽС������Խ��
    double e = 2.71828;   //���ʼ���ĵ���
    //һ���ƶ�һ��
    int index = 0;
    while (Estimate(tmp) != 0 && SearchNodeNum < 30000) {
        //�����ҳ����к�̽ڵ�
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

        //����ҳ�һ����̽ڵ㣬ֱ���ýڵ����ڵ�ǰ�ڵ�
        //�Ҳ����ýڵ����������
        int delta;
        int num = 56;
		T= 300;  //��ʼ�¶�Ϊ300
        while (!neighbor.empty()) {
            index = rand()%num;
            delta = Estimate(tmp) - neighbor[index].h;
			T= T*cold;  //�¶����½�
            if (delta>0) {  //���ڵ�ǰ�ڵ㣬�ƶ�������
                memcpy(tmp, neighbor[index].maze, sizeof(bool)*64);
                break;
            } else {    //���ڵ�ǰ�ڵ㣬��һ������pow(e, delta/T)����
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
