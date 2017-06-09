#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef int bool;
#define false 0  
#define true  1 

//
typedef struct ball

{
	double x;       //Բ��x����
	double y;		//Բ��y����
	double r;		//Բ�뾶
}Ball;

//���������Ѿ����õ�����

typedef struct ballList

{
	struct ballList * next;
	Ball ball;
}BallList;
BallList *head = NULL;
double step = 0.01;							//�ı�����λ�õ���С��λ
int num = 0;                                //�������
double sumr = 0;							//r^2֮��

void insert(Ball ball)  //��������

{

	struct ballList * newBall = (struct ballList *)malloc(sizeof(struct ballList));
	newBall->ball = ball;
	newBall->next = head;
	head = newBall;
}						
double distance(Ball b1, Ball b2)     //����֮�����

{
	double x1 = b1.x;
	double y1 = b1.y;
	double x2 = b2.x;
	double y2 = b2.y;
	return pow((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2), 0.5);
}          

bool judge(Ball b)  //�ж��¼���������Ƿ���Ϲ���
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
void putBall()   //�ı�����ĳ�ʼλ�ã������������������
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
void putBlock(double x, double y,double r)   //�����ϰ�

{
	Ball ball = { x, y, r };
	insert(ball);
}			

void  main()

{
	int n, pointNum, I;
	printf("����������Ҫ���õ�Բ����: ");
	scanf_s("%d", &n);
	printf("�������ϰ�Բ������: ");
	scanf_s("%d", &pointNum);

	for (I = 0; I < pointNum; I++)
	{
		printf("�������%d���ϰ�Բ�������Լ��뾶(����ÿո����)��", I + 1);
		double x, y, r;
		scanf_s("%lf %lf %lf", &x, &y, &r);
		putBlock(x, y, r);
	}
	printf("\nԲ���\t x����\t y����\t �뾶\t r^2֮��\n");
	for (I = 0; I < n; I++)
	{
		putBall();
	}
	printf("\nr^2֮��Ϊ:\t %lf\n", sumr);
	//return 0;
	getch();
}













