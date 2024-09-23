#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include "Doublebuffering.h"

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

int maxX, maxY;
int* board;

void Position(int x, int y);
void BoardMaker(int lengthX, int lengthY);
int CalculateIndex(int x, int y);

int main()
{
	// ���� ���� �Է¹ް� ���� ����
	int sizeX, sizeY;
	printf("size : ");
	scanf_s("%d", &sizeX);
	scanf_s("%d", &sizeY);
	system("cls");
	maxX = sizeX; maxY = sizeY;
	BoardMaker(sizeX, sizeY);

	// Ű �Է�
	char key = 0;

	int posX = 1, posY = 1;

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
				if (posY >= 1)
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
				if (posX <= 18)
				{
					posX += 2;
				}
				break;

			case DOWN:
				if (posY <= 9)
				{
					posY += 1;
				}
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
	return 0;
}

void Position(int x, int y)
{
	// X�� Y���� �����ϴ� ����ü
	COORD position = { x,y };

	// �ܼ� Ŀ���� ��ǥ�� �����ϴ� �Լ��Դϴ�.
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

void BoardMaker(int lengthX, int lengthY)
{
	board = malloc(sizeof(int) * lengthX * lengthY);

	if (board != NULL)
	{
		for (int j = 0; j < lengthY; j++)
		{
			for (int i = 0; i < lengthX; i++)
			{
				if (i * j == 0 || j == lengthY - 1 || i == lengthX - 1)
				{
					board[CalculateIndex(i, j)] = 0;
				}
				else
				{
					board[CalculateIndex(i, j)] = 1;
				}
			}
		}

		for (int j = 0; j < lengthY; j++)
		{
			for (int i = 0; i < lengthX; i++)
			{
				if (board[CalculateIndex(i, j)] == 1)
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
int CalculateIndex(int x, int y)
{
	int result = maxX * y + x;

	return result;
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