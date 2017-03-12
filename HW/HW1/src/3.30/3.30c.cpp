#include <math.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;
struct City {
	double x;
	double y;
	City() {}
	City(double a, double b) {
		x= a;
		y= b;
	}
};

int main() {
	srand((unsigned)(time(0)));
	int num;
	cout<< "请输入城市数目 : ";
	cin>> num;
	City city[num];
	for (int i= 0; i< num; i++) {	
		city[i].x= rand()%100/100.0;
		city[i].y= rand()%100/100.0;
	}
	cout<< "生成的TSP如下： "<< endl;
	cout<< "                           loc1                     loc2                     distance"<< endl;
	double dis= 0;
	for (int i= 0; i< num; i++) {
		for (int j= i+1; j< num; j++) {
			cout<< "city"<< i+1 << " & city"<< j+1;
			dis = sqrt(pow(city[i].x-city[j].x,2)+pow(city[i].y-city[j].y,2));
			cout<< "           ("<< city[i].x<< ", "<< city[i].y<< ")              ";
			cout<< " ("<< city[j].x<< ", "<< city[j].y<< ")              ";
			cout<< dis<< endl;
		}
	}
	
}
