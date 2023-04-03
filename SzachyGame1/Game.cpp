#include "Game.h"



bool Game::handleClick(sf::Vector2i mousePos)
{
	if (!gameBoard.checkMovingPossibilities((turn == TURN_P1 ? 1 : 2)))
	{
		return false;
	}
	sf::Vector2f clickedField;
	clickedField.x= (mousePos.x - 28) / 56 + 1;
	clickedField.y= (mousePos.y - 28) / 56 + 1;
	FIELD fieldtype= gameBoard.checkFieldType(sf::Vector2f(clickedField.x, clickedField.y));
	std::string possibleMoves = gameBoard.giveMoves(clickedField, (turn == TURN_P1 ? 1 : 2));
	if ((fieldtype == FLD_PAW1 && turn == TURN_P1)||(fieldtype == FLD_PAW2 && turn == TURN_P2))
	{
		gameBoard.setPickedPaw(clickedField);
		firstClick = true;
	}
	else if(firstClick)
	{
		possibleMoves = gameBoard.giveMoves(gameBoard.getPickedPaw(), (turn == TURN_P1 ? 1 : 2));
		for (int i = 0; i < possibleMoves.size(); i += 3)
		{
			int posX = possibleMoves[i] - '0';
			int posY = possibleMoves[i + 1] - '0';
			if (clickedField.x == posX && clickedField.y == posY)
			{
				if (!gameBoard.testChangingPosition(gameBoard.getPickedPaw(), clickedField, (turn == TURN_P1 ? 1 : 2)))
				{
					break;
				}
				if (gameBoard.setPawPosition(gameBoard.getPickedPaw(), clickedField) == MG_HIT)
				{
					gameBoard.deletePaw(clickedField, (turn == TURN_P1 ? 2 : 1));
					graphicBoard.deletePaw(clickedField);
				}
				graphicBoard.setFigurePosition(gameBoard.getPickedPaw(), clickedField, gWindow);
				if ((clickedField.y == 8 && turn == TURN_P1) || (clickedField.y == 1 && turn == TURN_P2))
				{
					int type;
					if (gameBoard.isTransforamble(clickedField, type))
					{
						graphicBoard.transformFigure(clickedField, type, (turn == TURN_P1 ? 1 : 2));
					}
				}
				turn = turn == TURN_P1 ? TURN_P2 : TURN_P1;
				break;
			}
		}
		firstClick = false;
	}

	if (firstClick)
	{
		gWindow.clear();
		graphicBoard.draw(gWindow, possibleMoves);
		gWindow.display();
	}
	else
	{
		gWindow.clear();
		graphicBoard.draw(gWindow,"");
		gWindow.display();
	}
	return true;
}

void Game::startGame()
{
	gWindow.clear();
	graphicBoard.draw(gWindow,"");
	gWindow.display();
}

Game::Game(sf::RenderWindow& windowS) : gameBoard(), graphicBoard(),gWindow(windowS)
{
	turn = TURN_P1;
	firstClick = false;
}


Game::~Game()
{
}
