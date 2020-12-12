#pragma once
#include <vector>
#include "Point.h"
enum class TetShape { S, Z, L, J, O, I, T, COUNT};
enum class TetColor { RED , ORANGE, YELLOW, GREEN, BLUE_LIGHT, BLUE_DARK, PURPLE};

class Tetromino {
	friend class TestSuite;
private:
	TetColor color;
	TetShape shape;

protected:
	std::vector<Point> blockLocs;

public:
	Tetromino();
	TetColor getColor() const;

	TetShape getShape() const;

	// set the shape
	//-clear any blockLocs set previously
	//  -set the blockLocs for the shape
	//  -set the color for the shape
	void setShape(TetShape shape);
	//	
	// rotate the shape 90 degrees around [0,0] (clockwise)
	// to do this:
	//  -iterate through blockLocs
	//  -rotate each Point 90 degrees around [0,0]
	// hint: rotate the point [1,2] clockwise around[0,0] and note
	// how the x,y values change. There are 2 functions inthe Point
	// class that can be used to accomplish a rotation.
	void rotateClockwise();

	void rotateCounterClockwise();
	// print a grid to display the current shape
	// to do this:print out a “grid”of text to represent a co-ordinate
	// system.  Start at top left [-3,3] go to bottom right [3,-3]
	// (use nested for loops)
	//  for each [x,y]point, loop through the blockLocs andif the point exists
	//in the list, print an 'x' instead of a '.'.You should end up with something
	//like this: (results will vary depending on shape and rotation, eg: this
	//  one shows a T shape rotated clockwise once)
	//  .......
	//  .......
	//  ...x...
	//  ..xx...
	//  ...x...
	//  .......
	void printToConsole() const;

	TetShape getRandomShape();
};
