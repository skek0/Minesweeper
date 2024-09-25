#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include "Doublebuffering.h"
// �ϴ� ��ũ�� ó���ϸ� Ŭ�����
#define UP 119
#define LEFT 97
#define RIGHT 100
#define DOWN 115

#define SELECT 106
#define MARK 107

int maxX, maxY;
int* board;

int isFirstHit = 0;

void Position(int x, int y);
void BoardMaker(int lengthX, int lengthY);
int CalculateIndex(int x, int y);
void LayMines(int amount);
void Select(int posX, int posY);
int CheckNearby(int posX, int posY);
void RenderBoard();
int CheckCondition(int amount);
void Mark(int posX, int posY);

int main()
{
	// ���� ���� �Է¹ް� ���� ����
	int sizeX, sizeY;
	int mineAmount;
	printf("size : ");
	scanf_s("%d %d", &sizeX, &sizeY);
	printf("\nmines : ");
	scanf_s("%d", &mineAmount);

	system("cls");
	maxX = sizeX; maxY = sizeY;
	BoardMaker(sizeX+2, sizeY+2);
	LayMines(mineAmount);
	// Ű �Է�
	char key = 0;

	int posX = 1, posY = 1;

	Position(posX, posY);

	while (1)
	{
		if (_kbhit())
		{
			key = _getch();

			if (key == -32)
			{
				key = _getch();
			}

			switch (key)
			{
			case UP:
				if (posY > 1)
				{
					posY -= 1;
				}
				break;

			case LEFT:
				if (posX >= 2)
				{
					posX -= 2;
				}
				break;

			case RIGHT:
				if (posX <= maxX*2 - 2)
				{
					posX += 2;
				}
				break;

			case DOWN:
				if (posY <= maxY-1)
				{
					posY += 1;
				}
				break;

			case SELECT:
				Select((posX+1)/2, posY);
				break;

			case MARK:
				Mark((posX+1)/2, posY);
				break;

			default:
				break;
			}

			system("cls");
			if (CheckCondition(mineAmount) == 1)
			{
				printf("cleared");
				break;
			}
			RenderBoard();
		}
		Position(posX, posY);
		printf("��");
	}

	free(board);

	return 0;
}
// 0 : �����ڸ�		1 : ����		2 : ��ź

void Position(int x, int y)
{
	// X�� Y���� �����ϴ� ����ü
	COORD position = { x,y };

	// �ܼ� Ŀ���� ��ǥ�� �����ϴ� �Լ��Դϴ�.
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

int CalculateIndex(int x, int y)
{
	int result = (maxX+2) * y + x;

	return result;
}
void BoardMaker(int lengthX, int lengthY)
{
	//�̵� ������ 1 ~ length-2

	board = malloc(sizeof(int) * lengthX * lengthY);

	if (board != NULL)
	{
		for (int j = 0; j < lengthY; j++)
		{
			for (int i = 0; i < lengthX; i++)
			{
				if (i * j == 0 || j == lengthY - 1 || i == lengthX - 1)
				{
					board[CalculateIndex(i, j)] = 10;	// �����ڸ�
				}
				else
				{
					board[CalculateIndex(i, j)] = 11;	// ����
				}
			}
		}

		for (int j = 0; j < lengthY; j++)
		{
			for (int i = 0; i < lengthX; i++)
			{
				if (board[CalculateIndex(i, j)] == 10)
				{
					printf("�� ");
				}
				else
				{
					printf("�� ");
				}
			}
			printf("\n");
		}
	}
}
void LayMines(int amount)
{
	srand((unsigned int)time(NULL));

	for (int i = 0; i < amount; i++)
	{
		int num = rand() % (maxX+2 * maxY+2);

		while (board[num] != 11)
		{
			num = rand() % (maxX+2 * maxY+2);
		}
		board[num] = 12;
	}
}

void Select(int posX, int posY)
{
	int nearBombs;
	switch (board[CalculateIndex(posX, posY)])
	{
	case 11:
		nearBombs = CheckNearby(posX, posY);
		printf("%d", nearBombs);
		board[CalculateIndex(posX, posY)] = nearBombs;
		break;
	case 12:
		system("cls");
		printf("gameOver");
		break;
	}
}

int CheckNearby(int posX, int posY)
{
	int count = 0;
	for (int j = -1; j <= 1; j++)
	{
		for (int i = - 1; i <= 1; i++)
		{
			if (board[CalculateIndex(posX + i, posY + j)] == 12)
			{
				count++;
			}
		}
	}
	return count;
}

void RenderBoard()
{
	Position(0, 0);
	if (board != NULL)
	{
		for (int j = 0; j < maxY+2; j++)
		{
			for (int i = 0; i < maxX+2; i++)
			{
				int value = board[CalculateIndex(i, j)];
				switch (value)
				{
				case 10:
					printf("�� ");
					break;
				case 11:
				case 12:
					printf("�� ");
					break;
				case 13:
				case 14:
					printf("�� ");
					break;
				case 15:
				case 16:
					printf("? ");
					break;
				default:
					printf("%d ", value);
					break;
				}/*
				if (value == 10)
				{
					printf("�� ");
				}
				else if(value == 11 || value == 12)
				{
					printf("�� ");
				}
				else
				{
					printf("%d ", value);
				}*/
			}
			printf("\n");
		}
	}
}

int CheckCondition(int amount)
{
	int flag = 0;
	for (int j = 0; j < maxY+2; j++)
	{
		for (int i = 0; i < maxX+2; i++)
		{
			if (board[CalculateIndex(i, j)] == 11)
			{
				return 0;
			}
		}
	}
	return 1;
}

void Mark(int posX, int posY)
{
	switch (board[CalculateIndex(posX, posY)])
	{
	case 11:
		board[CalculateIndex(posX, posY)] = 13;
		break;
	case 12:
		board[CalculateIndex(posX, posY)] = 14;
		break;
	case 13:
		board[CalculateIndex(posX, posY)] = 15;
		break;
	case 14:
		board[CalculateIndex(posX, posY)] = 16;
		break;
	case 15:
		board[CalculateIndex(posX, posY)] = 11;
		break;
	case 16:
		board[CalculateIndex(posX, posY)] = 12;
		break;

	default:
		printf("error");
		break;
	}
}


/*
	1. ���۰� ��
	���� ũ��, ��ź �� ����
	~���� ��~
	��ź Ŭ���� ���� ����
	���� �� Ŭ������ ���� ĭ�� ��ź�� ���� ���ٸ� Ŭ����

	2. ���ӿ��� �ʿ��� ���
	������ ���� ���θ� �޾� ���带 �����ϱ�
	WASD�� �̵�, J�� ����, K�� ��ź ��ũ�� ? ǥ��
	"ù ���ý�" ��ź�� ��ġ�� ���ϱ�
	������ ���� ������ 8ĭ �� ��ź�� �� ������ ���� ��ġ�� ��ü
	������ �� �� ��ź�� 0���Ͻ� ������ ��� ����� �����Ѱ����� ����
	��ź ���ý� ���ӿ���
	���õ��� ���� ���� ���� ��ź�� ���� ���ٸ� �¸�

	3. �ʿ��� ����� �߻�ȭ

	4. �ڵ�� �ű��


*/