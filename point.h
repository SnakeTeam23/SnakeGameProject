#include <iostream>
#include <vector>
using namespace std;

class Point{
protected:
	int x,y;

public:
    //constructor
	Point();
	Point(int x, int y);

    //setter, getter
	void setX(int x);
	void setY(int y);
	int getX() const;
	int getY() const;

    //operator overloading
	Point& operator=(const Point& v);
	Point& operator+=(const Point& v);
	Point operator+(const Point& v);
	Point& operator-=(const Point& v);
	Point operator-(const Point& v);
	bool operator==(const Point& v);
	bool operator!=(const Point& v);
};