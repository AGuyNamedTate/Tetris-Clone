#pragma once
#include <string>

class Point {
	friend class TestSuite;
private:
	//variable representing the x value of the point (left to right)
	int x;
	//variable representing the y value of the point (bottom to top)
	int y;
public:
	//default point constructor to initialize the point (0,0) by default
	Point();
	//constructor to create a point with the x corrdinate x and the y coordinate y
	Point(int x, int y);
	int getX() const;
	int getY() const;
	void setX(int x);
	void setY(int y);
	void setXY(int x, int y);
	// swap x and y
	void swapXY();
	// multiply x by some factor
	void multiplyX(int factor);
	// multiply y by some factor
	void multiplyY(int factor);
	// return a string in the form "[x,y]" to representthe state of the Point instance 
	// (for debugging)You could use stringstream if you want to try a forming a string in 
	// the same way you use cout http://stev.org/post/cstringstreamexamplefor an example
	std::string toString() const;
};