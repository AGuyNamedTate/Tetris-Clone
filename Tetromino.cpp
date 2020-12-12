#include "Tetromino.h"
#include "Point.h"
#include <iostream>


Tetromino::Tetromino() {
	setShape(getRandomShape());
}

TetShape Tetromino::getRandomShape() {
	return static_cast<TetShape>(rand() % static_cast<int>(TetShape::COUNT)); // COUNT represents the total number of shapes (7)
}

TetColor Tetromino::getColor() const {
	return this->color;
}

TetShape Tetromino::getShape() const {
	return this->shape;
}

void Tetromino::setShape(TetShape shape) {
	blockLocs.empty();
	switch (shape) {
	case TetShape::S:
		blockLocs = {Point(0,0),Point(0,1),Point(1,1),Point(-1,0)};
		color = TetColor::RED;
		this->shape = shape;
		break;
	case TetShape::L:
		blockLocs = {Point(0,0),Point(0,1),Point(0,-1),Point(1,-1)};
		color = TetColor::ORANGE;
		this->shape = shape;
		break;
	case TetShape::J:
		blockLocs = {Point(0,0),Point(0,1),Point(0,-1),Point(-1,-1)};
		color = TetColor::BLUE_DARK;
		this->shape = shape;
		break;
	case TetShape::O:
		blockLocs = {Point(0,0),Point(0,1),Point(1,1),Point(1,0)};
		color = TetColor::YELLOW;
		this->shape = shape;
		break;
	case TetShape::Z:
		blockLocs = {Point(0,0),Point(0,1),Point(-1,1),Point(1,0)};
		color = TetColor::GREEN;
		this->shape = shape;
		break;
	case TetShape::I:
		blockLocs = {Point(0,0),Point(0,1),Point(0,2),Point(0,-1)};
		color = TetColor::BLUE_LIGHT;
		this->shape = shape;
		break;
	case TetShape::T:
		blockLocs = {Point(0,0),Point(-1,0),Point(1,0),Point(0,-1)};
		color = TetColor::PURPLE;
		this->shape = shape;
		break;
	}
}

void Tetromino::rotateClockwise() {
	if (shape != TetShape::O) {
		for (int i{ 0 }; i < blockLocs.size();i++) {
			blockLocs[i].multiplyY(-1);
			blockLocs[i].swapXY();
		}
	}
}

void Tetromino::rotateCounterClockwise() {
	if (shape != TetShape::O) {
		for (int i{ 0 }; i < blockLocs.size();i++) {
			blockLocs[i].swapXY();
			blockLocs[i].multiplyY(-1);
		}
	}
}
	

void Tetromino::printToConsole() const {
	for (int y{3}; y > -3;y--) {
		for (int x{ -3 }; x < 3; x++) {
			char toPrint{ '.' };
			for (Point each : blockLocs) {
				if (each.getX() == x && each.getY() == y) {
					toPrint = 'x' ;
				}
			}
			std::cout << toPrint<<' ';
		}
		std::cout << std::endl;
	}
}
