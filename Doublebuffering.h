#pragma once
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>

int screenIndex; // -> 0���� �ʱ�ȭ��
HANDLE screen[2];

void Initialize();

void Flip();

void Clear();

void Release();

void Render(int x, int y, const char* string);

void Position(int x, int y);