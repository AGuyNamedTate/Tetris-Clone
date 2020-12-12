#include "TetrisGame.h"
#include <iostream>
#include <cassert>
//#include "Tetromino.h"


TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, Point gameboardOffset, Point nextShapeOffset)
	:window(window), blockSprite(blockSprite), gameboardOffset(gameboardOffset), nextShapeOffset(nextShapeOffset) {
	reset();
	//currentShape.setShape(currentShape.getRandomShape());
	//currentShape.setGridLoc(board.getSpawnLoc());
	//board.setContent(Gameboard::MAX_X / 2, Gameboard::MAX_Y / 2, 1);
	//draw();
	if (!scoreFont.loadFromFile("fonts/RedOctober.ttf")) {
		assert(false && "Missing font: RedOctober.ttf");
	};
	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(435, 325);

	//For Game Audio
	assert(bgm.openFromFile("sfx/tetrisMusic.ogg"));
	bgm.setLoop(true);
	bgm.setLoopPoints(sf::Music::TimeSpan(sf::seconds(6.443f), bgm.getDuration()));
	bgm.play();
}

// Draw anything to do with the game,
//   includes the board, currentShape, nextShape, score
//   called every game loop
void TetrisGame::draw() {
	drawGameboard();
	drawTetromino(currentShape, gameboardOffset);
	drawTetromino(nextShape, nextShapeOffset);
	drawTetromino(ghostShape, gameboardOffset, 0.5f);
	window.draw(scoreText);
}

// Event and game loop processing
// handles keypress events (up, left, right, down, space)
void TetrisGame::onKeyPressed(sf::Event event) {
	switch (event.key.code) {
	case sf::Keyboard::Up:
		attemptRotate(currentShape);
		updateGhostShape();
		break;
	case sf::Keyboard::Left:
		attemptMove(currentShape, -1, 0);
		updateGhostShape();
		break;
	case sf::Keyboard::Down:
		if (!attemptMove(currentShape, 0, 1)) {
			lock(currentShape);
			shapePlacedSinceLastGameLoop = true;
		}
		break;
	case sf::Keyboard::Right:
		attemptMove(currentShape, 1, 0);
		updateGhostShape();
		break;
	case sf::Keyboard::Space:
		drop(currentShape);
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
		break;
	}
}

// called every game loop to handle ticks & tetromino placement (locking)
void TetrisGame::processGameLoop(float secondsSinceLastLoop) {
	secondsSinceLastTick += secondsSinceLastLoop;
	if (shapePlacedSinceLastGameLoop) {
		if (!spawnNextShape()) {
			reset();
		}
		else {
			pickNextShape();
			int rowsRemoved = board.removeCompletedRows();
			score += (SCORE_PER_ROW *(rowsRemoved))*rowsRemoved;
			updateScoreDisplay();
			determineSecondsPerTick();
		}
		shapePlacedSinceLastGameLoop = false;
	}
	if (secondsSinceLastTick >= secondsPerTick) {
		tick();
		secondsSinceLastTick = 0;
	}
}

// A tick() forces the currentShape to move (if there were no tick,
// the currentShape would float in position forever). This should
// call attemptMove() on the currentShape.  If not successful, lock() 
// the currentShape (it can move no further), and record the fact that a
// shape was placed (using shapePlacedSinceLastGameLoop)
void TetrisGame::tick() {
	if (!attemptMove(currentShape, 0, 1)) {
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
	}
}

// reset everything for a new game (use existing functions) 
	//  - setScore to 0
	//  - determineSecondsPerTick(),
	//  - clear the gameboard,
	//  - pick & spawn next shape
	//  - pick next shape again (for the "on-deck" shape)
void TetrisGame::reset() {
	score = 0;
	updateScoreDisplay();
	determineSecondsPerTick();
	board.empty();
	pickNextShape();
	spawnNextShape();
	pickNextShape();
}

// assign nextShape.setShape a new random shape  
void TetrisGame::pickNextShape() {
	nextShape.setShape(nextShape.getRandomShape());
}

// copy the nextShape into the currentShape (through assignment)
//   position the currentShape to its spawn location.
//	 - return true/false based on isPositionLegal()
bool TetrisGame::spawnNextShape() { 
	currentShape = nextShape;
	currentShape.setGridLoc(board.getSpawnLoc());
	updateGhostShape();
	return isPositionLegal(currentShape);
}

// Test if a rotation is legal on the tetromino and if so, rotate it. 
//  To do this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) rotate it (shape.rotateClockwise())
//	 3) test if temp rotation was legal (isPositionLegal()), 
//      if so - rotate the original tetromino.
//	 4) return true/false to indicate successful movement
bool TetrisGame::attemptRotate(GridTetromino& shape) { 
	GridTetromino t = shape;
	t.rotateClockwise();
	if (isPositionLegal(t)) {
		shape.rotateClockwise();
		return true;
	}
	return false;
}

// test if a move is legal on the tetromino, if so, move it.
//  To do this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) move it (temp.move())
//	 3) test if temp move was legal (isPositionLegal(),
//      if so - move the original.
//	 4) return true/false to indicate successful movement
bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y) {
	GridTetromino t = shape;
	t.move(x, y);
	if (isPositionLegal(t)) {
		shape.move(x,y);
		return true;
	}
	return false;
}

// drops the tetromino vertically as far as it can 
//   legally go.  Use attemptMove(). This can be done in 1 line.
void TetrisGame::drop(GridTetromino& shape) {
	while (attemptMove(shape, 0, 1)) {};
}

// copy the contents of the tetromino's mapped block locs to the grid.
//	 1) get current blockshape locs via tetromino.getBlockLocsMappedToGrid()
//	 2) iterate on the mapped block locs and copy the contents (color) 
//      of each to the grid (via gameboard.setGridContent()) 
void TetrisGame::lock(const GridTetromino& shape) {
	for (const Point& each : shape.getBlockLocsMappedToGrid()) {
		board.setContent(each, static_cast<int>(shape.getColor()));
	}
}

// Graphics methods ==============================================

// Draw a tetris block sprite on the canvas		
// The block position is specified in terms of 2 offsets: 
//    1) the top left (of the gameboard in pixels)
//    2) an x & y offset into the gameboard - in blocks (not pixels)
//       meaning they need to be multiplied by BLOCK_WIDTH and BLOCK_HEIGHT
//       to get the pixel offset.
//	 1) set the block color using blockSprite.setTextureRect()
//   2) set the block location using blockSprite.setPosition()   
//	 3) draw the block using window.draw()
//   For details/instructions on these 3 operations see:
//       www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
//       use member variables: window and blockSprite (assigned in constructor)
void TetrisGame::drawBlock(const Point& topLeft, int xOffset, int yOffset, TetColor color, float alpha) {
	sf::IntRect blockTexture = sf::IntRect(static_cast<int>(color)*BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_HEIGHT);
	blockSprite.setTextureRect(blockTexture);
	blockSprite.setPosition(topLeft.getX()+(xOffset*BLOCK_WIDTH), topLeft.getY()+(yOffset*BLOCK_HEIGHT));
	blockSprite.setColor(sf::Color(255,255,255,alpha*255));
	window.draw(blockSprite);
}

// Draw the gameboard blocks on the window
//   Iterate through each row & col, use drawBlock() to 
//   draw a block if it isn't empty.
void TetrisGame::drawGameboard() {
	for (int y{ 0 };y < Gameboard::MAX_Y;y++) {
		for (int x{ 0 }; x < Gameboard::MAX_X;x++) {
			if (board.getContent(x, y) != Gameboard::EMPTY_BLOCK) {
				drawBlock(gameboardOffset, x, y, static_cast<TetColor>(board.getContent(x,y)));
			}
		}
	}
}

// Draw a tetromino on the window
//	 Iterate through each mapped loc & drawBlock() for each.
//   The topLeft determines a 'base point' from which to calculate block offsets
//      If the Tetromino is on the gameboard: use gameboardOffset
void TetrisGame::drawTetromino(const GridTetromino& tetromino, const Point& topLeft , float alpha) {
	for (const Point& each : tetromino.getBlockLocsMappedToGrid()) {
		drawBlock(topLeft, each.getX(), each.getY(),tetromino.getColor(), alpha);
	}
}

// update the score display
// form a string "score: ##" to display the current score
// user scoreText.setString() to display it.
void TetrisGame::updateScoreDisplay() {
	std::string scoreString = "Score: " + std::to_string(score);
	scoreText.setString(scoreString);
}

// State & gameplay/logic methods ================================

// return true if shape is within borders (isShapeWithinBorders())
//	 and does NOT intersect locked blocks (!doesShapeIntersectLockedBlocks())
bool TetrisGame::isPositionLegal(const GridTetromino& shape) const { 
	if (isShapeWithinBorders(shape) && !doesShapeIntersectLockedBlocks(shape)) {
		return true;
	}
	return false;
}

// return true if the shape is within the left, right, and lower border of
//	 the grid, but *NOT* the top border. (return false otherwise)
//   * Ignore the upper border because we want shapes to be able to drop
//     in from the top of the gameboard.
//   All of a shape's blocks must be inside these 3 borders to return true
bool TetrisGame::isShapeWithinBorders(const GridTetromino& shape) const {
	for (const Point& each : shape.getBlockLocsMappedToGrid()) {
		if ((each.getX() < 0 || each.getX() >= board.MAX_X) || each.getY() >= board.MAX_Y) {
			return false;
		}
	}
	return true;
}

// return true if the shape passed in intersects with content on the gameboard.
//   Make use of Gameboard's areLocsEmpty() and pass it the shape's mapped locs
//   in order to test if the locations are NOT empty.
bool TetrisGame::doesShapeIntersectLockedBlocks(const GridTetromino& shape) const { 
	if (board.areLocsEmpty(shape.getBlockLocsMappedToGrid())) {
		return false;
	}
	return true;
}

// set secsPerTick 
//   - basic: use MAX_SECS_PER_TICK
//   - advanced: base it on score (higher score results in lower secsPerTick)
void TetrisGame::determineSecondsPerTick() {
	secondsPerTick = std::max(MIN_SECONDS_PER_TICK, MAX_SECONDS_PER_TICK - (score / 10000.0));
}

void TetrisGame::updateGhostShape() {
	ghostShape = currentShape;
	drop(ghostShape);
}