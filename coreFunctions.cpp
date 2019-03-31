#include"game.h"
using namespace std;
/*
有雷自动结束
*/

//全局变量声明
typedef struct
{
	int x;
	int y;
}node;

queue<node> q1;
queue<node> q2;
queue<node> q3;
queue<node> q4;
double counts[1000];
int icount = 0;
int icount2 = 0;
int hitNum;
IMAGE img[30];
int  mine_num =30;//雷的数量
int row = 20;
int column = 20;//游戏等级 简单 中等 难
int map[50][50];
double pro[50][50];
int cell_size = 20;//方块大小
int offset = 60;



//游戏初始化
void Gameinit(int map_i,int map_j)
{
	//所需参数初始化
	hitNum = 0;

	//初始化数组
	for (int i = 0; i < map_i + 2; i++)
	{
		for (int j = 0; j < map_j + 2; j++)
		{
			map[i][j] = 0;
		}
	}
	//地雷的随机数种子
	srand((unsigned int)time(NULL));
	//放置地雷
	Put_mine(map_i, map_j, mine_num);
	
	//初始化地图最后一步，计算每个格子周围有多少雷
	for (int i = 1; i <= map_i; i++)
	{
		for (int j = 1; j <= map_j; j++)
		{
			if (map[i][j] == 0)//不是地雷
			{
				for (int s = i - 1; s <= i + 1; s++)
				{
					for (int x = j - 1; x <= j + 1; x++)
					{
						if (map[s][x] == -1)
						{
							map[i][j]++;
						}
					}
				}
			}
		}
	}


	//打印map
	//加载图片
	//地雷及数字
	loadimage(&img[0], "./image/0.gif", cell_size, cell_size);
	loadimage(&img[1], "./image/1.gif", cell_size, cell_size);
	loadimage(&img[2], "./image/2.gif", cell_size, cell_size);
	loadimage(&img[3], "./image/3.gif", cell_size, cell_size);
	loadimage(&img[4], "./image/4.gif", cell_size, cell_size);
	loadimage(&img[5], "./image/5.gif", cell_size, cell_size);
	loadimage(&img[6], "./image/6.gif", cell_size, cell_size);
	loadimage(&img[7], "./image/7.gif", cell_size, cell_size);
	loadimage(&img[8], "./image/8.gif", cell_size, cell_size);
	loadimage(&img[9], "./image/blood.gif", cell_size, cell_size);
	loadimage(&img[10], "./image/blank.gif", cell_size, cell_size);
	loadimage(&img[11], "./image/flag.gif", cell_size, cell_size);
	//背景
	loadimage(&img[12], "./image/0.gif", cell_size*row, cell_size*2);
	//模式选择
	loadimage(&img[13], "./image/5.png", cell_size, cell_size);
	loadimage(&img[14], "./image/6.png", cell_size, cell_size);
	loadimage(&img[15], "./image/7.png", cell_size, cell_size);
	loadimage(&img[16], "./image/8.png", cell_size*2, cell_size);

	//笑脸嘲讽
	loadimage(&img[17], "./image/face0.gif", cell_size * 2, cell_size*2);
	loadimage(&img[18], "./image/face2.gif", cell_size * 2, cell_size * 2);
	loadimage(&img[19], "./image/face3.gif", cell_size * 2, cell_size * 2);
	loadimage(&img[20], "./image/face4.gif", cell_size * 2, cell_size * 2);
	//雷是否标记对
	loadimage(&img[21], "./image/mine.gif", cell_size , cell_size );
	loadimage(&img[22], "./image/mine1.gif", cell_size, cell_size);
	loadimage(&img[23], "./image/mine2.gif", cell_size, cell_size);
    //切换模式
	loadimage(&img[24], "./image/mode.png", cell_size*4, cell_size);






}

//图形界面绘制地图
/*
-1 8 盖上
9  雷出现
10 18出现 数字出现
19 28
19 雷标记
20 28数字标记
*/
void DrawMap(int map_i,int map_j)
{
	//模式选择
	putimage(0, 0, &img[13]);
	putimage(cell_size*2, 0, &img[14]);
	putimage(cell_size*4, 0, &img[15]);
	putimage(cell_size * 6, 0, &img[16]);
	putimage(cell_size*row-cell_size *4, 0, &img[24]);
	//笑脸嘲讽
	for (int i =0; i < row*cell_size; i += 2*cell_size)
	{
		putimage(i, cell_size, &img[17]);
	}

	for (int i = 1; i <= map_i; i++)
	{
		for (int j = 1; j <= map_j; j++)
		{

			if (map[i][j] > 9 && map[i][j] <= 18)//因为雷的问题，不能等于9
			{
				putimage((i - 1) * cell_size, (j - 1) * cell_size + offset, &img[map[i][j] - 10]);
			}
			else if (map[i][j] == 9)
			{
				putimage((i - 1) * cell_size, (j - 1) * cell_size + offset, &img[9]);
			}
			else if (map[i][j] >= 19)
			{
				putimage((i - 1) * cell_size, (j - 1) * cell_size + offset, &img[11]);
			}
			else
			{
				putimage((i - 1) * cell_size, (j - 1) * cell_size +offset, &img[10]);
			}
		}
	}
}

//开始游戏
int PlayGame()
{
	int mine = 0;
	MOUSEMSG msg = { 0 };
	msg = GetMouseMsg();
	switch (msg.uMsg)
	{
	case WM_LBUTTONDOWN:
		taunt();
		if (msg.y - offset > 0)
		{
			if (map[msg.x / cell_size + 1][(msg.y - offset) / cell_size + 1] <= 9 && map[msg.x / cell_size + 1][(msg.y - offset) / cell_size + 1] != 0)
			{
				map[msg.x / cell_size + 1][(msg.y - offset) / cell_size + 1] += 10;
				mine = map[msg.x / cell_size + 1][(msg.y - offset) / cell_size + 1];
				hitNum++;
			}
			else if (map[msg.x / cell_size + 1][(msg.y - offset) / cell_size + 1] == 0)
			{
				diffusion(msg.x / cell_size + 1, (msg.y - offset) / cell_size + 1);
			}
		}
		else if(msg.x >cell_size*4 &&msg.x <cell_size*5 &&msg.y>0 && msg.y<cell_size)
		{
			mine = HARD;
		}
		else if (msg.x > 0 && msg.x < cell_size  && msg.y>0 && msg.y < cell_size)
		{
			mine = EASY;
		}
		else if (msg.x > cell_size * 2 && msg.x < cell_size*3  && msg.y>0 && msg.y < cell_size)
		{
			mine = NORMAL;
		}
		else if (msg.x > cell_size*6 && msg.x < cell_size*8  && msg.y>0 && msg.y < cell_size)
		{
			mine = HELL;
		}
		else if (msg.x > cell_size * row-cell_size*4 && msg.x < cell_size * row && msg.y>0 && msg.y < cell_size)
		{
			//TODO:
			mine = PC_P;
		}
		break;
	case WM_RBUTTONDOWN:
		if (msg.y - offset > 0)
		{
			if (map[msg.x / cell_size + 1][(msg.y - offset) / cell_size + 1] <= 9)
			{
				map[msg.x / cell_size + 1][(msg.y - offset) / cell_size + 1] += 20;
			}
			else if (map[msg.x / cell_size + 1][(msg.y - offset) / cell_size + 1] >= 19)
			{
				map[msg.x / cell_size + 1][(msg.y - offset) / cell_size + 1] -= 20;
			}
		}
		break;

	}
	return mine;
}
//0的自动扩散
void diffusion(int x, int y)
{
	map[x][y] += 10;
	hitNum++;
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (i > 0 && i < row+1 && j >0 && j < column+1)
			{
				if (map[i][j] < 9)
				{
					if (map[i][j] != 0)
					{
						map[i][j] += 10;
						hitNum++;
					}
					else
					{
						diffusion(i, j);
					}
				}
			}
		}
	}
}

//游戏开始方法，入口
void GameStart()
{
	int check;
	while (1)
	{
		initgraph(cell_size*row, cell_size*column+offset);//初始化图形界面
		Gameinit(row,column);
		HWND hwnd = GetHWnd();
		check = Paint_loop(hwnd);
		if (check != EASY && check != NORMAL && check != HARD && check != HELL && MessageBox(hwnd, "再玩一局？", "", MB_RETRYCANCEL) == IDCANCEL)
		{
			break;
		}
		else if (check == EASY)//模式
		{
			row = 8;
			column = 8;
			cell_size = 20;
			offset = 60;
			mine_num = 2;
		}
		else if (check == NORMAL)
		{
			row = 14;
			column = 14;
			cell_size = 20;
			offset = 60;
			mine_num = 15;
		}
		else if (check == HARD)
		{
			row = 20;
			column = 20;
			cell_size = 20;
			offset =60;
			mine_num = 30;
		}
		else if (check == HELL)
		{
			row = 30;
			column = 30;
			cell_size = 20;
			offset = 60;
			mine_num =150;
		}
		else if (check == PC_P)
		{
			check = -10;
		}
	}
}

//游戏结束时，显示地图方法
void mapShow(int map_i,int map_j,int check)
{
	putimage(0, 0, &img[13]);
	putimage(cell_size * 2, 0, &img[14]);
	putimage(cell_size * 4, 0, &img[15]);
	putimage(cell_size * 6, 0, &img[16]);
	putimage(cell_size*row - cell_size * 4, 0, &img[24]);
	//嘲讽循环
	if (check == 9)
	{
		for (int i = 0; i < row*cell_size; i += 2 * cell_size)
		{
			putimage(i, cell_size, &img[19]);
		}
	}
	else
	{
		for (int i = 0; i < row*cell_size; i += 2 * cell_size)
		{
			putimage(i, cell_size, &img[20]);
		}
	}
	//显示全局
	for (int i = 1; i <= map_i; i++)
	{
		for (int j = 1; j <= map_j; j++)
		{
			if (map[i][j] < 9 && map[i][j] != -1)//非雷，未掀开的
			{
				putimage((i - 1) * cell_size, (j - 1) * cell_size+offset, &img[map[i][j]]);
			}
			else if (map[i][j] == -1)//雷，未掀开的
			{
				putimage((i - 1) * cell_size, (j - 1) * cell_size + offset, &img[21]);
			}else if(map[i][j] >=19)//标记的
			{
				map[i][j] -= 20;
				if (map[i][j] < 9 && map[i][j] != -1)//标记错的
				{
					putimage((i - 1) * cell_size, (j - 1) * cell_size + offset, &img[22]);
				}
				else if (map[i][j] == -1)//标记对的
				{
					putimage((i - 1) * cell_size, (j - 1) * cell_size + offset, &img[21]);
				}
			}
			else {
				map[i][j] -= 10;
				if (map[i][j] < 9 && map[i][j] != -1)
				{
					putimage((i - 1) * cell_size, (j - 1) * cell_size + offset, &img[map[i][j]]);
				}
				else if (map[i][j] == -1)
				{
					putimage((i - 1) * cell_size, (j - 1) * cell_size + offset, &img[9]);
				}
			}
		}
	}
}

void Put_mine(int map_i,int map_j,int num)
{
	int m, n;
	//放置地雷
	for (int i = 0; i < num; i++)
	{
		do {
			m = rand() % map_i + 1;
			n = rand() % map_j + 1;
		} while (map[m][n] != 0);
		map[m][n] = -1;//-1表示含有地雷
	
	}
}
int Paint_loop(HWND hwnd)
{
	//pc_play(row, column);
	int check;
	int pc_check = -1;
	while (1)
	{
		
		DrawMap(row, column);
		check = PlayGame();
		if (check == 9||pc_check == 9)
		{
			check = 9;
			mapShow(row, column,check);
			MessageBox(hwnd, "游戏结束", "", MB_OK);
			break;
		}
		else if (row*column - hitNum == mine_num)
		{
			mapShow(row, column,check);
			MessageBox(hwnd, "你赢了！", "", MB_OK);
			break;
		}
		else if (check == HARD)
		{
			break;
		}
		else if (check == EASY)
		{
			break;
		}
		else if (check == NORMAL)
		{
			break;
		}
		else if (check == HELL)
		{
			break;
		}
		else if (check == PC_P)
		{
			pc_check = pc_play(row, column);
		}
	}
	return check;
}
void taunt()
{
	for (int i = 0; i < row*cell_size; i += 2 * cell_size)
	{
		putimage(i, cell_size, &img[18]);
	}
	Sleep(100);
}

//电脑自动玩扫雷方法 初级版
/*
计算未翻开格子是地雷的概率
当随机翻开时，寻找是地雷概率最小的
为了动态演示，取消diffusion方法
掀开逻辑
只掀开为雷概率最小的

*/

int pc_play(int map_i, int map_j)
{
	int count = -1;
	int pc_check = -1;
	double select1 = mine_num / (row*column);//初始化时各点为雷概率
	//初始化棋盘为雷概率
	for (int i = 1; i <= map_i; i++)
	{
		for (int j = 1; j <= map_j; j++)
		{
			pro[i][j] = select1;
		}
	}
	//开局，随机选择一点掀开
	int x, y;
	x = rand() % map_i + 1;
	y = rand() % map_j + 1;
	while (map[x][y] != 0&& map[x][y] != -1)
	{
		hitNum++;
		map[x][y] += 10;
		x = rand() % map_i + 1;
		y = rand() % map_j + 1;
	}
	if (map[x][y] == 0)
	{
		pc_diffusion(x, y);
	}
	else if (map[x][y] == -1)
	{
		//TO DO
		pc_check = 9;
	}
	while (row*column - mine_num != hitNum&&count !=0&&pc_check!=9)
	{
		count = is_mine(row, column);
		is_zero(row, column);
		is_true(row, column);
	}
	int dont = is_mine(row, column);
	is_zero(row, column);
	is_true(row, column);
	return pc_check;
}
//电脑更直观显示的扩散方法
void pc_diffusion(int x, int y)
{
	map[x][y] += 10;
	hitNum++;
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (i > 0 && i < row + 1 && j >0 && j < column + 1)
			{
				if (map[i][j] < 9)
				{
					if (map[i][j] != 0)
					{
						map[i][j] += 10;
						hitNum++;
					}
					else
					{		
						DrawMap(row, column);
						Sleep(50);
						pc_diffusion(i, j);
					}
				}
			}
		}
	}
}
//插旗方法
int is_mine(int map_i, int map_j)//m,n为宫格大小
{
	int iicount = 0;
	for (int i = 1; i <= map_i; i++)
	{
		for (int j = 1; j <= map_j; j++)
		{
			if (map[i][j] - 10 < 9 && map[i][j] - 10 > 0)
			{
				node inode;
				inode.x = i;
				inode.y = j;
				q1.push(inode);
				q2.push(inode);
			
			}

		}
	}
	//遍历并标记必为雷的格
	while (!q1.empty())
	{
		float count = 0.0;//除法需要小数
		node imine = q1.front();
		q1.pop();
		//printf("%d........%d\n", imine.x, imine.y);
		int m = imine.x;
		int n = imine.y;
		for (int i = m-1; i <=m+1; i++)
		{
			for (int j = n-1; j <= n+1; j++)
			{
				if ((map[i][j] < 9||map[i][j]>18) && i>=1 && i<=row && j>=1 && j<=column)
				{
					count+=1;
				}
			}
		}
		counts[icount++] = count;

	
		
		for (int i = m - 1; i <= m + 1; i++)
		{
			for (int j = n - 1; j <= n + 1; j++)
			{
				if (map[i][j] < 9)
				{
					pro[i][j] = count/(map[m][n]-10);
					if (pro[i][j] == 1)
					{
						map[i][j] += 20;
						iicount++;
						DrawMap(row, column);
					}
				}
			}
		}
		//Sleep(50);
	}
	return iicount;
	}

//根据插旗后的布局，寻找雷的概率为0的位置
void is_zero(int map_i, int map_j)
{
	while (!q2.empty())
	{
		node imine = q2.front();
		q2.pop();
		int m = imine.x;
		int n = imine.y;
		int zero;
		//除去格子外有效的旗子
		for (int i = m - 1; i <= m + 1; i++)
		{
			for (int j = n - 1; j <= n + 1; j++)
			{
				if ((map[i][j]<9) && i >= 1 && i <= row && j >= 1 && j <= column)
				{
					counts[icount2]--;
				}
			}
		}
		//新建队列，里面是电脑下一次要选择的坐标
		for (int i = m - 1; i <= m + 1; i++)
		{
			for (int j = n - 1; j <= n + 1; j++)
			{
				if ((map[i][j] < 9) && i >= 1 && i <= row && j >= 1 && j <= column)
				{
					zero =counts[icount2] - (map[m][n]-10);
					if (zero == 0)
					{
						node izero;
						izero.x = i;
						izero.y = j;
						q3.push(izero);
					}

				}
			}
		}
		icount2++;
	}
	icount = 0;
	icount2 = 0;
}

//电脑自动扫雷（若不用继续猜的话）
void is_true(int map_i, int map_j)
{
	node itrue;
	int m, n;
	if (!q3.empty())
	{
		itrue = q3.front();
		q3.pop();
		m = itrue.x;
		n = itrue.y;
	}

	while (!q3.empty())
	{
		if (map[m][n] < 9&&map[m][n]!=0)
		{
			map[m][n] += 10;
			hitNum++;
			itrue = q3.front();
			m = itrue.x;
			n = itrue.y;
		}
		else if (map[m][n] == 0)
		{
			pc_diffusion(m, n);
		}
		{
			q3.pop();
			if (!q3.empty())
			{
				itrue = q3.front();
				m = itrue.x;
				n = itrue.y;
			}
		}
		DrawMap(row, column);

	}

}

//因为每个格都有可能为雷，概率可近似为一样，所以随机选择
int is_guess(int map_i, int map_j)
{
	for (int i = 1; i <= map_i; i++)
	{
		for (int j = 1; j <= map_j; j++)
		{
			if (map[i][j] < 9)
			{
				map[i][j] += 10;
				return map[i][j];
			}
		}
	}
}
