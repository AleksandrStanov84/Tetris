#include "pch.h"
//#pragma once
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <ctime>
#include <cstdlib>
#include <random>
//#include "src_sample/ConsolEngine/PerformanceCounter.h"
//#include "src_sample/ConsolEngine/BaseApp.h"

using namespace std;

enum MyEnum
{
	KEY_UP = 72, //вверх
	KEY_DOWN = 80, //вниз
	KEY_LEFT = 75, //влево
	KEY_RIGHT = 77, //вправо
	KEY_SPACE = 32, //пробел
	KEY_ESC = 27, //escape
	KEY_ENTER = 13 //enter
};

class Tetris// :public BaseApp
{
public://private:
	const char SCR_SP = (char)183; //закрашивание поля
	const char SCR_OB = (char)2; //закрашивание фигуры
	static const int FigurMap_Count = 7; //количество фигур тетриса
	static const int SIZE_X = 15; //-ширина поля
	static const int SIZE_Y = 20; //-длина поля

	int pX, pY, score, nextMap;

	int screen[SIZE_X][SIZE_Y] = { 0 }; //размер экрана
	int map[4][4]; //фигура
	const int fMap[FigurMap_Count][4][4] = //инициализация фигур тетриса
	{
		{
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		{
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0}
		},
		{
			{0, 0, 1, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		{
			{1, 1, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		{
			{0, 1, 1, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		{
			{1, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},
		{
			{1, 1, 1, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		}
	};

	Tetris()
	{
		int pX(0);
		int pY(0);
		int score(0);
		int nextMap(0);
	}
	void StartGame();

	void GotoXY(int xpos, int ypos); //помещения курсора в точку xpos, ypos	

	void Print(); //печать

	void PrintNextMap(); //напечатать следующую идущую фигуру в поле

	int getKey();//-пауза во время игры

	bool ValNewPos(int x, int y); //необходимость движения фигуры вниз

	int inv(int x) { return 3 - x; }

	void MoveMap(); //поворот фигуры

	int Rnd(int max); //рандомизация

	void Sleep(int milsec); //задержка

	void DeleteLine(); //убить собранную линию

	void CreateMap(); //создание фигуры

	void ClearScreen(); //очистка экрана

	void CreateRndScreen(); //создать случайный экран

	~Tetris()
	{
		cout << "\tПопробуйте еще!" << endl;
	}
};

void Tetris::GotoXY(int xpos, int ypos) //помещения курсора в точку xpos, ypos
{
	ShowCursor(false);
	COORD scrn;
	HANDLE hStdOut/*hOuput*/ = GetStdHandle(STD_OUTPUT_HANDLE);//создание "ручки" для вывода на экран консоли текста и прочего 
	scrn.X = xpos; scrn.Y = ypos;//-присваем значения
	SetConsoleCursorPosition(hStdOut, scrn);//-ставим курсор,на нужную позицию.
}

void Tetris::Print()//печать
{
	int i, j;
	int buff[SIZE_X][SIZE_Y];

	for (i = 0; i < SIZE_Y; i++)
		for (j = 0; j < SIZE_X; j++)
			buff[j][i] = screen[j][i]; //вывод из буфера на экран
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (map[j][i]) buff[j + pX][i + pY] = 1; //если часть фигуры, то в буфере обозначить 1

	GotoXY(0, 0); //перейти в точку с координатами 0, 0
	for (i = 0; i < SIZE_Y; i++)
	{
		for (j = 0; j < SIZE_X; j++)
		{
			putchar(buff[j][i] == 0 ? SCR_SP : SCR_OB); //если элемент в буфере не часть фигуры, то закрасить полем
		}
		putchar('\n');
	}

	GotoXY(SIZE_X + 1, 0); //уйти вбок
	cout << "Колличество очков:  " << score; //вывести количество очков, набираемое в ходе игры
}

void Tetris::PrintNextMap() //напечатать следующую идущую фигуру в поле
{
	GotoXY(SIZE_X + 1, 2);
	cout << "СЛЕДУЮЩАЯ ФИГУРА: \n" << endl;

	for (int i = 0; i < 4; i++)
	{
		GotoXY(SIZE_X + 2, i + 3);
		for (int j = 0; j < 4; j++)
		{
			putchar(fMap[nextMap][j][i] == 0 ? ' ' : SCR_OB); //если не часть фигуры, то закрасить пробелами, иначе закрасить фигурой
		}
	}
}

int Tetris::getKey()//-пауза во время игры
{
	int c;
	if (_kbhit()) //если нажата клавиша
	{
		if ((c = _getch()) == 224)
			c = _getch();
		//если нажата клавиша p (пауза), то нажать любую клавишу для продолжения игры
		return c;
	}
	return false;
}

bool Tetris::ValNewPos(int x, int y) //необходимость движения фигуры вниз
{
	if (x < 0)
		return false;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (map[j][i])
			{
				if ((j + x >= SIZE_X) || (i + y >= SIZE_Y))
					return false;//-проверка не зашла ли фигура за границы нашего экрана.
				if (screen[j + x][i + y])
				{
					return false;//-если да, то вернем "фолс"
				}
			}
		}
	}
	return true;//-если нет, возвращаем "тру" - фигура движется вниз.
}

void Tetris::MoveMap() //поворот фигуры
{
	int _map[4][4];
	int sx = 4, sy = 4;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			_map[j][i] = map[j][i];//-создае копию фигуры,для того что бы вернуть первоначальный вид.
			if (map[j][i])//цикл с условием ЕСЛИ - фигура равна нулю, то 
			{
				if (i < sx)
					sx = i;//проходим по фигурке запоминаем новую координату х
				if (inv(j) < sy)
					sy = inv(j);//новую координату у
			}
			map[j][i] = 0;//-обнуляем матрицу с фигурой
		}

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (_map[inv(i)][j])
				map[j - sx][i - sy] = 1;//рисуем перевертышь
	if (!ValNewPos(pX, pY))
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				map[j][i] = _map[j][i];//возвращаем обратно на изначальную фигурку
}

int Tetris::Rnd(int max) //рандомизация
{
	max++;
	srand(time(NULL));
	return rand() % max;
}

void Tetris::Sleep(int milsec) //задержка
{
	clock_t t = clock();
	while (clock() - t < milsec);
}

void Tetris::DeleteLine() //убить собранную линию
{
	int i, j, k, cl;

	for (i = SIZE_Y - 1; i >= 0; i--)
	{
		cl = 1;
		for (j = 0, cl = 1; j < SIZE_X; j++)
		{
			if (!screen[j][i]) cl = 0;
		}
		if (cl)
		{
			GotoXY(0, i);
			for (k = 0; k < SIZE_X; k++) putchar('_'), Sleep(20);

			score += (((i * (-1)) + SIZE_Y) * 10);

			for (k = i; k > 0; k--)
			{
				for (j = 0; j < SIZE_X; j++)
				{
					screen[j][k] = screen[j][k - 1];
				}
			}
			i++;
			Print();
		}
	}
}

void Tetris::CreateMap() //создание фигуры
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			map[j][i] = fMap[nextMap][j][i];
	pY = 0;
	pX = SIZE_X / 2;

	nextMap = Rnd(FigurMap_Count - 1);
	PrintNextMap();
}

void Tetris::ClearScreen() //очистка экрана
{
	for (int i = 0; i < SIZE_Y; i++)
		for (int j = 0; j < SIZE_X; j++)
			screen[j][i] = 0;
}

void Tetris::CreateRndScreen() //создать случайный экран
{
	ClearScreen();
	int rn = Rnd(10);
	for (int i = SIZE_Y - 1; i >= (SIZE_Y - 1) - rn; i--)
		for (int j = 0; j < SIZE_X; j++)
		{
			screen[j][i] = Rnd(1);
		}
}

void Tetris::StartGame() //игра
{
	time_t tm;

	system("cls");
	pX = SIZE_X / 2;//значение для падения фигуры по х, выход из центра
	pY = 0;
	score = 0;

	tm = clock();

	nextMap = Rnd(FigurMap_Count - 1);//следующая фигура выбирается из 6 
	CreateMap();//создание фигурки

	while (true)
	{
		int c = getKey();

		switch (c)
		{
		case KEY_UP:
			MoveMap();//поворот
			break;

		case KEY_SPACE:
			MoveMap();//поворот
			break;

		case KEY_DOWN://движение вниз
			for (; ValNewPos(pX, pY + 1); pY++);
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					if (map[j][i])
						screen[pX + j][pY + i] = 1;

			Print();
			DeleteLine();//проход по функции проверки заполнилась ли линия и её удаление
			CreateMap();//новая фигура
			break;
		case KEY_LEFT:
			if (ValNewPos(pX - 1, pY))
				pX--;//движение влево ,уменьшаем значение пера по х
			break;

		case KEY_RIGHT:
			if (ValNewPos(pX + 1, pY))
				pX++;//движение вправо ,увеличиваем значение пера по х
			break;

		case 'p'://пауза
			_getch();
			break;

		case KEY_ESC://выход
			return;
		}

		if ((clock() - tm) > 720)//-время падения фигуры 
		{
			tm = clock();

			if (!(ValNewPos(pX, pY + 1)))
			{
				for (int i = 0; i < 4; i++)
					for (int j = 0; j < 4; j++)
						if (map[j][i])
							screen[pX + j][pY + i] = 1;
				CreateMap();
				DeleteLine();
			}
			else
				pY++;//падение вниз
		}

		Print();

		for (int i = 0; i < SIZE_X; i++)
		{
			if (screen[i][0])//если экранный у и у фигуры совпали и равны 0,то конец игры ,поле заполнилось до верха
			{
				system("cls");
				GotoXY(2, 8);
				cout << "ВЫПРОИГРАЛИ!\n" << endl;
				return;
			}
		}
	}
}


int main()
{
	setlocale(LC_ALL, "Russian");
	system("color 0F");

	Tetris t;
	t.StartGame();
}
