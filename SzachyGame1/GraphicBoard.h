#pragma once
#include <SFML/Graphics.hpp>

class GraphicBoard
{
private:
	static const int size = 56;
	int gBoard[8][8];
	sf::Texture boardTexture;
	sf::Texture pawTexture;
	sf::Sprite boardSprite;
	std::vector<sf::Sprite*>figures;

public:
	GraphicBoard();
	~GraphicBoard();
	void draw(sf::RenderWindow & window,std::string);
	void setFigurePosition(sf::Vector2f startingPosition, sf::Vector2f finalPosition, sf::RenderWindow & window);
	void deletePaw(sf::Vector2f pawPosition);
	void transformFigure(sf::Vector2f pawPosition, int type, int team);
	void showPossibleMoves(std::string moves, sf::RenderWindow & window);
};

