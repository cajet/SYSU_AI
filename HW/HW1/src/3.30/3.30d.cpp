	# include<stdio.h> 
	# include<stdlib.h>
	# include<time.h>
	# include<math.h> 
	# define Max 31 
	
	int cn,tt,start;                          // Ҫ�����ĳ��и���,,���
	double arry1[Max][Max];                   // �ڽӾ��󣬴���������м�ľ���
	double fn=0,gn=0,hn=0;                    // ��������
	double f1=0,g1=0,h1=0;
	int arry3[Max];                           // ����������ĳ�����
	int arry4[Max];                           // ��־λ���飬cn������������������0��δ��������1
	
	// ���嶥����������
	struct Vertex               
	{
	  int x;
	  int y;
	} City[Max];
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// ������
	int main() {  
	  void RandNum(int); 
	  void CityCoordinate();
	  double CityCost(int,int);
	  void TSP();
	  double MaxLengh();
	
	  int i,j;
	
	  CityCoordinate();          // ������ɲ���ʾ20�����м�������
	
	   for(i=1; i<Max; i++)                 // ������ɲ���ʾ��ʾ�����м������ڽӾ���
	   {
	     tt=0;
	     for(j=i; j<Max; j++,tt++)
	     {  
		   if(i==j)   arry1[i][j]=0;
	       else   arry1[i][j]=CityCost(i,j);
	     }
	   }
	  
	  TSP();                               // ����С�������������·��
	
	  printf("\n��%d���������·��Ϊ��%d��",start,start);
	  for(i=2;i<=cn;i++) printf("%d��",arry3[i]);
	  printf("%d\n",arry3[cn+1]);
	
	  printf("��·������Ϊ:%f\n",fn);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// �����������
	int RandNum(int max)        
	{
	  int m;
	  m=rand()%(max-1)+1;                   // ����һ��1~20�������
	  return m;
	}
	
	// ���ɲ���ʾ��������
	void CityCoordinate()      
	{
	  int i,j,hh=0;
	
	  srand((unsigned)time(NULL));   // ʹ�õ�ǰʱ����Ϊ����
	  City[1].x=RandNum(Max);        // ���ɲ���ʾ��1�����е�����
	  City[1].y=RandNum(Max);
	  printf("City[1]������: (%d,%d);     ", City[1].x, City[1].y);
	  
	
	  for(i=2;i<Max;i++)            // ���ɵ�2~20�����е�����
	  {
	    City[i].x=RandNum(Max);
	    City[i].y=RandNum(Max);
	
	    for(j=1;j<i;j++)            // ����غ�����,���غ�ʱ��������
	       if(City[i].x== City[j].x&& City[i].y== City[j].y)
	         i=i-1;
	 
		hh++;                             // ����
	    if(0!=i%2)  hh=0;              
		if(0==hh)   printf("\n");
	
	    printf("City[%d]������: (%d,%d);   ", i, City[i].x, City[i].y);// ��ʾ��i�����е�����
	  }
	}
	
	// ���㲢��ʾ���м��ŷʽ����
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
	  if(0!=tt%2)  hh=0;            // ����  
	  if(0==hh)   printf("\n");
	
	  printf("%d��%d�ľ��룺%3.2f      ", i, j, Distance);
	  return arry1[i][j];
	}
	
	// ������ʽ��MST�������·��/////////////////////////////////////////////////////////
	void TSP()
	{
	  int Mnode;                                         // ��㣬��ǰ������ĸ��ڵ�
	  int h,i,k,l,m,n,nn;
	  int x,y=0;
	  int arry2[Max]={0,0, 0,0, 0, 0,0, 0,0, 0, 0,0, 0,0, 0, 0,0, 0,0, 0,0}; // ��־λ���飬����������0��δ��������1
	  double temp1=100,temp2=100;
	  double layer1[Max];                                      // ��ʼ����ǰ������ڵ�
	  double layer2[Max];                                      // ��ʼ�����������ڵ�
	
	  printf("\n������Ҫ�����ĳ��и�����");
	  scanf("%d",&cn);
	  printf("\n");
	
	  printf("������Ҫ�����ĳ��У�\n");
	  for(h=1;h<=cn;h++)                  // ���������ڵ�
	  {
	    scanf("%d",&x);
		if(0==arry2[x]) arry2[x]=1;   // �����ظ�
		else if(1==arry2[x]) h=h-1;
	  }
	
	  printf("\n");
	  for(i=1;i<Max;i++)                  // ��ʾ�����ڵ�
	     if(1==arry2[i]) 
		    printf("%d ",i);
	  printf("\n");
	
	  printf("������������У�");         // ���������
	  scanf("%d",&start); 
	  printf("\n");
	
	  arry2[start]=0;                         // ��ʼ��
	  arry3[1]=start;
	  arry3[cn+1]=start;
	  Mnode=arry3[1];
	
	//for(i=1;i<Max;i++) printf("%d ",arry2[i]);    // ��ʾarry2[]00000000000000000000000000000000
	// printf("\n");
	
	 ///////////////////////////////////////////////////////////////////////////////////// ����·��
	for(n=2;n<=cn;n++)                       // �ҳ�����2��cn
	{
	  for(nn=1;nn<Max;nn++)                  // ��ʼ��layer1[]��layer2[]
	  {
		  layer1[nn]=0;
	      layer2[nn]=0;
	  }
	
	  for(k=1;k<Max;k++)                    // ����Mnode���к�̽ڵ�                   
	     if(1==arry2[k]) 
		 {  
			gn=g1+arry1[Mnode][k];
			hn=arry1[k][start];
			fn=gn+hn;
	        layer1[k]=fn;
		 }
	
	  for(l=1;l<Max;l++)                   // �ҳ���һ����̽ڵ�y������ʼ��y
	    if(0!=layer1[l]) 
		{
		  y=l;
		  break;
		} 
	
	// for(i=1;i<Max;i++) printf("%d ",arry2[i]);    // ��ʾarry2[]00000000000000000000000000000000
	// printf("\n");
	
	  for(m=y+1;m<Max;m++)  //�����������������������������Ƚϲ�������Ѻ��
	  { 
		if(layer1[y]==layer1[m]) //////////////// �������������̴�����ͬ���������ǵ���һ��ڵ�
		{
		  Mnode=y;                        ////// ����yΪ���ڵ�
	      arry2[y]=0;                     
		  for(k=1;k<Max;k++)                   // ����y�����к�̽ڵ�
		     if(1==arry2[k]) 
		     {
		  	   gn=g1+arry1[Mnode][k];
		 	   hn=arry1[k][start];
			   fn=gn+hn;
	           layer2[k]=fn;
		     }
		  for(l=1;l<Max;l++)                  // �ҳ�y�Ĵ�����С�ĺ�̽ڵ�
	        if(0!=layer2[l]&&temp1>layer2[l]) 	temp1=layer2[l]; 
	
		  for(nn=1;nn<Max;nn++)    layer2[nn]=0;   // ��ʼ��layer2[]
	
		  Mnode=m;                       ////// ����mΪ���ڵ�
	      arry2[y]=1;
	      arry2[m]=0;                     
		  for(k=1;k<Max;k++)                   // ����m�����к�̽ڵ�
		      if(1==arry2[k]) 
		     {
		  	   gn=g1+arry1[Mnode][k];
		 	   hn=arry1[k][start];
			   fn=gn+hn;
	           layer2[k]=fn;
		     }
		  for(l=1;l<Max;l++)                  // �ҳ�m�Ĵ�����С�ĺ�̽ڵ�
	          if(0!=layer2[l]&&temp2>layer2[l])	  temp2=layer2[l]; 
	
		  arry2[y]=1;    arry2[m]=1;         // �������������¶�arry2[]ֵ�ĸı�
	
		  if (temp1>temp2)  y=m; 
		}
	    else if(0!=layer1[m]&&layer1[y]>layer1[m])  y=m; 
	  }                      //���������������������������� �Ƚϲ�������Ѻ��
	
	 //for(i=1;i<Max;i++) printf("%d ",arry2[i]);    // ��ʾarry2[]00000000000000000000000000000000
	 //printf("\n");
	
	 g1=g1+arry1[Mnode][y];
	 Mnode=y; 
	 arry2[y]=0;
	 arry3[n]=y;
	 }
	 //for(i=1;i<Max;i++) printf("%d ",arry2[i]);    // ��ʾarry2[]00000000000000000000000000000000
	 //  printf("\n");
	
	fn=g1+arry1[y][start];
	}///////////////////////////////////////////////////////////////////////////////////// ����·��
