#include "point.h"
using namespace std;

Point::Point(): x(0), y(0){}
Point::Point(int x, int y):x(x), y(y) {}
Point::~Point(){}

void Point::set_x(int x){(*this).x = x;}
void Point::set_y(int y){(*this).y = y;}
int Point::get_x() const {return x;}
int Point::get_y() const {return y;}


Point& Point::operator+=(const Point& p){
	x = get_x() + p.get_x();
	y = get_y() + p.get_y();
	return *this;
}

Point Point::operator-(const Point& p){
	Point v2(x, y);
	return v2 -= p;
}

Point Point::operator+(const Point& p){
	Point v2(x, y);
	return v2 += p;
}


Point& Point::operator-=(const Point& p){
	set_x(x - p.get_x());
	set_y(y - p.get_y());
	return *this;
}

bool Point::operator!=(const Point& p){
	return !((*this) == p);
}

bool Point::operator==(const Point& p){
	return get_x() == p.get_x() && get_y() == p.get_y();
}

Point& Point::operator=(const Point& p){
	x = p.get_x();
	y = p.get_y();
	return *this;
}

