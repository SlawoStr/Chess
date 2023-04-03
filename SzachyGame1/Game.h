#pragma once
#include "GameBoard.h"
#include "GraphicBoard.h"
#include <iostream>

enum TURN{TURN_P1,TURN_P2};
class Game
{
	GameBoard gameBoard;
	GraphicBoard graphicBoard;
	TURN turn;
	bool firstClick;
	sf::RenderWindow& gWindow;
public:
	bool handleClick(sf::Vector2i mousePos);
	void startGame();
	Game(sf::RenderWindow &);
	~Game();
};

