#include"game.h"
using namespace std;
/*
�����Զ�����
*/

//ȫ�ֱ�������
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
int  mine_num =30;//�׵�����
int row = 20;
int column = 20;//��Ϸ�ȼ� �� �е� ��
int map[50][50];
double pro[50][50];
int cell_size = 20;//�����С
int offset = 60;



//��Ϸ��ʼ��
void Gameinit(int map_i,int map_j)
{
	//���������ʼ��
	hitNum = 0;

	//��ʼ������
	for (int i = 0; i < map_i + 2; i++)
	{
		for (int j = 0; j < map_j + 2; j++)
		{
			map[i][j] = 0;
		}
	}
	//���׵����������
	srand((unsigned int)time(NULL));
	//���õ���
	Put_mine(map_i, map_j, mine_num);
	
	//��ʼ����ͼ���һ��������ÿ��������Χ�ж�����
	for (int i = 1; i <= map_i; i++)
	{
		for (int j = 1; j <= map_j; j++)
		{
			if (map[i][j] == 0)//���ǵ���
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


	//��ӡmap
	//����ͼƬ
	//���׼�����
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
	//����
	loadimage(&img[12], "./image/0.gif", cell_size*row, cell_size*2);
	//ģʽѡ��
	loadimage(&img[13], "./image/5.png", cell_size, cell_size);
	loadimage(&img[14], "./image/6.png", cell_size, cell_size);
	loadimage(&img[15], "./image/7.png", cell_size, cell_size);
	loadimage(&img[16], "./image/8.png", cell_size*2, cell_size);

	//Ц������
	loadimage(&img[17], "./image/face0.gif", cell_size * 2, cell_size*2);
	loadimage(&img[18], "./image/face2.gif", cell_size * 2, cell_size * 2);
	loadimage(&img[19], "./image/face3.gif", cell_size * 2, cell_size * 2);
	loadimage(&img[20], "./image/face4.gif", cell_size * 2, cell_size * 2);
	//���Ƿ��Ƕ�
	loadimage(&img[21], "./image/mine.gif", cell_size , cell_size );
	loadimage(&img[22], "./image/mine1.gif", cell_size, cell_size);
	loadimage(&img[23], "./image/mine2.gif", cell_size, cell_size);
    //�л�ģʽ
	loadimage(&img[24], "./image/mode.png", cell_size*4, cell_size);






}

//ͼ�ν�����Ƶ�ͼ
/*
-1 8 ����
9  �׳���
10 18���� ���ֳ���
19 28
19 �ױ��
20 28���ֱ��
*/
void DrawMap(int map_i,int map_j)
{
	//ģʽѡ��
	putimage(0, 0, &img[13]);
	putimage(cell_size*2, 0, &img[14]);
	putimage(cell_size*4, 0, &img[15]);
	putimage(cell_size * 6, 0, &img[16]);
	putimage(cell_size*row-cell_size *4, 0, &img[24]);
	//Ц������
	for (int i =0; i < row*cell_size; i += 2*cell_size)
	{
		putimage(i, cell_size, &img[17]);
	}

	for (int i = 1; i <= map_i; i++)
	{
		for (int j = 1; j <= map_j; j++)
		{

			if (map[i][j] > 9 && map[i][j] <= 18)//��Ϊ�׵����⣬���ܵ���9
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

//��ʼ��Ϸ
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
//0���Զ���ɢ
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

//��Ϸ��ʼ���������
void GameStart()
{
	int check;
	while (1)
	{
		initgraph(cell_size*row, cell_size*column+offset);//��ʼ��ͼ�ν���
		Gameinit(row,column);
		HWND hwnd = GetHWnd();
		check = Paint_loop(hwnd);
		if (check != EASY && check != NORMAL && check != HARD && check != HELL && MessageBox(hwnd, "����һ�֣�", "", MB_RETRYCANCEL) == IDCANCEL)
		{
			break;
		}
		else if (check == EASY)//ģʽ
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

//��Ϸ����ʱ����ʾ��ͼ����
void mapShow(int map_i,int map_j,int check)
{
	putimage(0, 0, &img[13]);
	putimage(cell_size * 2, 0, &img[14]);
	putimage(cell_size * 4, 0, &img[15]);
	putimage(cell_size * 6, 0, &img[16]);
	putimage(cell_size*row - cell_size * 4, 0, &img[24]);
	//����ѭ��
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
	//��ʾȫ��
	for (int i = 1; i <= map_i; i++)
	{
		for (int j = 1; j <= map_j; j++)
		{
			if (map[i][j] < 9 && map[i][j] != -1)//���ף�δ�ƿ���
			{
				putimage((i - 1) * cell_size, (j - 1) * cell_size+offset, &img[map[i][j]]);
			}
			else if (map[i][j] == -1)//�ף�δ�ƿ���
			{
				putimage((i - 1) * cell_size, (j - 1) * cell_size + offset, &img[21]);
			}else if(map[i][j] >=19)//��ǵ�
			{
				map[i][j] -= 20;
				if (map[i][j] < 9 && map[i][j] != -1)//��Ǵ��
				{
					putimage((i - 1) * cell_size, (j - 1) * cell_size + offset, &img[22]);
				}
				else if (map[i][j] == -1)//��ǶԵ�
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
	//���õ���
	for (int i = 0; i < num; i++)
	{
		do {
			m = rand() % map_i + 1;
			n = rand() % map_j + 1;
		} while (map[m][n] != 0);
		map[m][n] = -1;//-1��ʾ���е���
	
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
			MessageBox(hwnd, "��Ϸ����", "", MB_OK);
			break;
		}
		else if (row*column - hitNum == mine_num)
		{
			mapShow(row, column,check);
			MessageBox(hwnd, "��Ӯ�ˣ�", "", MB_OK);
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

//�����Զ���ɨ�׷��� ������
/*
����δ���������ǵ��׵ĸ���
���������ʱ��Ѱ���ǵ��׸�����С��
Ϊ�˶�̬��ʾ��ȡ��diffusion����
�ƿ��߼�
ֻ�ƿ�Ϊ�׸�����С��

*/

int pc_play(int map_i, int map_j)
{
	int count = -1;
	int pc_check = -1;
	double select1 = mine_num / (row*column);//��ʼ��ʱ����Ϊ�׸���
	//��ʼ������Ϊ�׸���
	for (int i = 1; i <= map_i; i++)
	{
		for (int j = 1; j <= map_j; j++)
		{
			pro[i][j] = select1;
		}
	}
	//���֣����ѡ��һ���ƿ�
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
//���Ը�ֱ����ʾ����ɢ����
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
//���췽��
int is_mine(int map_i, int map_j)//m,nΪ�����С
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
	//��������Ǳ�Ϊ�׵ĸ�
	while (!q1.empty())
	{
		float count = 0.0;//������ҪС��
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

//���ݲ����Ĳ��֣�Ѱ���׵ĸ���Ϊ0��λ��
void is_zero(int map_i, int map_j)
{
	while (!q2.empty())
	{
		node imine = q2.front();
		q2.pop();
		int m = imine.x;
		int n = imine.y;
		int zero;
		//��ȥ��������Ч������
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
		//�½����У������ǵ�����һ��Ҫѡ�������
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

//�����Զ�ɨ�ף������ü����µĻ���
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

//��Ϊÿ�����п���Ϊ�ף����ʿɽ���Ϊһ�����������ѡ��
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
