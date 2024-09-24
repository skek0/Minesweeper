#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include "Doublebuffering.h"

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

				break;
			default:
				printf("Exception\n");
				break;
			}

			//system("cls");

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
					board[CalculateIndex(i, j)] = 0;	// �����ڸ�
				}
				else
				{
					board[CalculateIndex(i, j)] = 1;	// ����
				}
			}
		}

		for (int j = 0; j < lengthY; j++)
		{
			for (int i = 0; i < lengthX; i++)
			{
				if (board[CalculateIndex(i, j)] == 0)
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
		int num = rand() % (maxX * maxY);
		while (board[num] == 0 || board[num] == 2)
		{
			num = rand() % (maxX * maxY);
		}
		board[num] = 2;
	}
	for (int j = 0; j < maxY+2; j++)
	{
		for (int i = 0; i < maxX+2; i++)
		{
			printf("%d ", board[CalculateIndex(i, j)]);
		}
		printf("\n");
	}
}

void Select(int posX, int posY)
{
	switch (board[CalculateIndex(posX, posY)])
	{
	case 1:
		printf("%d", CheckNearby(posX, posY));
		break;
	case 2:
		printf("gameOver");
		break;
	}
}

// ���ľߴ�
int CheckNearby(int posX, int posY)
{
	int count = 0;
	for (int j = -1; j <= 1; j++)
	{
		for (int i = - 1; i <= 1; i++)
		{
			if (board[CalculateIndex(posX + i, posY + j)] == 2);
			{
				count++;
			}
		}
	}
	return count;
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