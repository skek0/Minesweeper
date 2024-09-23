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
	// 가로 세로 입력받고 보드 생성
	int sizeX, sizeY;
	printf("size : ");
	scanf_s("%d", &sizeX);
	scanf_s("%d", &sizeY);
	system("cls");
	maxX = sizeX; maxY = sizeY;
	BoardMaker(sizeX, sizeY);

	// 키 입력
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
		printf("★");
	}
	return 0;
}

void Position(int x, int y)
{
	// X와 Y축을 설정하는 구조체
	COORD position = { x,y };

	// 콘솔 커서의 좌표를 설정하는 함수입니다.
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
					printf("□ ");
				}
				else
				{
					printf("■ ");
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
	1. 시작과 끝
	보드 크기, 폭탄 수 설정
	~게임 중~
	폭탄 클릭시 게임 오버
	보드 내 클릭하지 않은 칸이 폭탄의 수와 같다면 클리어

	2. 게임에서 필요한 기능
	보드의 가로 세로를 받아 보드를 생성하기
	WASD로 이동, J로 선택, K로 폭탄 마크와 ? 표기
	"첫 선택시" 폭탄의 위치를 정하기
	선택한 블럭의 인접한 8칸 내 폭탄의 총 갯수를 블럭의 위치에 대체
	인접한 블럭 내 폭탄이 0개일시 인접한 모든 블록을 선택한것으로 간주
	폭탄 선택시 게임오버
	선택되지 않은 블럭의 수가 폭탄의 수와 같다면 승리

	3. 필요한 기능을 추상화

	4. 코드로 옮기기


*/