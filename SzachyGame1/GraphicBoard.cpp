#include "GraphicBoard.h"
#include <Windows.h>
#include <iostream>

GraphicBoard::GraphicBoard()
{
	int board[8][8] =
	{
		 1, 2, 3, 4, 5, 3, 2, 1,
		 6, 6, 6, 6, 6, 6, 6, 6,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		-6,-6,-6,-6,-6,-6,-6,-6,
		-1,-2,-3,-4,-5,-3,-2,-1
	};

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			gBoard[i][j] = board[i][j];

	boardTexture.loadFromFile("images/board.png");
	pawTexture.loadFromFile("images/figures.png");

	boardSprite.setTexture(boardTexture);
	for (int i = 0; i < 32; i++)
	{
		figures.push_back(new sf::Sprite(pawTexture));
	}

	int k = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (gBoard[i][j] != 0)
			{
				int n = gBoard[i][j] < 0 ? 1 : 0;
				int column = abs(gBoard[i][j])-1;
				figures[k]->setTextureRect(sf::IntRect(size*column, size*n, size, size));
				figures[k]->setPosition(size*j + 28, size*i + 28);
				k++;
			}
		}
	}


}


GraphicBoard::~GraphicBoard()
{
	for (int i = 0; i < figures.size(); i++)
	{
		delete figures[i];
	}
}

void GraphicBoard::draw(sf::RenderWindow & window,std::string moves)
{
	window.draw(boardSprite);
	for (int i = 0; i < figures.size(); i++)
	{
		window.draw(*figures[i]);
	}
	if (moves.size() != 0)
	{
		showPossibleMoves(moves, window);
	}
}

void GraphicBoard::setFigurePosition(sf::Vector2f startingPosition, sf::Vector2f finalPosition, sf::RenderWindow & window)
{
	for (int i = 0; i < figures.size(); i++)
	{
		if (figures[i]->getGlobalBounds().contains((startingPosition.x - 1)*size + 28, (startingPosition.y - 1)*size + 28))
		{
				float xChange=((startingPosition.x - 1)*size + size / 2) - ((finalPosition.x - 1)*size + size / 2);
				float yChange =((startingPosition.y - 1)*size + size / 2) - ((finalPosition.y - 1)*size + size / 2);
				xChange *= -1;
				yChange *= -1;
				sf::Vector2f position;
				position.x = (startingPosition.x - 1)*size + size / 2;
				position.y = (startingPosition.y - 1)*size + size / 2;

				xChange /= 1000;
				yChange /= 1000;
				for (int j = 0; j < 1000; j++)
				{
					figures[i]->setPosition(position);
					position.x += xChange;
					position.y += yChange;
					window.clear();
					draw(window,"");
					window.display();
					Sleep(0.6);
				}
			    figures[i]->setPosition((finalPosition.x - 1)*size + 28, (finalPosition.y - 1)*size + 28);
				break;
		}
	}
}

void GraphicBoard::deletePaw(sf::Vector2f pawPosition)
{
	int i = 0;
	for (auto it = figures.begin(); it < figures.end(); it++)
	{
		if (figures[i]->getGlobalBounds().contains((pawPosition.x - 1)*size + 28, (pawPosition.y - 1)*size + 28))
		{
			delete figures[i];
			figures.erase(it);
			break;
		}
		else
		{
			i++;
		}
	}


}

void GraphicBoard::transformFigure(sf::Vector2f pawPosition, int type, int team)
{
	for (int i = 0; i < figures.size(); i++)
	{
		if (figures[i]->getGlobalBounds().contains((pawPosition.x - 1)*size + 28, (pawPosition.y - 1)*size + 28))
		{
			int n = (team == 1 ? 0 : 1);
			figures[i]->setTextureRect(sf::IntRect(size*type, size*n, size, size));
		}
	}

}

void GraphicBoard::showPossibleMoves(std::string moves,sf::RenderWindow & window)
{
	sf::CircleShape shape(10.f);
	shape.setFillColor(sf::Color::Red);
	for (int i = 0; i < moves.size(); i += 3)
	{
		int posX = moves[i] - '0';
		int posY = moves[i + 1] - '0';
		shape.setPosition((posX - 1)*size-10 + size, (posY - 1)*size + size-10);//nowy wzór
		window.draw(shape);
	}
}
