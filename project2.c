#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef int bool;
#define false 0  
#define true  1 

//
typedef struct ball

{
	double x;       //圆心x坐标
	double y;		//圆心y坐标
	double r;		//圆半径
}Ball;

//用来储存已经放置的气球

typedef struct ballList

{
	struct ballList * next;
	Ball ball;
}BallList;
BallList *head = NULL;
double step = 0.01;							//改变气球位置的最小单位
int num = 0;                                //气球个数
double sumr = 0;							//r^2之和

void insert(Ball ball)  //插入气球

{

	struct ballList * newBall = (struct ballList *)malloc(sizeof(struct ballList));
	newBall->ball = ball;
	newBall->next = head;
	head = newBall;
}						
double distance(Ball b1, Ball b2)     //气球之间距离

{
	double x1 = b1.x;
	double y1 = b1.y;
	double x2 = b2.x;
	double y2 = b2.y;
	return pow((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2), 0.5);
}          

bool judge(Ball b)  //判断新加入的气球是否符合规则
{
	if ((fabs(b.x) + b.r) > 1 || (fabs(b.y) + b.r) > 1)
	{
		return false;
	}
	struct ballList *tmp = head;
	while (tmp)
	{
		Ball ball = tmp->ball;
		if (distance(b, ball) < b.r + ball.r)
		{
			return false;
		}
		tmp = tmp->next;
	}
	return true;
}							
void putBall()   //改变气球的初始位置，求的满足条件的气球
{
	Ball ball = { -1 + step, -1 + step, 0 };
	Ball maxBall = ball;
	int I, j;
	for (I = 0; ball.x < 1; ++I)
	{
		ball.x += step;
		ball.y = -1 + step;
		for (j = 0; ball.y < 1; ++j)
		{
			ball.y += step;
			ball.r = 0;
			double rstep = 0.1;
			while (rstep > 0.00001)
			{
				if (ball.r > maxBall.r)
				{
					maxBall = ball;
				}
				ball.r += rstep;
				if (!judge(ball))
				{
					ball.r -= rstep;
					rstep /= 10;
				}
			}
		}
	}
	if (judge(maxBall))
	{
		insert(maxBall);
		num++;
		sumr += maxBall.r * maxBall.r;
		printf("%d\t %.3lf\t %.3lf\t %.3lf\t %lf \n", num, maxBall.x, maxBall.y, maxBall.r, sumr);
	}
}								
void putBlock(double x, double y,double r)   //放置障碍

{
	Ball ball = { x, y, r };
	insert(ball);
}			

void  main()

{
	int n, pointNum, I;
	printf("请输入内所要放置的圆数量: ");
	scanf_s("%d", &n);
	printf("请输入障碍圆的数量: ");
	scanf_s("%d", &pointNum);

	for (I = 0; I < pointNum; I++)
	{
		printf("请输入第%d个障碍圆的坐标以及半径(其间用空格隔开)：", I + 1);
		double x, y, r;
		scanf_s("%lf %lf %lf", &x, &y, &r);
		putBlock(x, y, r);
	}
	printf("\n圆编号\t x坐标\t y坐标\t 半径\t r^2之和\n");
	for (I = 0; I < n; I++)
	{
		putBall();
	}
	printf("\nr^2之和为:\t %lf\n", sumr);
	//return 0;
	getch();
}













