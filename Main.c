#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define UP 119
#define LEFT 97
#define RIGHT 100
#define DOWN 115

#define SELECT 106
#define MARK 107

int maxX, maxY;
char* board;
int mineAmount;

int isFirstHit = 1;
int play = 1;
int failed = 0;

void SetConsoleSize(int width, int height)
{
	char command[50];
	snprintf(command, sizeof(command), "mode con: cols=%d lines=%d", width, height);
	system(command);
}
int StartScreen();
void HowTo();

void Position(int x, int y);
void BoardMaker(int lengthX, int lengthY);
int CalculateIndex(int x, int y);
void LayMines(int amount);
void Select(int posX, int posY);
int CheckNearby(int posX, int posY);
int CheckNearbyMarks(int posX, int posY);
void ShowRemainingMines();
void RenderBoard();
void RenderChanged(int x, int y);
int CheckCondition(int amount);
void Mark(int posX, int posY);
int Finished(int cleared);
void Textcolor(int colorNum);

void PrintAnswer();

int main()
{
	SetConsoleSize(50, 25);

	if (!StartScreen())
	{
		return;
	}
	while (play)
	{
		system("cls");
		// 가로 세로 입력받고 보드 생성
		int sizeX, sizeY;
		Position(0, 0);
		printf("Length : ");
		scanf_s("%d", &sizeX);
		printf("\nWidth : ");
		scanf_s("%d", &sizeY);
		printf("\nmines : ");
		scanf_s("%d", &mineAmount);

		system("cls");
		maxX = sizeX; maxY = sizeY;
		BoardMaker(sizeX + 2, sizeY + 2);
		RenderBoard();

		// 키 입력
		char key = 0;

		int posX = 1, posY = 1;
		int priorX = 1, priorY = 1;
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
						priorY = posY;
						posY -= 1;
					}
					break;

				case LEFT:
					if (posX >= 2)
					{
						priorX = posX;
						posX -= 2;
					}
					break;

				case RIGHT:
					if (posX <= maxX * 2 - 2)
					{
						priorX = posX;
						posX += 2;
					}
					break;

				case DOWN:
					if (posY <= maxY - 1)
					{
						priorY = posY;
						posY += 1;
					}
					break;

				case SELECT:
					Select((posX + 1) / 2, posY);
					//릴리즈때 없앨 것ㅡㅡㅡㅡㅡㅡㅡ
					Position(0, maxY + 2);
					PrintAnswer();
					//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
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

				if (CheckCondition(mineAmount) == 1)
				{
					play = Finished(1);
					break;
				}
				else if (failed == 1)
				{
					play = Finished(0);
					break;
				}
				RenderBoard();
				RenderChanged((posX + 1) / 2, posY);
				ShowRemainingMines();
			}

			Position(priorX, priorY);
			printf(" ");
			Position(posX, posY);
			printf("▷");
		}
		free(board);
	}


	return 0;
}

int StartScreen()
{
	int returnTostartscreen = 1;
	int key = 0;
	int start = 0, game = 1;
	int posY = 3;

	while (returnTostartscreen) {
		returnTostartscreen = 0;
		start = 0;
		printf("WASD to move\n");
		printf("J to select, K to mark\n");

		Position(2, 3);
		printf("Start");
		Position(2, 4);
		printf("How To Play");
		Position(2, 5);
		printf("Quit");


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
					if (posY >= 4)
					{
						Position(0, posY);
						printf("  ");
						Position(0, posY - 1);
						printf("▶");
						posY--;
					}
					break;
				case DOWN:
					if (posY <= 4)
					{
						Position(0, posY);
						printf("  ");
						Position(0, posY + 1);
						printf("▶");
						posY++;
					}
					break;
				case SELECT:
					if (posY == 3) { game = 1; start = 1; }
					else if (posY == 4) { HowTo(); returnTostartscreen = 1; start = 1; posY = 3; }
					else if (posY == 5) { game = 0; start = 1; }
					break;

				default:
					break;
				}
			}
		}
	}
	return game;
}
void HowTo()
{
	system("cls");

	printf("Press J on □ to reveal\n");
	printf("Press K to mark for mines - ▣\n");
	printf("or leave it for later - ?\n\n");
	printf("Number displays how many mines around this tile\n");
	printf("Click on numbered tiles to reveal all approached tiles\n\n");

	Position(0, 8);
	printf("▶");
	Position(2, 8);
	printf("Back");
	int key;
	while (1)
	{
		if (_kbhit())
		{
			key = _getch();

			if (key == -32)
			{
				key = _getch();
			}
			if (key == SELECT)
			{
				system("cls");

				break;
			}
		}
	}
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
	int result = (maxX + 2) * y + x;

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
void LayMinesForTreasure(int amount)
{
	srand((unsigned int)time(NULL));


}
void Select(int posX, int posY)
{
	int nearBombs = 0;
	int index = CalculateIndex(posX, posY);
	switch (board[index])
	{
	case 11:
		if (isFirstHit)
		{
			board[index] = 12;
			LayMines(mineAmount);
			isFirstHit = 0;
			board[index] = 11;
		}
		nearBombs = CheckNearby(posX, posY);
		//printf("%d", nearBombs);
		board[index] = nearBombs;
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
		failed = 1;
		break;
	case 10:
	case 13:
	case 14:
	case 15:
	case 16:
		break;
	case 0:
		break;
	default:
		if (board[index] == CheckNearbyMarks(posX, posY))
		{
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					int value = board[CalculateIndex(posX + i, posY + j)];
					if (value == 11 || value == 12)
					{
						Select(posX + i, posY + j);
					}
				}
			}
		}
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
			if (value == 12 || value == 14 || value == 16)
			{
				count++;
			}
		}
	}
	return count;
}
int CheckNearbyMarks(int posX, int posY)
{
	int count = 0;
	for (int j = -1; j <= 1; j++)
	{
		for (int i = -1; i <= 1; i++)
		{
			int value = board[CalculateIndex(posX + i, posY + j)];
			if (value == 13 || value == 14)
			{
				count++;
			}
		}
	}
	return count;
}
void ShowRemainingMines()
{
	int count = 0;
	for (int j = 0; j < maxY + 2; j++)
	{
		for (int i = 0; i < maxX + 2; i++)
		{
			int value = board[CalculateIndex(i, j)];
			if (value == 13 || value == 14)
			{
				count++;
			}
		}
	}
	Position((maxX + 2) * 2, 0);
	Textcolor(15);
	printf("Mines : %d", mineAmount - count);
}
void RenderBoard()
{
	Position(0, 0);
	if (board != NULL)
	{
		for (int j = 0; j < maxY + 2; j++)
		{
			for (int i = 0; i < maxX + 2; i++)
			{
				int value = board[CalculateIndex(i, j)];
				switch (value)
				{
				case 10:
					Textcolor(9);
					printf("■ ");
					break;
				case 11:
				case 12:
					Textcolor(7);
					printf("□ ");
					break;
				case 13:
				case 14:
					Textcolor(12);
					printf("▣ ");
					break;
				case 15:
				case 16:
					Textcolor(8);
					printf("? ");
					break;
				case 0:
					printf("  ");
					break;
				default:
					Textcolor(15);
					printf("%d ", value);
					break;
				}
			}
			printf("\n");
		}
	}
}
void RenderChanged(int x, int y)
{
	int value = board[CalculateIndex(x, y)];
	switch (value)
	{
	case 10:
		Textcolor(9);
		printf("■ ");
		break;
	case 11:
	case 12:
		Textcolor(7);
		printf("□ ");
		break;
	case 13:
	case 14:
		Textcolor(12);
		printf("▣ ");
		break;
	case 15:
	case 16:
		Textcolor(8);
		printf("? ");
		break;
	case 0:
		printf("  ");
		break;
	default:
		Textcolor(15);
		printf("%d ", value);
		break;
	}
}
int CheckCondition(int amount)
{
	int flag = 0;
	for (int j = 0; j < maxY + 2; j++)
	{
		for (int i = 0; i < maxX + 2; i++)
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
int Finished(int cleared)
{
	system("cls");
	Position(0, 0);
	for (int j = 0; j < maxY + 2; j++)
	{
		for (int i = 0; i < maxX + 2; i++)
		{
			int value = board[CalculateIndex(i, j)];
			switch (value) {
			case 10:
				Textcolor(9);
				printf("■ ");
				break;
			case 11:
			case 15:
			case 0:
				printf("  ");
				break;
			case 13:
				Textcolor(12);
				printf("X ");
				break;
			case 12:
			case 14:
			case 16:
				Textcolor(12);
				printf("◈ ");
				break;
			default:
				Textcolor(15);
				printf("%d ", value);
				break;
			}
		}
		printf("\n");
	}

	Position(0, maxY + 4);
	Textcolor(1);
	printf("Press any key to Continue..");

	_getch();

	system("cls");
	Position(0, 0);
	Textcolor(15);

	if (cleared)
	{
		printf("CONGRATULATIONS!");
	}
	else
	{
		printf("Failed..");
	}

	isFirstHit = 1;
	failed = 0;

	Position(2, 4);
	printf("Restart");
	Position(2, 5);
	printf("Quit");

	int selected = 0, game = 1;
	int key;

	Position(0, 4);
	printf("▶");

	while (!selected)
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
				Position(0, 5);
				printf("  ");
				Position(0, 4);
				printf("▶");
				game = 1;
				break;
			case DOWN:
				Position(0, 4);
				printf("  ");
				Position(0, 5);
				printf("▶");
				game = 0;
				break;
			case SELECT:
				selected = 1;
				break;

			default:
				break;
			}
		}
	}
	return game;
}

void PrintAnswer()
{
	Textcolor(15);
	for (int j = 0; j < maxY + 2; j++)
	{
		for (int i = 0; i < maxX + 2; i++)
		{
			printf("%d ", board[CalculateIndex(i, j)]);
		}
		printf("       \n");
	}
}
void Textcolor(int colorNum)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

/*
쓰인 콘솔요소
	커서 위치 이동

쓰인 알고리즘?
	보물찾기 모드-
*/

//파스칼스네이크?