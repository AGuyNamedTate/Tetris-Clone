#include "GridTetromino.h"

GridTetromino::GridTetromino() {
	this->gridLoc = Point(0,0);
}

// return the tetromino's grid/gameboard loc (x,y)
Point GridTetromino::getGridLoc() const {
	return this->gridLoc;
}
// sets the tetromino's grid/gameboard loc using x,y
void GridTetromino::setGridLoc(int x, int y) {
	this->gridLoc = Point(x, y);
}
// sets the tetromino's grid/gameboard loc using a Point
void GridTetromino::setGridLoc(const Point& pt) {
	this->gridLoc = pt;
}

// transpose the gridLoc of this shape
//	(1,0) represents a move to the right (x+1)
//	(-1,0) represents a move to the left (x-1)
//	(0,1) represents a move down (y+1)
void GridTetromino::move(int xOffset, int yOffset) {
	gridLoc.setX(gridLoc.getX() + xOffset);
	gridLoc.setY(gridLoc.getY() + yOffset);
}

// build and return a vector of Points to represent our inherited
// blockLocs vector mapped to the gridLoc of this object instance.
// eg: if we have a Point [x,y] in our vector,
// and our gridLoc is [5,6] the mapped Point would be [5+x,6+y].
std::vector<Point> GridTetromino::getBlockLocsMappedToGrid() const {
	std::vector<Point> mappedLocs;
	for (Point each : blockLocs) {
		mappedLocs.push_back(Point(each.getX() + gridLoc.getX(), each.getY() + gridLoc.getY()));
	}
	return mappedLocs;
}