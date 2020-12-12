#include "Point.h"
#include <sstream>

Point::Point() {
	this->x= 0;
	this->y= 0;
}
Point::Point(int x, int y) {
	this->x = x;
	this->y = y;
}
int Point::getX() const{
	return this->x;
}
int Point::getY() const{
	return this->y;
}
void Point::setX(int x) {
	this->x = x;
}
void Point::setY(int y) {
	this->y = y;
}
void Point::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}
void Point::swapXY() {
	int temp =this->x;
	this->x = this->y;
	this->y = temp;
}
void Point::multiplyX(int factor) {
	this->x = this->x * factor;
}
void Point::multiplyY(int factor) {
	this->y = this->y * factor;
}
std::string Point::toString() const{
	std::stringstream ss;
	ss << getX();
	std::string x = ss.str();
	ss.str("");
	ss << getY();
	std::string y = ss.str();

	return "[" + x + "," + y+"]";
}