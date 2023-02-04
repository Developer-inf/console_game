/**************************************************************
*                 КАФЕДРА № 304 2 КУРС ООП                    *
*-------------------------------------------------------------*
* Project Type  : Win32 Console Application                   *
* Project Name  : OOPEx08_01                                  *
* File Name     : Source.CPP								  *
* Language      : C/C++                                       *
* Programmer(s) : Завтур П.С.                                 *
* Modified By   :                                             *
* Lit Source    : Шилдт Г. С++ самоучитель                    *
* Created       : 17/03/21                                    *
* Last Revision : 22/05/21                                    *
* Comment(s)    : Управление на клавишах стрелок              *
**************************************************************/

#include <iostream>			//ввод\ввывод
#include <windows.h>		//работа с windows
#include <ctime>			//работа со временем
#include <chrono>			//работа со временем системы
#include "Classes.h"		//классы
#include "GetConlWindow.h"	//указатель на консольное окно

#define KEY_DOWN(vk_code)	((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)	//макрос для определения нажатой кнопки
#define asteroidColor		RGB(131, 137, 150)	//цвет астероидов
#define starColor			RGB(255, 255, 255)	//цвет звезд
#define	normalRocketColor	RGB(255, 181, 0)	//стандартный цвет ракеты
#define	firstCollision		RGB(255, 92 , 0)	//цвет ракеты после 1 повреждения
#define secondCollision		RGB(255, 18 , 0)	//цвет аркеты после 2 

//объявление контекста устройства
HDC hdc;

//главная функция
int main()
{
	srand(time(0));				//установка сида для рандомных чисел
	int score = 0;				//кол-во очков
	int isInvisible = 0;		//неуязвимость
	int begin_time[2] = { 0 };	//счетчик времени для бонусов
	int speed = 10;				//скорость ракеты
	int flag = 0;				//флаг
	int score_mult = 1000;			//множитель очков
	int collisionCount = 0;		//кол-во жизней

	double velocity = 1;		//скорость падения астероидов
	double userVelocity = 1;	//ускорение астероидов

	wellRocketWithNoses welRoc(15, 50, normalRocketColor);	//целая ракета
	goodRocket godRoc(15, 50, firstCollision);				//ракета с 1 повереждением
	badRocket badRoc(15, 50, secondCollision);				//ракета с 2 повреждениями
	ARocket* rocket = &welRoc;								//ракета

	Bounty bounty[4];			//бонусы
	Asteroid astr[numAseroids];	//астероиды
	Asteroid stars[numStars];	//здезды

	//две переменные для счета времени
	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	//спавн бонусов
	for (size_t i = 0; i < 4; i++)
	{
		bounty[i].setFigure(i);
		bounty[i].move(rand() % (width - 30) + 30, -(rand() % 2 * height + height), i);
	}

	//спавн звезд
	for (size_t i = 0; i < numStars; i++)
	{
		stars[i].setRadius(1);													//установка радиуса здезды
		stars[i].move(rand() % width + 0, rand() % height + 0, asteroidColor);	//перемещение в неизвестном направлении
	}

	//спавн астероидов
	for (size_t i = 0; i < numAseroids; i++)
	{
		astr[i].setRadius(rand() % 50 + 10);									//установка радиуса здезды
		astr[i].move(rand() % width + 0, -(rand() % width + 0), asteroidColor);	//перемещение в неизвестном направлении
	}

	//ввод ускорения (сложности) пользователем
	std::cout << "Enter velocity (game difficulty, recomended - 10)\n";
	std::cin >> userVelocity;
	userVelocity = abs(userVelocity);
	if (userVelocity < 1) userVelocity = 1;
	system("cls");

	//получим дескриптор консольного окна
	HWND hwnd = GetConcolWindow();

	//если дескриптор существует - можем работать
	if (hwnd != NULL)
	{
		//получим контекст устройства для консольного окна
		hdc = GetWindowDC(hwnd);

		//если контекст существует - можем работать
		if (hdc != 0)
		{
			//пока не нажат ESC
			while (!KEY_DOWN(VK_ESCAPE))
			{
				//фиксируем время
				tp2 = std::chrono::system_clock::now();
				std::chrono::duration<float> elapsedTime = tp2 - tp1;
				tp1 = tp2;

				//stars
				for (size_t i = 0; i < numStars; i++)
				{
					//если звезды вышли за экран , спавним их заново
					if (stars[i].gety() > height)
						stars[i].move(stars[i].getx(), 0, starColor);

					//двигаем звезды вперед
					stars[i].move(stars[i].getx(), stars[i].gety() + 2, starColor);
				}

				//ateroids
				for (size_t i = 0; i < numAseroids; i++)
				{
					if (astr[i].gety() - astr[i].getRadius() < 800)
					{
						astr[i].move(astr[i].getx(), astr[i].gety() + velocity * 5, asteroidColor);
					}
					//если астероиды вышли за экран , спавним их заново
					else
					{
						score += astr[i].getRadius() * score_mult;	//обновляем счетчик очков
						astr[i].setRadius(rand() % 50 + 10);
						astr[i].move(rand() % width + 0, -(rand() % width + 0), asteroidColor);
					}
				}
				velocity += userVelocity * 0.0001;	//прибавляем скорость

				//bounty
				for (size_t i = 0; i < 4; i++)
				{
					//если бонусы вышли за экран , спавним их заново
					if (bounty[i].gety() > height + 100)
						bounty[i].move(rand() % (width - 30) + 30, -(rand() % 2 * height + height), i);
					//если столкнулись с бонусами
					else if (rocket->checkCollision(bounty[i]))
					{
						switch (i)
						{
						case 0:	//неуязвимость
							isInvisible = 1;
							begin_time[0] = clock() / CLK_TCK;
							break;

						case 1:	//ускорение
							flag = 1;
							speed += 7;
							begin_time[1] = clock() / CLK_TCK;
							break;

						case 2:	//множитель очков
							score_mult *= 2;
							break;

						case 3:	//потеря очков
							score /= 2;
						}
						//спавним бонус заново
						bounty[i].move(rand() % (width - 30) + 30, -(rand() % 2 * height + height), i);
					}
					else
						//просто двигаем бонус
						bounty[i].move(bounty[i].getx(), bounty[i].gety() + 3 + velocity, i);
				}

				rocket->hide();	//стираем ракету
				//если кнопка вправо
				if (KEY_DOWN(VK_RIGHT))
				{
					if (rocket->getRocDx() < width / 2 - rocket->getRocWidth() * 2)
						rocket->changeRocOffsetX(speed);
				}
				//если кнопка вправо
				else if (KEY_DOWN(VK_LEFT))
				{
					if (rocket->getRocDx() > -width / 2 + rocket->getRocWidth() * 2)
						rocket->changeRocOffsetX(-speed);
				}
				//если кнопка вправо
				if (KEY_DOWN(VK_UP))
				{
					if (rocket->getRocDy() > -height / 2 + rocket->getRocHeight())
						rocket->changeRocOffsetY(speed);
				}
				//если кнопка вправо
				else if (KEY_DOWN(VK_DOWN))
				{
					if (rocket->getRocDy() < height / 2 - rocket->getRocHeight())
						rocket->changeRocOffsetY(-speed);
				}

				//рисуем ракету
				rocket->show((isInvisible) ? RGB(0, 72, 186) : normalRocketColor);

				//game over
				if (!isInvisible)	//если уязвимый
				{
					for (size_t i = 0; i < numAseroids; i++)
					{
						//проверяем столкновение астероидов с ракетой
						if (rocket->checkCollision(astr[i]))
						{
							//количество жизней это число справа
							if (collisionCount < 2)
							{
								//при первом столконовении выбираем 2 вид ракеты
								if (collisionCount == 0)
									rocket = &godRoc;
								//при втором выбираем 3 вид ракеты
								else
									rocket = &badRoc;

								collisionCount++;
							}
							else
							{
								//вывод очков 
								std::cout << "	\t\t\t\t\tGame Over!\n"
												"Press R to restart!\n"
												"Press S to change difficulty!\n"
												"Press Esc to exit";

								//пока не нажата кнопка рестарта
								while (!KEY_DOWN('R'))
								{
									//если нажат ESC или пробел
									if (KEY_DOWN(VK_SPACE) || KEY_DOWN(VK_ESCAPE))
										return 0;

									//если нажата S , то меняем сложность
									if (KEY_DOWN('S'))
									{
										system("cls");
										std::cout << "Enter velocity (game difficulty)\n";
										std::cin >> userVelocity;
										break;
									}
								}

								score = 0;			//вовращаем к исходному значению кол-во очков
								score_mult = 1;		//вовращаем к исходному значению множитель очков
								collisionCount = 0;	//вовращаем к исходному значению счетчик столкновений
								velocity = 1;		//вовращаем к исходному значению скорось метеоритов

								rocket = &welRoc;	//выбираем целую ракету

								system("cls");	//очищаем экран
							}

							//заново спавним бонусы
							for (size_t i = 0; i < 4; i++)
								bounty[i].move(rand() % (width - 30) + 30, -(rand() % 2 * height + height), i);

							//заново спавним астероиды
							for (size_t i = 0; i < numAseroids; i++)
							{
								astr[i].setRadius(rand() % 50 + 10);
								astr[i].move(rand() % width + 0, -(rand() % width + 0), asteroidColor);
							}

							rocket->changeRocOffsetX(-rocket->getRocDx());	//вовращаем к исходному значению смещение х
							rocket->changeRocOffsetY(rocket->getRocDy());	//вовращаем к исходному значению смещение у

							system("cls");	//очищаем экран
							break;
						}
					}
				}

				//вывод на экран кол-ва и множителя очков
				printf("Score: %d | x%d | FPS: %.2lf\r", score, score_mult, 1.f / elapsedTime.count());

				//проверка бонусов на истекший срок годности
				if (clock() / CLK_TCK - begin_time[0] > 5)
					isInvisible = 0;

				if (clock() / CLK_TCK - begin_time[1] > 5)
					if (flag == 1)
					{
						speed -= 7;
						flag = 0;
					}
				
				//если набрано больше миллиона очков , то победа
				if (score > 1000000)
				{
					system("cls");
					std::cout << "\t\t\t Congratulations! You wasted your time successfully!\n";
					while (!KEY_DOWN(VK_SPACE) && !KEY_DOWN(VK_ESCAPE)) {}
					return 0;
				}
			}
		}
	}
}