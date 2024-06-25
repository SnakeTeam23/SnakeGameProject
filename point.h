using namespace std;

class Point{
private:
	int x,y;

public:

	Point& operator+=(const Point& p);
	Point operator+(const Point& p);
	Point& operator-=(const Point& p);
	Point& operator=(const Point& p);
	Point operator-(const Point& p);
	bool operator==(const Point& p);
	bool operator!=(const Point& p);

	void set_x(int x);
	void set_y(int y);
	int get_x() const;
	int get_y() const;

	Point();
	Point(int x, int y);
	~Point();
};