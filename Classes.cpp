#include <windows.h>	//работа с windows
#include "Classes.h"	//классы

extern HDC hdc;	//контекст устройства

//конструктор пикселя
Location::Location(int setx, int sety)
{
	x = setx;
	y = sety;
}

int Location::getx() { return x; }

int Location::gety() { return y; }

Location::~Location()
{

}

//конструктор объекта
ACircle::ACircle(int setx, int sety) : Location(setx, sety)
{

}

int ACircle::getRadius()
{
	return radius;
}

void ACircle::setRadius(int setradius)
{
	radius = setradius;
}

void ACircle::move(int tox, int toy, COLORREF rgb)
{
	hide();		//сделать точку невидимой
	x = tox;	//поменять координаты ТОЧКИ
	y = toy;
	show(rgb);		//показать точку на новом месте
}

ACircle::~ACircle()
{

}

//рисовка объекта
void Asteroid::show(COLORREF rgb)
{
	HPEN Pen = CreatePen(PS_SOLID, 2, rgb);	//зададим перо
	HBRUSH brush = CreateSolidBrush(rgb);	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активной

	// Нарисуем круг установленным цветом
	Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

	// Уничтожим нами созданные объекты  
	DeleteObject(Pen);
	DeleteObject(brush);
}

//спрятать объект
void Asteroid::hide()
{
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));	//зададим перо
	HBRUSH brush = CreateSolidBrush(RGB(12, 12, 12));	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активной

	// Нарисуем круг установленным цветом
	Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);
}

Asteroid::Asteroid(int setx, int sety, int setradius) : ACircle(setx, sety)
{
	radius = setradius;
}

Bounty::Bounty(int setx, int sety, int setradius) : ACircle(setx, sety)
{
	radius = setradius;
	figure = 0;
}

void Bounty::setFigure(int fgr)
{
	figure = fgr;
}

//рисуем бонус
void Bounty::show(COLORREF color)
{
	COLORREF color_1 = RGB(0, 0, 0);	//цвет 1
	COLORREF color_2 = RGB(0, 0, 0);	//цвет 2

	POINT point[3];	//точки для полигона

	//выбираем цает для фигуры
	switch (int(color))
	{
	case 0:
		color_1 = RGB(0, 72, 186);
		color_2 = RGB(242, 180, 0);
		break;
	case 1:
		color_1 = RGB(255, 225, 53);
		color_2 = RGB(128, 64, 64);
		break;
	case 2:
		color_1 = RGB(255, 219, 0);
		break;
	case 3:
		color_1 = RGB(127, 127, 127);
		color_2 = RGB(163, 73, 163);
	}

	HPEN Pen_1 = CreatePen(PS_SOLID, 2, color_1);
	HPEN Pen_2 = CreatePen(PS_SOLID, 2, color_2);
	HBRUSH brush = CreateSolidBrush(color_1);

	SelectObject(hdc, Pen_1);	//сделаем перо активным
	SelectObject(hdc, brush);

	//выбираем фигуру
	switch (figure)
	{
	case 0:	//неуязвимость
		point[0].x = x - radius * 0.3; point[0].y = y - radius * 0.7;
		point[1].x = x + radius * 0.3; point[1].y = y;
		Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
		SelectObject(hdc, Pen_2);
		Polygon(hdc, point, 2);	// \ 
		point[0].x = x - radius * 0.3; point[0].y = y;
		Polygon(hdc, point, 2);	// -
		point[1].x = x + radius * 0.3; point[1].y = y + radius * 0.7;
		Polygon(hdc, point, 2);	// \ 

		//вот что получится
		//	\
		// ---
		//  \

		break;

	case 1:	//ускорение
		point[0].x = x - radius * 0.7; point[0].y = y - radius * 0.3;
		point[1].x = x - radius * 0.3; point[1].y = y;
		point[2].x = x - radius * 0.7; point[2].y = y + radius * 0.3;
		Rectangle(hdc, x - radius, y - radius * 0.8, x + radius, y + radius * 0.8); //[]
		SelectObject(hdc, Pen_2);
		Polygon(hdc, point, 3);
		for (size_t i = 0; i < 3; i++)
			point[i].x += radius;
		Polygon(hdc, point, 3);	// >>

		//вот что получится
		//	[>>]

		break;

	case 2:	//очки * 2
		// <
		point[0].x = x - radius * 0.7; point[0].y = y;
		point[1].x = x; point[1].y = y - radius * 0.2;
		point[2].x = x; point[2].y = y + radius * 0.2;
		Polygon(hdc, point, 3);
		// >
		point[0].x = x + radius * 0.7;
		Polygon(hdc, point, 3);
		// ^
		point[0].x = x; point[0].y = y - radius * 0.7;
		point[1].x = x - radius * 0.2; point[1].y = y;
		point[2].x = x + radius * 0.2; point[2].y = y;
		Polygon(hdc, point, 3);
		// v
		point[0].x = x; point[0].y = y + radius * 0.7;
		Polygon(hdc, point, 3);

		//вот что получится
		//	^
		// < >
		//	v

		break;

	case 3:	//очки / 2
		Rectangle(hdc, x - radius * 0.7, y - radius * 0.8, x + radius * 0.7, y - radius * 0.6);	// -
		Rectangle(hdc, x - radius * 0.7, y + radius * 0.8, x + radius * 0.7, y + radius * 0.6);	// _
		SelectObject(hdc, Pen_2);
		brush = CreateSolidBrush(RGB(163, 73, 163));
		SelectObject(hdc, brush);
		Rectangle(hdc, x - radius * 0.5, y - radius * 0.6, x + radius * 0.5, y + radius * 0.6);	// |

		//вот что получится
		//	--
		//	||
		//	__
	}

	// Уничтожим нами созданные объекты
	DeleteObject(Pen_1);
	DeleteObject(Pen_2);
	DeleteObject(brush);
}

//стереть бонус
void Bounty::hide()
{
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));
	HBRUSH brush = CreateSolidBrush(RGB(12, 12, 12));

	SelectObject(hdc, Pen);	//сделаем перо активным
	SelectObject(hdc, brush);

	// Нарисуем прямоугольник установленным цветом
	Rectangle(hdc, x - radius, y - radius, x + radius, y + radius);

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);
}

ARocket::ARocket(int newWidth, int newHeight, COLORREF setColor) : ACircle(newWidth, newHeight)
{
	damageColor = setColor;
	rocWidth = newWidth;
	rocHeight = newHeight;
	points[0].x = width / 2 - rocWidth; points[0].y = height / 2;
	points[1].x = width / 2 + rocWidth; points[1].y = height / 2;
	points[2].x = width / 2;			points[2].y = height / 2 - rocWidth;
}

int ARocket::getRocWidth()
{
	return rocWidth;
}

int ARocket::getRocHeight()
{
	return rocHeight;
}

int ARocket::getRocDx()
{
	return dx;
}
int ARocket::getRocDy()
{
	return dy;
}

void ARocket::setDamageColor(COLORREF rgb)
{
	damageColor = rgb;
}

void ARocket::changeRocOffsetX(int val)
{
	dx += val;					//прибавляем смещение влево
	for (size_t i = 0; i < 3; i++)
		points[i].x += val;		//смещаем нос ракеты влево
}

void ARocket::changeRocOffsetY(int val)
{
	dy -= val;					//прибавляем смещение вверх
	for (size_t i = 0; i < 3; i++)
		points[i].y -= val;		//смещаем нос ракеты вверх
}

//рисовка компонентов ракеты
void ARocket::drawNose(COLORREF penclr)
{
	HPEN Pen = CreatePen(PS_SOLID, 2, penclr);		//зададим перо
	HBRUSH brush = CreateSolidBrush(damageColor);	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активным

	Polygon(hdc, points, 3);	//нарисуем нос ракеты

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);
}

void ARocket::drawRightTank(COLORREF penclr)
{
	HPEN Pen = CreatePen(PS_SOLID, 2, penclr);		//зададим перо
	HBRUSH brush = CreateSolidBrush(damageColor);	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активным

	// Нарисуем прямоугольник установленным цветом
	Rectangle(hdc,
		width / 2 - rocWidth + dx + 1 - rocWidth * 0.3,	//левая граница
		height / 2 + dy + rocHeight * 0.6,				//верхняя граница
		width / 2 - rocWidth + dx + 1,					//правая граница
		height / 2 + rocHeight + dy						//нижняя граница
	);

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);
}

void ARocket::drawLeftTank(COLORREF penclr)
{
	HPEN Pen = CreatePen(PS_SOLID, 2, penclr);		//зададим перо
	HBRUSH brush = CreateSolidBrush(damageColor);	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активным

	// Нарисуем прямоугольник установленным цветом
	Rectangle(hdc,
		width / 2 + rocWidth + dx + 1 + rocWidth * 0.3,	//левая граница
		height / 2 + dy + rocHeight * 0.6,				//верхняя граница
		width / 2 + rocWidth + dx + 1,					//правая граница
		height / 2 + rocHeight + dy						//нижняя граница
	);

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);
}

void ARocket::drawBody(COLORREF penclr)
{
	HPEN Pen = CreatePen(PS_SOLID, 2, penclr);		//зададим перо
	HBRUSH brush = CreateSolidBrush(damageColor);	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активным

	// Нарисуем прямоугольник установленным цветом
	Rectangle(hdc, width / 2 - rocWidth + dx + 1, height / 2 + dy, width / 2 + rocWidth + dx + 1, height / 2 + rocHeight + dy);

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);

	//границы пламени
	for (size_t i = 0; i < rocHeight * 0.125; i++)
	{
		SetPixel(hdc, width / 2 - rocWidth + dx, height / 2 + i + rocHeight + dy, RGB(255, 0, 0));
		SetPixel(hdc, width / 2 + rocWidth + dx, height / 2 + i + rocHeight + dy, RGB(255, 0, 0));
	}

	//рисуем 4 языка пламени у ракеты
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < rocWidth * 0.5; j++)
		{
			SetPixel(hdc, width / 2 - rocWidth + j / 2 + i * rocWidth * 0.5 + dx, height / 2 + rocHeight + rocWidth * 0.5 - j + dy, RGB(255, 0, 0));
			SetPixel(hdc, width / 2 - rocWidth + rocWidth * 0.25 + j / 2 + i * rocWidth * 0.5 + dx, height / 2 + rocHeight + j + dy, RGB(255, 0, 0));
		}
	}

	//ждем 10 секунд
	Sleep(10);

	//стираем их
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < rocWidth * 0.5; j++)
		{
			SetPixel(hdc, width / 2 - rocWidth + j / 2 + i * rocWidth * 0.5 + dx, height / 2 + rocHeight + rocWidth * 0.5 - j + dy, RGB(12, 12, 12));
			SetPixel(hdc, width / 2 - rocWidth + rocWidth * 0.25 + j / 2 + i * rocWidth * 0.5 + dx, height / 2 + rocHeight + j + dy, RGB(12, 12, 12));
		}
	}

	//рисуем 8 языков пламени у ракеты
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < rocWidth * 0.5; j++)
		{
			SetPixel(hdc, width / 2 - rocWidth + j / 4 + i * rocWidth * 0.25 + dx, height / 2 + rocHeight + rocWidth * 0.5 - j + dy, RGB(255, 0, 0));
			SetPixel(hdc, width / 2 - rocWidth + rocWidth * 0.125 + j / 4 + i * rocWidth * 0.25 + dx, height / 2 + rocHeight + j + dy, RGB(255, 0, 0));
		}
	}

	//ждем 10 секунд
	Sleep(10);

	//стираем их
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < rocWidth * 0.5; j++)
		{
			SetPixel(hdc, width / 2 - rocWidth + j / 4 + i * rocWidth * 0.25 + dx, height / 2 + rocHeight + rocWidth * 0.5 - j + dy, RGB(12, 12, 12));
			SetPixel(hdc, width / 2 - rocWidth + rocWidth * 0.125 + j / 4 + i * rocWidth * 0.25 + dx, height / 2 + rocHeight + j + dy, RGB(12, 12, 12));
		}
	}
}

void ARocket::hideNose()
{
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));	//зададим перо
	HBRUSH brush = CreateSolidBrush(RGB(12, 12, 12));	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активным

	// Нарисуем прямоугольник установленным цветом
	Polygon(hdc, points, 3);	//рисуем нос ракеты

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);
}

void ARocket::hideRightTank()
{
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));	//зададим перо
	HBRUSH brush = CreateSolidBrush(RGB(12, 12, 12));	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активным

	// Нарисуем прямоугольник установленным цветом
	Rectangle(hdc,
		width / 2 - rocWidth + dx + 1 - rocWidth * 0.3,	//левая граница
		height / 2 + dy + rocHeight * 0.6,				//верхняя граница
		width / 2 - rocWidth + dx + 1,					//правая граница
		height / 2 + rocHeight + dy						//нижняя граница
	);

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);
}

void ARocket::hideLeftTank()
{
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));		//зададим перо
	HBRUSH brush = CreateSolidBrush(RGB(12, 12, 12));	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активным

	// Нарисуем прямоугольник установленным цветом
	Rectangle(hdc,
		width / 2 + rocWidth + dx + 1 + rocWidth * 0.3,	//левая граница
		height / 2 + dy + rocHeight * 0.6,				//верхняя граница
		width / 2 + rocWidth + dx + 1,					//правая граница
		height / 2 + rocHeight + dy						//нижняя граница
	);

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);
}

void ARocket::hideBody()
{
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));	//зададим перо
	HBRUSH brush = CreateSolidBrush(RGB(12, 12, 12));	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активным

	// Нарисуем прямоугольник установленным цветом
	Rectangle(hdc, width / 2 - rocWidth + dx + 1, height / 2 + dy, width / 2 + rocWidth + dx + 1, height / 2 + rocHeight + dy);

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);

	for (size_t i = 0; i < rocHeight * 0.125; i++)
	{
		SetPixel(hdc, width / 2 - rocWidth + dx, height / 2 + i + rocHeight + dy, RGB(12, 12, 12));
		SetPixel(hdc, width / 2 + rocWidth + dx, height / 2 + i + rocHeight + dy, RGB(12, 12, 12));
	}
}

//КЛАСС wellRocketWithNoses
//конструктор
wellRocketWithNoses::wellRocketWithNoses(int setx, int sety, COLORREF setColor) : wellRocket(setx, sety, setColor)			//конструктор
{
}
//изменяем рисовку правого топливного бака
void wellRocketWithNoses::drawRightTank(COLORREF penclr)
{
	HPEN Pen = CreatePen(PS_SOLID, 2, penclr);		//зададим перо
	HBRUSH brush = CreateSolidBrush(damageColor);	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активным

	// Нарисуем прямоугольник установленным цветом
	Rectangle(hdc,
		width / 2 + rocWidth + dx + 1 + rocWidth * 0.3,	//правая граница
		height / 2 + dy + rocHeight * 0.6,				//верхняя граница
		width / 2 + rocWidth + dx + 1,					//левая граница
		height / 2 + rocHeight + dy						//нижняя граница
	);

	//кончик бака
	POINT tmp[3] =
	{
		{	width / 2 + rocWidth + dx + 1 + rocWidth * 0.4,	height / 2 + dy + rocHeight * 0.6	},	//правая нижняя  точка
		{	width / 2 + rocWidth + dx + 1,					height / 2 + dy + rocHeight * 0.6	},	//левая  нижняя  точка
		{	width / 2 + rocWidth + dx + 1,					height / 2 + dy + rocHeight * 0.4	}	//левая  верхняя точка
	};

	Polygon(hdc, tmp, 3);	//нарисуем кончик

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);
}

//скрываем правый
void wellRocketWithNoses::hideRightTank()
{
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));		//зададим перо
	HBRUSH brush = CreateSolidBrush(RGB(12, 12, 12));		//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активным

	// Нарисуем прямоугольник установленным цветом
	Rectangle(hdc,
		width	/ 2 + rocWidth + dx + 2 + rocWidth * 0.4,	//правая граница
		height	/ 2 + dy + rocHeight * 0.4,					//верхняя граница
		width	/ 2 + rocWidth + dx + 1,					//левая граница
		height	/ 2 + dy + rocHeight						//нижняя граница
	);

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);
}

//перерисовываем левый топливный бак
void wellRocketWithNoses::drawLeftTank(COLORREF penclr)
{
	HPEN Pen = CreatePen(PS_SOLID, 2, penclr);		//зададим перо
	HBRUSH brush = CreateSolidBrush(damageColor);	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активным

	// Нарисуем прямоугольник установленным цветом
	Rectangle(hdc,
		width / 2 - rocWidth + dx + 1 - rocWidth * 0.3,	//левая граница
		height / 2 + dy + rocHeight * 0.6,				//верхняя граница
		width / 2 - rocWidth + dx + 1,					//правая граница
		height / 2 + rocHeight + dy						//нижняя граница
	);

	//кончик бака
	POINT tmp[3] =
	{
		{	width / 2 - rocWidth + dx - 2 - rocWidth * 0.4,	height / 2 + dy + rocHeight * 0.6	},	//левая  нижняя  точка
		{	width / 2 - rocWidth + dx - 1,					height / 2 + dy + rocHeight * 0.6	},	//правая нижняя  точка
		{	width / 2 - rocWidth + dx - 1,					height / 2 + dy + rocHeight * 0.4	}	//правая верхняя точка
	};

	Polygon(hdc, tmp, 3);	//нарисуем кончик

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);
}

//скрываем левый топливный бак
void wellRocketWithNoses::hideLeftTank()
{
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));		//зададим перо
	HBRUSH brush = CreateSolidBrush(RGB(12, 12, 12));	//зададим кисть

	SelectObject(hdc, Pen);		//сделаем перо активным
	SelectObject(hdc, brush);	//сделаем кисть активным

	// Нарисуем прямоугольник установленным цветом
	Rectangle(hdc,
		width / 2 - rocWidth + dx - 2 - rocWidth * 0.4,	//левая граница
		height / 2 + dy + rocHeight * 0.4,				//верхняя граница
		width / 2 - rocWidth + dx + 1,					//правая граница
		height / 2 + rocHeight + dy						//нижняя граница
	);

	// Уничтожим нами созданные объекты
	DeleteObject(Pen);
	DeleteObject(brush);
}

//КЛАСС wellRocket
wellRocket::wellRocket(int setx, int sety, COLORREF setColor) : ARocket(setx, sety, setColor)			//конструктор
{
}
//нарисовать ракету
void wellRocket::show(COLORREF penClr)
{
	drawNose(penClr);
	drawRightTank(penClr);
	drawLeftTank(penClr);
	drawBody(penClr);
}

//стираем ракету
void wellRocket::hide()
{
	hideNose();
	hideRightTank();
	hideLeftTank();
	hideBody();
}

//функция проверки столкновения ракеты с астероидами
bool wellRocket::checkCollision(ACircle& astr)
{
	//находим ближайшую точку между центром астероида и ракетой
	int xn = max(width / 2 - rocWidth + dx, min(astr.getx(), width / 2 + rocWidth + dx));
	int yn = max(height / 2 + dy, min(astr.gety(), height / 2 + rocHeight + dy));

	//разность координат астероида и ближайшей точки
	int dlx = xn - astr.getx();
	int dly = yn - astr.gety();

	//по формуле круга узнаем соприкоснулись ли точки
	if ((dlx * dlx + dly * dly) < astr.getRadius() * astr.getRadius())
		return 1;

	//возьмем внутри треугольника маленький квадрат и сделаем ту же проверку
	xn = max(width / 2 - rocWidth / 2 + dx, min(astr.getx(), width / 2 + rocWidth / 2 + dx));
	yn = max(height / 2 - rocWidth / 2 + dy, min(astr.gety(), height / 2 + dy));

	dlx = xn - astr.getx();
	dly = yn - astr.gety();

	if ((dlx * dlx + dly * dly) < astr.getRadius() * astr.getRadius())
		return 1;

	return 0;
}

//КЛАСС goodRocket
goodRocket::goodRocket(int setx, int sety, COLORREF setColor) : ARocket(setx, sety, setColor)			//конструктор
{
}

void goodRocket::show(COLORREF color)
{
	drawNose(color);
	drawBody(color);
}

void goodRocket::hide()
{
	hideNose();
	hideBody();
}

bool goodRocket::checkCollision(ACircle& astr)
{
	//находим ближайшую точку между центром астероида и ракетой
	int xn = max(width / 2 - rocWidth + dx, min(astr.getx(), width / 2 + rocWidth + dx));
	int yn = max(height / 2 + dy, min(astr.gety(), height / 2 + rocHeight + dy));

	//разность координат астероида и ближайшей точки
	int dlx = xn - astr.getx();
	int dly = yn - astr.gety();

	//по формуле круга узнаем соприкоснулись ли точки
	if ((dlx * dlx + dly * dly) < astr.getRadius() * astr.getRadius())
		return 1;

	//возьмем внутри треугольника маленький квадрат и сделаем ту же проверку
	xn = max(width / 2 - rocWidth / 2 + dx, min(astr.getx(), width / 2 + rocWidth / 2 + dx));
	yn = max(height / 2 - rocWidth / 2 + dy, min(astr.gety(), height / 2 + dy));

	dlx = xn - astr.getx();
	dly = yn - astr.gety();

	if ((dlx * dlx + dly * dly) < astr.getRadius() * astr.getRadius())
		return 1;

	return 0;
}

//КЛАСС badRocket
badRocket::badRocket(int setx, int sety, COLORREF setColor) : ARocket(setx, sety, setColor)			//конструктор
{
}

void badRocket::show(COLORREF color)
{
	drawBody(color);
}

void badRocket::hide()
{
	hideBody();
}

bool badRocket::checkCollision(ACircle& astr)
{
	//находим ближайшую точку между центром астероида и ракетой
	int xn = max(width / 2 - rocWidth + dx, min(astr.getx(), width / 2 + rocWidth + dx));
	int yn = max(height / 2 + dy, min(astr.gety(), height / 2 + rocHeight + dy));

	//разность координат астероида и ближайшей точки
	int dlx = xn - astr.getx();
	int dly = yn - astr.gety();

	//по формуле круга узнаем соприкоснулись ли точки
	if ((dlx * dlx + dly * dly) < astr.getRadius() * astr.getRadius())
		return 1;

	return 0;
}