#include<iostream>
#include<graphics.h>
#include<vector>
#include <conio.h>
#include<time.h>
#include<random>
#include<string>

using namespace std;
const int swidth = 1200;
const int sheight = 800;
const int step = 40;
const int a = 40;
const int interv = 200;

int ranw()
{
	int min = 0, max = swidth-a;
	random_device seed;//硬件生成随机数种子
	ranlux48 engine(seed());//利用种子生成随机数引擎
	uniform_int_distribution<> distrib(min, max);//设置随机数范围，并为均匀分布
	int random = distrib(engine);//随机数
	return random;

}
int ranh()
{
	int min = 0, max = sheight-a;
	random_device seed;//硬件生成随机数种子
	ranlux48 engine(seed());//利用种子生成随机数引擎
	uniform_int_distribution<> distrib(min, max);//设置随机数范围，并为均匀分布
	int random = distrib(engine);//随机数
	return random;
}

bool rdr(RECT r1, RECT r2)
{
	if (r1.left == r2.left && r1.top == r2.top)
		return 1;
	return 0;
}

bool clickinrect(int x, int y, RECT r)
{
	return(r.left <= x && r.right >= x && r.bottom >= y && r.top <= y);
}

void BK_DRAW()
{
	setcolor(LIGHTGRAY);
	for (int i = 0; i < swidth; i += a)
	{
		for (int j = 0; j < sheight; j += a)
		{
			rectangle(i, j, i + a, j + a);
		}
	}
	return;
}

void OVER(int score)
{
	TCHAR* str = new TCHAR[128];
	TCHAR s[] = _T("Press Enter To Restart");

	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);

	settextstyle(40, 20, _T("黑体"));
	_stprintf_s(str, 128, _T("炫了：%d粒"), score);
	outtextxy(swidth / 2 - textwidth(str) / 2, sheight / 5, str);

	settextstyle(40, 20, _T("黑体"));
	outtextxy(swidth / 2 - textwidth(s) / 2, sheight * 0.5, s);

	RECT texitr;
	LPCTSTR texit = _T("EXIT");
	settextstyle(60, 0, _T("黑体")); //文字高度 宽度 字体
	//exit 矩形坐标设置
	texitr.left = swidth / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = sheight / 1.5;
	texitr.bottom = texitr.top + textheight(texit);
	outtextxy(texitr.left, texitr.top, texit); //exit输出

	while (1)
	{
		ExMessage msg;
		peekmessage(&msg, EM_KEY);
		if (msg.vkcode == 0x0D)
		{
			return;
		}
		peekmessage(&msg,EM_MOUSE);
		if (msg.lbutton)
		{
			if (clickinrect(msg.x, msg.y, texitr))
			{
				exit(0);
			}	
		}
	}
}

class HEAD
{
public:
	HEAD():hr(hr),msg(msg)
	{
		hr.left = swidth/2;
		hr.right = hr.left + a;
		hr.top = sheight/2;
		hr.bottom = hr.top + a;
		hor = 0, ver = 0;
		upf = 1, downf = 1, leftf = 1, rightf = 1;
	}

	void move()
	{
		peekmessage(&msg, EM_KEY);
		{
			if (msg.vkcode == 0x28 && downf == 1)
			{
				hor = 0, ver = step;
				upf = 0, downf = 1, leftf = 1, rightf = 1;
			}
			else if (msg.vkcode == 0x26 && upf == 1)
			{
				hor = 0,ver = -step;
				upf = 1, downf = 0, leftf = 1, rightf = 1;
			}
			else if(msg.vkcode == 0x25 && leftf == 1)
			{
				hor = -step, ver = 0;
				upf = 1, downf = 1, leftf = 1, rightf = 0;
			}
			else if (msg.vkcode == 0x27 && rightf == 1)
			{
				hor = step, ver = 0;
				upf = 1, downf = 1, leftf = 0, rightf = 1;
			}
		}
		
		if (hr.left == 0 && hor < 0)
			hor = 0;
		if (hr.right == swidth && hor > 0)
			hor = 0;
		if (hr.top == 0 && ver < 0)
			ver = 0;
		if (hr.bottom == sheight && ver > 0)
			ver = 0;

		if (hor != 0 || ver != 0)
		{
			//清除痕迹
			setfillcolor(WHITE);
			fillrectangle(hr.left, hr.top, hr.right, hr.bottom);
		}
		//更新蛇头位置
		hr.left += hor, hr.right +=hor;
		hr.top += ver, hr.bottom +=ver;
		setfillcolor(RED);
		fillrectangle(hr.left, hr.top, hr.right, hr.bottom);
	}

	RECT returnrect()
	{
		return hr;
	}

	int returnver()
	{
		return ver;
	}

	int returnhor()
	{
		return hor;
	}

private:
	RECT hr;
	ExMessage msg;
	bool upf, downf, leftf, rightf;
	int hor,ver;
};

class BODY
{
public:
	BODY() :br(br)
	{
		
	}

	void addfirstbody(HEAD head)
	{
		int ver = head.returnver();
		int hor = head.returnhor();
		RECT hr = head.returnrect();

		br.left = hr.left - hor, br.right = br.left + a;
		br.top = hr.top - ver, br.bottom = br.top + a;
		setfillcolor(RED);
		fillrectangle(br.left, br.top, br.right, br.bottom);

		return;
	}
	
	
	void followhead(HEAD head)
	{

		//清除痕迹
		setfillcolor(WHITE);
		fillrectangle(br.left, br.top, br.right, br.bottom);

		int ver = head.returnver();
		int hor = head.returnhor();
		RECT hr = head.returnrect();

		br.left = hr.left - hor, br.right = br.left + a;
		br.top = hr.top - ver, br.bottom = br.top + a;
		setfillcolor(RED);
		fillrectangle(br.left, br.top, br.right, br.bottom);

		return;
	}

	void addbody(HEAD head,RECT prerect)
	{
		int ver = head.returnver();
		int hor = head.returnhor();

		br.left = prerect.left - hor, br.right = br.left + a;
		br.top = prerect.top - ver, br.bottom = br.top + a;
		setfillcolor(RED);
		fillrectangle(br.left, br.top, br.right, br.bottom);


		return;
	}

	void follow(RECT prerect)
	{
		//清除痕迹
		setfillcolor(WHITE);
		fillrectangle(br.left, br.top, br.right, br.bottom);

		//int ver = head.returnver();
		//int hor = head.returnhor();

		br.left = prerect.left, br.right = br.left + a;
		br.top = prerect.top, br.bottom = br.top + a;
		setfillcolor(RED);
		fillrectangle(br.left, br.top, br.right, br.bottom);

		return;
	}

	RECT returnrect()
	{
		return br;
	}

private:
	RECT br;
};

class FOOD
{
public:
	FOOD():is_exist(0),fr(fr)
	{

	}
	
	void add()
	{
		fr.left = ranw() % swidth;
		fr.top = ranh() % sheight;
		if (fr.left % a != 0) fr.left += a - fr.left % a;
		if (fr.top % a != 0) fr.top += a - fr.top % a;
		fr.right = fr.left + a;
		fr.bottom = fr.top + a;

		setfillcolor(YELLOW);
		fillrectangle(fr.left, fr.top, fr.right, fr.bottom);
		is_exist = 1;
	}

	bool exist()
	{
		if (is_exist==1) return 1;
		return 0;
	}

	bool eaten()
	{
		if (is_exist)
		{
			setfillcolor(WHITE);
			fillrectangle(fr.left, fr.top, fr.right, fr.bottom + a);
			is_exist = 0;
			return 1;
		}
		return 0;
	}
			

	RECT returnrect()
	{
		return fr;
	}

private:
	bool is_exist;
	RECT fr;
};

int main()
{	

	while (1)
	{
		vector<BODY> bodies;
		bool is_play=1;
		HEAD head;
		FOOD food;
		RECT prerect,temprect;
		
		flushmessage();

		initgraph(swidth, sheight);
		//蛇头初始化
		setfillcolor(RED);


		setbkcolor(WHITE);
		cleardevice();
		BK_DRAW();

		fillrectangle(swidth / 2, sheight / 2, swidth / 2 + a, sheight / 2 + a);
		int score = 0;

		while (is_play == 1)
		{

			head.move();

			if (rdr(food.returnrect(), head.returnrect()))
			{
				if (food.eaten())
				{
					bodies.push_back(BODY());
					if (bodies.size() == 1)
					{
						bodies[0].addfirstbody(head);
					}
					else
					{
						bodies[bodies.size() - 1].addbody(head, bodies[bodies.size() - 2].returnrect());
					}
					score++;
				}
			}

			//body跟随
			if (!bodies.empty())
			{
				temprect = bodies[0].returnrect();
				bodies[0].followhead(head);
				for (int i = 1; i < bodies.size(); i++)
				{
					prerect = temprect;
					temprect = bodies[i].returnrect();
					bodies[i].follow(prerect);
				}
			}

			//food添加
			if (!food.exist())
			{
				food.add();
			}

			//food被body覆盖检测与修复
			if (!bodies.empty())
			{
				int i;
				for (i = 0; i < bodies.size(); i++)
				{
					if (rdr(bodies[i].returnrect(), food.returnrect()))
					{
						break;
					}
				}
				if (i == bodies.size())
				{
					RECT fr = food.returnrect();
					setfillcolor(YELLOW);
					fillrectangle(fr.left, fr.top, fr.right, fr.bottom);
				}
			}

			for (int i = 0; i < bodies.size(); i++)
			{
				if (rdr(head.returnrect(), bodies[i].returnrect()))
				{
					is_play = 0;
					break;
				}
			}
			Sleep(interv);
		}
		OVER(score);

	}
	return 0;
}