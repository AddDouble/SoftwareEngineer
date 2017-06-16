#include <iostream>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <vector>
#include <string>
#include  <fstream>
HANDLE  hThreads[8];
#define step 0.01
using namespace std;
struct put_Thread_Parameter
{
	float vec_x;
	float vec_y;
	float vec_z;
	put_Thread_Parameter(float v_x, float v_y, float v_z)
	{
		vec_x = v_x;
		vec_y = v_y;
		vec_z = v_z;

	}
};//线程结构体
put_Thread_Parameter ptp_array[8] = { { 1,1,1 },{ 1,1,-1 },{ 1,-1,1 },{ 1,-1,-1 },
{ -1,1,1 },{ -1,1,-1 },{ -1,-1,1 },{ -1,-1,-1 } };    //线程参数
struct ball  //结构体表示球
{
	float x;
	float y;
	float z;
	float r;
	ball(float num1, float num2, float num3, float radio)
	{
		x = num1;
		y = num2;
		z = num3;
		r = radio;
	}
	ball(void) {};
};
ball Ballvec[100];           //保存已经有效的球
int current_ball = 0;        //当前已经放入的ball的数目
ball maxBall_Threads(0, 0, 0, 0);    //用于记录八个线程中的最大球
vector<ball> probably_balls;      //存储每个线程找到的最大球。对称的最大球可以直接加入,减少运算
float sum = 0;        //sum r^3
CRITICAL_SECTION cs;
float inline pow2_distance(const ball& b1, const  ball& b2);  //返回平方后的球中心距离
bool push_block(float x, float y, float z);      //放入block
bool judge(const ball& b);  //判断该球是否合法;
DWORD WINAPI put_Thread(LPVOID lpt);    //放球的线程函数
int n;                     //放置球的数量
void input_n();
int m;                     //放置tiny_block的数量
void input_m();
void print_result();   //打印结果
//void draw();
int main()
{
	InitializeCriticalSection(&cs);
	input_n();
	input_m();
	time_t begin_time = time(NULL);
	while (n>0)
	{
		for (int i = 0; i < 8; i++)
		{
			hThreads[i] = CreateThread(NULL, NULL, put_Thread, LPVOID(&ptp_array[i]), NULL, NULL);
		}
		//八个线程从正方体的八个顶点开始遍历搜索,搜索的区域为起始点到(0,0,0)点的小正方形
		WaitForMultipleObjects(8, hThreads, true, INFINITE);
		if (judge(maxBall_Threads))
		{
			Ballvec[current_ball++] = maxBall_Threads;
			n--;
			for (int i = 0; i <8; i++)   //遍历可能的球，找到半径与目前最大球的半径相同的球，加入ballvec
			{
				if (abs(maxBall_Threads.r - probably_balls[i].r)<0.0001&&judge(probably_balls[i]))
				{
					Ballvec[current_ball++] = probably_balls[i];
					n--;
					if (n <= 0)
						goto show;
				}
			}
			probably_balls.clear();  //重置
			maxBall_Threads = ball(0, 0, 0, 0);
		}
	}
	time_t end_time = time(NULL);
	cout << end_time - begin_time << endl;
show:
	print_result();
	//draw();
	system("pause");
	return 0;
}
float inline pow2_distance(const ball& b1, const  ball& b2)
{
	float x1 = b1.x;
	float x2 = b2.x;
	float y1 = b1.y;
	float y2 = b2.y;
	float z2 = b2.z;
	float z1 = b1.z;
	return (z2 - z1) * (z2 - z1) + (y2 - y1) * (y2 - y1) + (x2 - x1)*(x2 - x1);
}
bool push_block(float x, float y, float z)
{
	if (abs(x) > 1 || abs(y) > 1 || abs(z)>1)
		return false;
	Ballvec[current_ball++] = ball(x, y, z, 0);
	return true;
}

bool judge(const ball&  b)  //判断该球是否合法
{
	if ((abs(b.x) + b.r)> 1 || (abs(b.y) + b.r) > 1 || (abs(b.z) + b.r)>1)
		return false;
	for (int i = 0; i < current_ball; i++)
	{
		ball temp = Ballvec[i];
		if (pow2_distance(temp, b) < (b.r + temp.r)*(b.r + temp.r))
			return false;
	}
	return true;  //返回true的情况只有所有球都相切或者相离
}

DWORD WINAPI put_Thread(LPVOID lpt)
{
	put_Thread_Parameter  *parameters = (put_Thread_Parameter*)lpt;
	float vec_x = parameters->vec_x;
	float vec_y = parameters->vec_y;
	float vec_z = parameters->vec_z;
	ball newone((-vec_x + step*vec_x), (-vec_y + step*vec_y), (-vec_z + step*vec_z), 0);
	ball maxBall = newone;
	ball begin_Ball = newone;
	while (abs(newone.x - begin_Ball.x)<(1 + step))   //对x,y,r都进行以step为步数的搜索，直到与链表中的球相切
	{
		newone.x += step*vec_x;
		newone.y = -vec_y + step*vec_y;
		while (abs(newone.y - begin_Ball.y)<(1 + step))
		{
			newone.y += step*vec_y;
			newone.z = -vec_z + step*vec_z;
			while (abs(newone.z - begin_Ball.z)<(1 + step))
			{
				newone.z += step*vec_z;
				newone.r = 0;
				float rstep = 0.1;
				while (rstep > 0.0001)
				{
					if (newone.r > maxBall.r)
						maxBall = newone;
					newone.r += rstep;
					if (!judge(newone))
					{
						newone.r -= rstep;
						rstep /= 10;
					}
				}
			}
		}
	}
	EnterCriticalSection(&cs);
	probably_balls.push_back(maxBall);
	if (maxBall.r>maxBall_Threads.r)
		maxBall_Threads = maxBall;
	LeaveCriticalSection(&cs);
	return 0;
}
void input_n()
{
	cout << "输入放置圆的数量" << endl;
	cin >> n;
}
void input_m()
{
	cout << "输入放置tiny blocks的数量" << endl;
	cin >> m;
	while (m--)
	{
		cout << "输入点坐标(x,y,z)" << endl;
		float x, y, z;
		cin >> x >> y >> z;
		if (push_block(x, y, z) == false)
		{
			cout << "非法重新输入" << endl;
			m++;
		}
	}
}
void print_result()
{
	ofstream ofs("C:\\Users\\刘竞\\Desktop\\result.dat");//保存到文件中
	string result_str;
	int j = 0;
	for (int i = 0; i < current_ball; i++)
	{
		auto part = Ballvec[i];
		if (part.r > 0)  //输入时只输出球而不输出blocks
		{
			j++;
			sum += part.r*part.r*part.r;
			cout << "第" << j << "个球的坐标(" << part.x << ", " << part.y << ", " << part.z << ") 半径:" << part.r << endl;
			ofs << part.x << " ";
			ofs << part.y << " ";
			ofs << part.z << " ";
			ofs << part.r << "\n";
		}
	}
	cout << "sum r^3：" << sum << endl;
}
//void draw()
//{
//	LPTSTR path = L"D:\\Matlab\\bin\\matlab.exe";
//	LPTSTR commandLine = L"draw.m";
//	STARTUPINFO si = { 0 };
//	PROCESS_INFORMATION pi;
//	CreateProcess(path, commandLine, NULL, NULL, false, NULL, NULL, NULL, &si, &pi);
//}