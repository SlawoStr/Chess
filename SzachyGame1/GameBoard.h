#pragma once

#include <SFML/Graphics.hpp>

enum FIELD{FLD_EMPTY,FLD_PAW1,FLD_PAW2};
enum MESSAGE { MG_MOVE, MG_HIT };
enum CASTLING{ CAS_LEFT,CAS_RIGHT,CAS_BOTH,CAS_BLOCKED };


class Paw
{
private:
	sf::Vector2f position;
	int team;
	bool transformed;
public:
	Paw(sf::Vector2f position, int team, bool transformed) { this->position = position; this->team = team; this->transformed = transformed; }
	virtual bool checkMoves(FIELD gField[][8],std::string & moves)=0;
	virtual bool isKing() { return false; }
	bool checkPosition(sf::Vector2f position);
	int getTeam() { return team; }
	void setPosition(sf::Vector2f position) { this->position = position; }
	sf::Vector2f getPosition() { return position; }
	bool isTransformed() { return transformed; }
	virtual ~Paw() = 0;
};

class Knight : public Paw
{
public:
	Knight(sf::Vector2f position, int team,bool transformed);
	bool checkMoves(FIELD gField[][8], std::string & moves);
	int getType(){}
};

class Bishop : virtual public Paw
{
public:
	Bishop(sf::Vector2f, int team, bool transformed);
	bool checkMoves(FIELD gField[][8], std::string & moves);
};

class Rook : virtual public Paw
{
public:
	Rook(sf::Vector2f, int team, bool transformed);
	bool checkMoves(FIELD gField[][8], std::string & moves);
};

class King : public Paw
{
public:
	King(sf::Vector2f position,int team, bool transformed);
	bool checkMoves(FIELD gField[][8], std::string & moves);
	bool isKing() { return true; }
};


class ClassicPaw : public Paw
{
public:
	ClassicPaw(sf::Vector2f position, int team, bool transformed);
	bool checkMoves(FIELD gField[][8], std::string & moves);
};

class Queen : public Rook,public Bishop
{
public:
	Queen(sf::Vector2f position, int team, bool transformed);
	bool checkMoves(FIELD gField[][8], std::string & moves);
};




class GameBoard
{
private:
	FIELD gField[8][8];
	std::vector<Paw *>pawList;
	sf::Vector2f pickedPaw;


public:
	std::string giveMoves(sf::Vector2f, int team);
	void deletePaw(sf::Vector2f position,int team);
	void transformPaw(sf::Vector2f position, int type);
	MESSAGE setPawPosition(sf::Vector2f startingPosition, sf::Vector2f finalPosition);
	FIELD checkFieldType(sf::Vector2f position);
	sf::Vector2f getPickedPaw() { return pickedPaw; }
	void setPickedPaw(sf::Vector2f position) { pickedPaw = position; }
	bool isTransforamble(sf::Vector2f position, int & type);
	bool isFieldSafe(sf::Vector2f position, int team,int blockedPaw=100);
	bool testChangingPosition(sf::Vector2f startingPosition, sf::Vector2f finalPosition,int team);
	bool checkMovingPossibilities(int team);
	GameBoard();
	~GameBoard();
};