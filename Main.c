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

int isFirstHit = 1;
int mineAmount;

int StartScreen();

void Position(int x, int y);
void BoardMaker(int lengthX, int lengthY);
int CalculateIndex(int x, int y);
void LayMines(int amount);
void Select(int posX, int posY);
int CheckNearby(int posX, int posY);
void RenderBoard();
int CheckCondition(int amount);
void Mark(int posX, int posY);
void Finished(int cleared);

void PrintAnswer();

int main()
{
	if (!StartScreen())
	{
		return;
	}
	
	system("cls");


	// 가로 세로 입력받고 보드 생성
	int sizeX, sizeY;
	printf("size : ");
	scanf_s("%d %d", &sizeX, &sizeY);
	printf("\nmines : ");
	scanf_s("%d", &mineAmount);

	system("cls");
	maxX = sizeX; maxY = sizeY;
	BoardMaker(sizeX+2, sizeY+2);
	//LayMines(mineAmount);
	// 키 입력
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
				Select((posX + 1) / 2, posY);
				break;

			case MARK:
				if (!isFirstHit)
				{
					Mark((posX + 1) / 2, posY);
				}
				break;

			default:
				break;
			}

			system("cls");
			if (CheckCondition(mineAmount) == 1)
			{
				Finished(1);
				break;
			}
			RenderBoard();
		}
		Position(0,maxY+2);
		PrintAnswer();
		Position(posX, posY);
		printf("★");
	}

	free(board);

	return 0;
}

int StartScreen()
{
	printf("WASD to move\n");
	printf("J to select, K to mark\n");

	Position(2, 3);
	printf("Start");
	Position(2, 4);
	printf("Quit");

	int key = 0;
	int start = 0, game = 1;

	Position(0, 3);
	printf("▶");

	while (!start)
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
				Position(0, 4);
				printf("  ");
				Position(0, 3);
				printf("▶");
				game = 1;
				break;
			case DOWN:
				Position(0, 3);
				printf("  ");
				Position(0, 4);
				printf("▶");
				game = 0;
				break;
			case SELECT:
				start = 1;
				break;

			default:
				break;
			}
		}
	}
	return game;
}
void Position(int x, int y)
{
	// X와 Y축을 설정하는 구조체
	COORD position = { x,y };

	// 콘솔 커서의 좌표를 설정하는 함수입니다.
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

int CalculateIndex(int x, int y)
{
	int result = (maxX+2) * y + x;

	return result;
}
void BoardMaker(int lengthX, int lengthY)
{
	//이동 범위는 1 ~ length-2
	board = malloc(sizeof(int) * lengthX * lengthY);

	if (board != NULL)
	{
		for (int j = 0; j < lengthY; j++)
		{
			for (int i = 0; i < lengthX; i++)
			{
				if (i * j == 0 || j == lengthY - 1 || i == lengthX - 1)
				{
					board[CalculateIndex(i, j)] = 10;	// 가장자리
				}
				else
				{
					board[CalculateIndex(i, j)] = 11;	// 보드
				}
			}
		}

		for (int j = 0; j < lengthY; j++)
		{
			for (int i = 0; i < lengthX; i++)
			{
				if (board[CalculateIndex(i, j)] == 10)
				{
					printf("■ ");
				}
				else
				{
					printf("□ ");
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
		int num = rand() % ((maxX + 2) * (maxY + 2));

		while (board[num] != 11)
		{
			num = rand() % ((maxX + 2) * (maxY + 2));
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
		if (isFirstHit)
		{
			board[CalculateIndex(posX, posY)] = 12;
			LayMines(mineAmount);
			isFirstHit = 0;
			board[CalculateIndex(posX, posY)] = 11;
		}
		nearBombs = CheckNearby(posX, posY);
		printf("%d", nearBombs);
		board[CalculateIndex(posX, posY)] = nearBombs;
		if (nearBombs == 0)
		{
			for (int j = -1; j <= 1; j++)
			{
				for (int i = -1; i <= 1; i++)
				{
					Select(posX + i, posY + j);
				}
			}
		}
		break;
	case 12:
		system("cls");
		Finished(0);
		break;
	}
}
int CheckNearby(int posX, int posY)
{
	int count = 0;
	for (int j = -1; j <= 1; j++)
	{
		for (int i = -1; i <= 1; i++)
		{
			int value = board[CalculateIndex(posX + i, posY + j)];
			if (value == 12 ||
				value == 14 ||
				value == 16)
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
					printf("■ ");
					break;
				case 11:
				case 12:
					printf("□ ");
					break;
				case 13:
				case 14:
					printf("▣ ");
					break;
				case 15:
				case 16:
					printf("? ");
					break;
				case 0:
					printf("  ");
					break;
				default:
					printf("%d ", value);
					break;
				}
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
			if (board[CalculateIndex(i, j)] == 11 ||
				board[CalculateIndex(i, j)] == 13 || 
				board[CalculateIndex(i, j)] == 15)
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

void Finished(int cleared)
{
	if (cleared)
	{
		printf("CONGRATULATIONS!");
	}
	else
	{
		printf("Failed..");
	}

	Position(2, 4);
	printf("Restart");
	Position(2, 5);
	printf("Quit");
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

void PrintAnswer()
{
	for (int j = 0; j < maxY+2; j++)
	{
		for (int i = 0; i < maxX+2; i++)
		{
			printf("%d ",board[CalculateIndex(i, j)]);
		}
		printf("\n");
	}
}