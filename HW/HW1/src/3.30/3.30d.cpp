	# include<stdio.h> 
	# include<stdlib.h>
	# include<time.h>
	# include<math.h> 
	# define Max 31 
	
	int cn,tt,start;                          // 要经过的城市个数,,起点
	double arry1[Max][Max];                   // 邻接矩阵，存放两两城市间的距离
	double fn=0,gn=0,hn=0;                    // 启发函数
	double f1=0,g1=0,h1=0;
	int arry3[Max];                           // 存放已历经的城市名
	int arry4[Max];                           // 标志位数组，cn个城市中已历经的置0，未历经的置1
	
	// 定义顶点数据类型
	struct Vertex               
	{
	  int x;
	  int y;
	} City[Max];
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// 主函数
	int main() {  
	  void RandNum(int); 
	  void CityCoordinate();
	  double CityCost(int,int);
	  void TSP();
	  double MaxLengh();
	
	  int i,j;
	
	  CityCoordinate();          // 随机生成并显示20个城市及其坐标
	
	   for(i=1; i<Max; i++)                 // 随机生成并显示表示两城市间距离的邻接矩阵
	   {
	     tt=0;
	     for(j=i; j<Max; j++,tt++)
	     {  
		   if(i==j)   arry1[i][j]=0;
	       else   arry1[i][j]=CityCost(i,j);
	     }
	   }
	  
	  TSP();                               // 用最小生成树查找最短路径
	
	  printf("\n从%d出发的最佳路径为：%d→",start,start);
	  for(i=2;i<=cn;i++) printf("%d→",arry3[i]);
	  printf("%d\n",arry3[cn+1]);
	
	  printf("总路径长度为:%f\n",fn);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// 随机数产生器
	int RandNum(int max)        
	{
	  int m;
	  m=rand()%(max-1)+1;                   // 产生一个1~20的随机数
	  return m;
	}
	
	// 生成并显示城市坐标
	void CityCoordinate()      
	{
	  int i,j,hh=0;
	
	  srand((unsigned)time(NULL));   // 使用当前时间作为种子
	  City[1].x=RandNum(Max);        // 生成并显示第1个城市的坐标
	  City[1].y=RandNum(Max);
	  printf("City[1]的坐标: (%d,%d);     ", City[1].x, City[1].y);
	  
	
	  for(i=2;i<Max;i++)            // 生成第2~20个城市的坐标
	  {
	    City[i].x=RandNum(Max);
	    City[i].y=RandNum(Max);
	
	    for(j=1;j<i;j++)            // 检查重合坐标,有重合时重新生成
	       if(City[i].x== City[j].x&& City[i].y== City[j].y)
	         i=i-1;
	 
		hh++;                             // 换行
	    if(0!=i%2)  hh=0;              
		if(0==hh)   printf("\n");
	
	    printf("City[%d]的坐标: (%d,%d);   ", i, City[i].x, City[i].y);// 显示第i个城市的坐标
	  }
	}
	
	// 计算并显示城市间的欧式距离
	double CityCost(int i,int j)
	{
	  int x1,x2,y1,y2,hh=0;
	  double Distance,t;
	
	  x1= City[i].x;
	  x2= City[j].x;
	  y1= City[i].y;
	  y2= City[j].y;
	  t=(x1-x2)* (x1-x2)+(y1-y2)* (y1-y2);
	  Distance=sqrt(t);
	  arry1[i][j]=Distance;
	  
	  hh++;
	  if(0!=tt%2)  hh=0;            // 换行  
	  if(0==hh)   printf("\n");
	
	  printf("%d与%d的距离：%3.2f      ", i, j, Distance);
	  return arry1[i][j];
	}
	
	// 用启发式的MST查找最短路径/////////////////////////////////////////////////////////
	void TSP()
	{
	  int Mnode;                                         // 起点，当前搜索层的父节点
	  int h,i,k,l,m,n,nn;
	  int x,y=0;
	  int arry2[Max]={0,0, 0,0, 0, 0,0, 0,0, 0, 0,0, 0,0, 0, 0,0, 0,0, 0,0}; // 标志位数组，已历经的置0，未历经的置1
	  double temp1=100,temp2=100;
	  double layer1[Max];                                      // 初始化当前搜索层节点
	  double layer2[Max];                                      // 初始化后继搜索层节点
	
	  printf("\n请输入要经过的城市个数：");
	  scanf("%d",&cn);
	  printf("\n");
	
	  printf("请输入要历经的城市：\n");
	  for(h=1;h<=cn;h++)                  // 输入历经节点
	  {
	    scanf("%d",&x);
		if(0==arry2[x]) arry2[x]=1;   // 避免重复
		else if(1==arry2[x]) h=h-1;
	  }
	
	  printf("\n");
	  for(i=1;i<Max;i++)                  // 显示历经节点
	     if(1==arry2[i]) 
		    printf("%d ",i);
	  printf("\n");
	
	  printf("请输入出发城市：");         // 输入出发点
	  scanf("%d",&start); 
	  printf("\n");
	
	  arry2[start]=0;                         // 初始化
	  arry3[1]=start;
	  arry3[cn+1]=start;
	  Mnode=arry3[1];
	
	//for(i=1;i<Max;i++) printf("%d ",arry2[i]);    // 显示arry2[]00000000000000000000000000000000
	// printf("\n");
	
	 ///////////////////////////////////////////////////////////////////////////////////// 搜索路径
	for(n=2;n<=cn;n++)                       // 找出城市2～cn
	{
	  for(nn=1;nn<Max;nn++)                  // 初始化layer1[]和layer2[]
	  {
		  layer1[nn]=0;
	      layer2[nn]=0;
	  }
	
	  for(k=1;k<Max;k++)                    // 搜索Mnode所有后继节点                   
	     if(1==arry2[k]) 
		 {  
			gn=g1+arry1[Mnode][k];
			hn=arry1[k][start];
			fn=gn+hn;
	        layer1[k]=fn;
		 }
	
	  for(l=1;l<Max;l++)                   // 找出第一个后继节点y，并初始化y
	    if(0!=layer1[l]) 
		{
		  y=l;
		  break;
		} 
	
	// for(i=1;i<Max;i++) printf("%d ",arry2[i]);    // 显示arry2[]00000000000000000000000000000000
	// printf("\n");
	
	  for(m=y+1;m<Max;m++)  //。。。。。。。。。。。。。。比较并查找最佳后继
	  { 
		if(layer1[y]==layer1[m]) //////////////// 如果出现两个后继代价相同，搜索它们的下一层节点
		{
		  Mnode=y;                        ////// 先以y为父节点
	      arry2[y]=0;                     
		  for(k=1;k<Max;k++)                   // 搜索y的所有后继节点
		     if(1==arry2[k]) 
		     {
		  	   gn=g1+arry1[Mnode][k];
		 	   hn=arry1[k][start];
			   fn=gn+hn;
	           layer2[k]=fn;
		     }
		  for(l=1;l<Max;l++)                  // 找出y的代价最小的后继节点
	        if(0!=layer2[l]&&temp1>layer2[l]) 	temp1=layer2[l]; 
	
		  for(nn=1;nn<Max;nn++)    layer2[nn]=0;   // 初始化layer2[]
	
		  Mnode=m;                       ////// 再以m为父节点
	      arry2[y]=1;
	      arry2[m]=0;                     
		  for(k=1;k<Max;k++)                   // 搜索m的所有后继节点
		      if(1==arry2[k]) 
		     {
		  	   gn=g1+arry1[Mnode][k];
		 	   hn=arry1[k][start];
			   fn=gn+hn;
	           layer2[k]=fn;
		     }
		  for(l=1;l<Max;l++)                  // 找出m的代价最小的后继节点
	          if(0!=layer2[l]&&temp2>layer2[l])	  temp2=layer2[l]; 
	
		  arry2[y]=1;    arry2[m]=1;         // 消除假设条件下对arry2[]值的改变
	
		  if (temp1>temp2)  y=m; 
		}
	    else if(0!=layer1[m]&&layer1[y]>layer1[m])  y=m; 
	  }                      //。。。。。。。。。。。。。。 比较并查找最佳后继
	
	 //for(i=1;i<Max;i++) printf("%d ",arry2[i]);    // 显示arry2[]00000000000000000000000000000000
	 //printf("\n");
	
	 g1=g1+arry1[Mnode][y];
	 Mnode=y; 
	 arry2[y]=0;
	 arry3[n]=y;
	 }
	 //for(i=1;i<Max;i++) printf("%d ",arry2[i]);    // 显示arry2[]00000000000000000000000000000000
	 //  printf("\n");
	
	fn=g1+arry1[y][start];
	}///////////////////////////////////////////////////////////////////////////////////// 搜索路径
