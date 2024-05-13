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
Point& Point::operator=(const Point& v){
	setX(v.getX());
	setY(v.getY());
	return *this;
}

Point& Point::operator+=(const Point& v){
	setX(getX() + v.getX());
	setY(getY() + v.getY());
	return *this;
}

Point Point::operator+(const Point& v){
	Point v2(getX(), getY());
	return v2 += v;
}

Point& Point::operator-=(const Point& v){
	setX(getX() - v.getX());
	setY(getY() - v.getY());
	return *this;
}

Point Point::operator-(const Point& v){
	Point v2(getX(), getY());
	return v2 -= v;
}

bool Point::operator==(const Point& v){
	return getX() == v.getX() && getY() == v.getY();
}

bool Point::operator!=(const Point& v){
	return !operator == (v);
}