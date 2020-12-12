#include <SFML/Graphics.hpp>
#include <iostream>
#include "TetrisGame.h"
#include "TestSuite.h"
#include <time.h>


int main()
{
	srand(time(0)); //seed the randomizer for random tetrominos;
	// run some sanity tests on our classes to ensure they're working as expected.	
	TestSuite::runTestSuite();

	sf::Sprite blockSprite;			// the tetromino block sprite
	sf::Texture blockTexture;		// the tetromino block texture
	sf::Sprite backgroundSprite;	// the background sprite
	sf::Texture backgroundTexture;	// the background texture

	// load images
	backgroundTexture.loadFromFile("images/background.png");// load the background sprite
	backgroundSprite.setTexture(backgroundTexture);

	blockTexture.loadFromFile("images/tiles.png");			// load the tetris block sprite
	blockSprite.setTexture(blockTexture);

	// create the game window
	sf::RenderWindow window(sf::VideoMode(640, 800), "Tetris Game Window");
	// set a max framerate of 30 FPS
	window.setFramerateLimit(30);

	// set up a tetris game
	TetrisGame game(window, blockSprite, Point(54, 125), Point(490, 210));

	// set up a clock so we can determine seconds per game loop
	sf::Clock clock;

	// create an event for handling userInput from the GUI (graphical user interface)
	sf::Event guiEvent;

	// the main game loop
	while (window.isOpen())
	{
		// handle any window or keyboard events that have occured since the last game loop
		while (window.pollEvent(guiEvent))
		{
			if (guiEvent.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (guiEvent.type == sf::Event::KeyPressed)
			{
				game.onKeyPressed(guiEvent);	// handle key press
			}
		}

		// how long since the last loop (fraction of a second)		
		float elapsedTime = clock.getElapsedTime().asSeconds();
		clock.restart();

		// handle any window or keyboard events that have occured since the last game loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				game.onKeyPressed(event);	// handle key press
			}
		}

		game.processGameLoop(elapsedTime);	// handle tetris game logic in here.

		// Draw the game to the screen
		window.clear(sf::Color::White);	// clear the entire window
		window.draw(backgroundSprite);	// draw the background (onto the window) 				
		game.draw();					// draw the game (onto the window)
		window.display();				// re-display the entire window
	}

	return 0;
}
