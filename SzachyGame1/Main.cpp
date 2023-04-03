#include <SFML/Graphics.hpp>
#include "GraphicBoard.h"
#include <iostream>
#include "Game.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(504, 504), "SFML works!");

	GraphicBoard graphicBoard;
	GameBoard gameBoard;
	Game game(window);
	std::string napis;
	game.startGame();
	bool nextRound = true;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);

			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					if ((mousePos.x > 28 && mousePos.x < 476) && (mousePos.y > 28 && mousePos.y < 476))
					{
						if (nextRound == true)
						{
							nextRound =game.handleClick(mousePos);
						}
					}
				}
			}
		}
		if (nextRound == false)
		{
			std::cout << "GAME OVER" << std::endl;
		}
		/*
		window.clear();
		graphicBoard.draw(window,napis);
		window.display();
		*/
	}

	return 0;
}