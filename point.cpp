#include "point.h"
using namespace std;

//Constructor
Point::Point(): x(0), y(0){}
Point::Point(int x, int y) {setX(x); setY(y);}

//setter and getter
void Point::setX(int x){this->x = x;}
void Point::setY(int y){this->y = y;}
int Point::getX() const {return this->x;}
int Point::getY() const {return this->y;}

//operator overloading
Point& Point::operator=(const Point& p){
	setX(p.getX());
	setY(p.getY());
	return *this;
}

Point& Point::operator+=(const Point& p){
	setX(getX() + p.getX());
	setY(getY() + p.getY());
	return *this;
}

Point Point::operator+(const Point& p){
	Point v2(getX(), getY());
	return v2 += p;
}
Point Point::operator-(const Point& p){
	Point v2(getX(), getY());
	return v2 -= p;
}

Point& Point::operator-=(const Point& p){
	setX(getX() - p.getX());
	setY(getY() - p.getY());
	return *this;
}

bool Point::operator==(const Point& p){
	return getX() == p.getX() && getY() == p.getY();
}

bool Point::operator!=(const Point& p){
	return !operator == (p);
}