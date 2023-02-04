#pragma once
#include <windows.h>

const int width = 920;		//ширина экрана
const int height = 640;		//высота экрана
const int numStars = 50;	//кол-во здезд
const int numAseroids = 20;	//кол-во астероидов

//класс позиции
class Location
{
protected:
	int x;	//координата х
	int y;	//координата у

public:
	Location(int InitX, int InitY); //Конструктор
	~Location();                    //Деструктор
	int getx();						//узнать значение х
	int gety();						//узнать значение у
};

//класс точки
class ACircle : public Location
{
protected:
	int radius = 0;			//радиус объекта

public:
	ACircle(int setx, int sety);	//конструктор
	~ACircle();						//деструктор
	int getRadius();				//узнать радиус объекта
	void setRadius(int setradius);	//установить радиуса объекта

	//виртуальные методы
	virtual void hide() = 0;								//функция стирания объекта
	virtual void show(COLORREF rgb) = 0;					//функция рисования объекта
	virtual void move(int tox, int toy, COLORREF rgb);		//функция перемещения объекта
};

//класс астероида
class Asteroid : public ACircle
{
public:
	Asteroid(int setx = 0, int sety = 0, int setradius = 30);	//конструктор
	virtual void hide();										//стереть объект
	virtual void show(COLORREF rgb);							//нарисовать объект
};

//класс бонусов
class Bounty : public ACircle
{
protected:
	int figure = 0;	//фигура

public:
	Bounty(int setx = 0, int sety = 0, int setradius = 10);	//конструктор
	void setFigure(int fgr);								//выбор фигуры
	virtual void show(COLORREF color);						//нарисовать бонус
	virtual void hide();									//стереть бонус
};

//класс ракеты
class ARocket : public ACircle
{
protected:
	int rocWidth = 0;	//ширина ракеты
	int rocHeight = 0;	//высота ракеты
	int dx = 0;			//смещение ракеты по х
	int dy = 0;			//смещение ракеты по у	
	COLORREF damageColor = RGB(255, 181, 0);	//цвет ракеты
	POINT points[3];	//носовая часть ракеты

public:
	ARocket(int newWidth, int newHeight, COLORREF setColor);	//конструктор
	int getRocWidth();											//узнать ширину ракеты
	int getRocHeight();											//узнать высоту ракеты
	int getRocDx();												//узнать смещение по х
	int getRocDy();												//узнать смещение по у

	void setDamageColor(COLORREF rgb);							//установить цвет ракеты в зависимости от повреждений
	void changeRocOffsetX(int val);								//сместить оакету по х
	void changeRocOffsetY(int val);								//сместить ракету по 

	//рисовка компонентов
	virtual void drawNose(COLORREF penclr);						//носовая часть
	virtual void drawRightTank(COLORREF penclr);				//правый топливный бак
	virtual void drawLeftTank(COLORREF penclr);					//левый топливный бак
	virtual void drawBody(COLORREF penclr);						//корпус

	//удадение компонентов
	virtual void hideNose();
	virtual void hideRightTank();
	virtual void hideLeftTank();
	virtual void hideBody();

	virtual bool checkCollision(ACircle& astr) = 0;				//проверить столкновение с объектом
};	

class wellRocket : public ARocket
{
public:
	wellRocket(int setx, int sety, COLORREF setColor);			//конструктор

	virtual void show(COLORREF color) override;					//нарисовать бонус
	virtual void hide() override;								//стереть бонус
	virtual bool checkCollision(ACircle& astr) override;		//проверить столкновение с объектом
};

class wellRocketWithNoses : public wellRocket
{
public:
	wellRocketWithNoses(int setx, int sety, COLORREF setColor);	//конструктор

	//рисуем части ракеты как нам угодно в этом классе
	virtual void drawRightTank(COLORREF penclr) override;		//перерисовываем правый топливный бак
	virtual void hideRightTank() override;						//скрываем правый топливный бак
	virtual void drawLeftTank(COLORREF penclr) override;		//перерисовываем левый топливный бак
	virtual void hideLeftTank() override;						//скрываем левый топливный бак
};

class goodRocket : public ARocket
{
public:
	goodRocket(int setx, int sety, COLORREF setColor);			//конструктор

	virtual void show(COLORREF color) override;					//нарисовать бонус
	virtual void hide() override;								//стереть бонус
	virtual bool checkCollision(ACircle& astr) override;		//проверить столкновение с объектом
};

class badRocket : public ARocket
{
public:
	badRocket(int setx, int sety, COLORREF setColor);			//конструктор

	virtual void show(COLORREF color) override;					//нарисовать бонус
	virtual void hide() override;								//стереть бонус
	virtual bool checkCollision(ACircle& astr) override;		//проверить столкновение с объектом
};