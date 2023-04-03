#include "GameBoard.h"
#include <iostream>
#include <list>

struct VectorComparator
{
	// Compare 2 Player objects using name
	bool operator ()(const sf::Vector2f & vec1, const sf::Vector2f & vec2)
	{
		if (vec1.y * 10 + vec1.x > vec2.y * 10 + vec2.x)
		{
			return true;
		}
		else
			return false;
	}
};


std::string GameBoard::giveMoves(sf::Vector2f position, int team)
{
	std::string napis;
	for (int i = 0; i < pawList.size(); i++)
	{
		if (pawList[i]->checkPosition(position))
		{
			pawList[i]->checkMoves(gField, napis);
			break;
		}
	}
	return napis;
}

void GameBoard::deletePaw(sf::Vector2f position,int team)
{
	int i = 0;
	for (auto it = pawList.begin(); it < pawList.end(); it++)
	{
		if (pawList[i]->checkPosition(position) && pawList[i]->getTeam()==team)
		{
			delete pawList[i];
			pawList.erase(it);
			break;
		}
		else
		{
			i++;
		}
	}
}

void GameBoard::transformPaw(sf::Vector2f position, int type)
{
	int i = 0;
	sf::Vector2f pawPosition;
	int team;
	for (auto it = pawList.begin(); it < pawList.end(); it++)
	{
		if (pawList[i]->checkPosition(position))
		{
			pawPosition = pawList[i]->getPosition();
			team = pawList[i]->getTeam();
			delete pawList[i];
			pawList.erase(it);
			switch (type)
			{
			case 0:
				pawList.push_back(new Rook(pawPosition, team,true));
				break;
			case 1:
				pawList.push_back(new Knight(pawPosition, team,true));
				break;
			case 2:
				pawList.push_back(new Bishop(pawPosition, team,true));
				break;
			case 3:
				pawList.push_back(new Queen(pawPosition, team,true));
				break;
			}
			break;
		}
		else
		{
			i++;
		}
	}
}

MESSAGE GameBoard::setPawPosition(sf::Vector2f startingPosition, sf::Vector2f finalPosition)
{
	MESSAGE gMessage;
	int team;
	for (int i = 0; i < pawList.size(); i++)
	{
		if (pawList[i]->checkPosition(startingPosition))
		{

			if (gField[(int)finalPosition.y - 1][(int)finalPosition.x - 1] ==FLD_EMPTY)
			{
				gMessage = MG_MOVE;
			}
			else
			{
				gMessage = MG_HIT;
			}
			pawList[i]->setPosition(finalPosition);
			gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] = FLD_EMPTY;
			gField[(int)finalPosition.y - 1][(int)finalPosition.x - 1] = (pawList[i]->getTeam() == 1 ? FLD_PAW1 : FLD_PAW2);
			break;
		}
	}
	return gMessage;
}

FIELD GameBoard::checkFieldType(sf::Vector2f position)
{
	return gField[(int)position.y - 1][(int)position.x - 1];
}

bool GameBoard::isTransforamble(sf::Vector2f position, int & type)
{
	for (int i = 0; i < pawList.size(); i++)
	{
		if (pawList[i]->checkPosition(position) && pawList[i]->isTransformed() == false)
		{
			std::cout << "Choose paw to swap" << std::endl;
			std::cin >> type;
			transformPaw(position, type);
			return true;
		}
	}
	return false;
}

bool GameBoard::isFieldSafe(sf::Vector2f position, int team,int blockedPaw)
{
	std::string blockedFields;
	for (int i = 0; i < pawList.size(); i++)
	{
		if (pawList[i]->getTeam() != team)
		{
			if (i == blockedPaw)
			{
				continue;
			}
			pawList[i]->checkMoves(gField, blockedFields);
		}
	}
	std::list<sf::Vector2f> moves;
	int k = 0;

	for (int i = 0; i < blockedFields.size(); i += 3)
	{
		int posX = blockedFields[i] - '0';
		int posY = blockedFields[i + 1] - '0';
		moves.push_back(sf::Vector2f(posX, posY));
	}

	moves.sort(VectorComparator());
	moves.unique();

	for (auto it = moves.begin(); it != moves.end(); it++)
	{
		if (position.x == it->x && position.y == it->y)
		{
			return false;
		}
	}
	return true;
}

bool GameBoard::testChangingPosition(sf::Vector2f startingPosition, sf::Vector2f finalPosition,int team)
{
	sf::Vector2f kingPosition;
	int pawNumber;
	FIELD secondPosition;
	bool sucess = false;
	for (int i = 0; i < pawList.size(); i++)
	{
		if (pawList[i]->isKing() && pawList[i]->getTeam() == team)
		{
			kingPosition = pawList[i]->getPosition();
		}
	}
	gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] = FLD_EMPTY;
	secondPosition = gField[(int)finalPosition.y - 1][(int)finalPosition.x - 1];
	gField[(int)finalPosition.y - 1][(int)finalPosition.x - 1] = (team == 1 ? FLD_PAW1 : FLD_PAW2);

	if (secondPosition == (team == 1 ? FLD_PAW2 : FLD_PAW1))
	{
		for (int i = 0; i < pawList.size(); i++)
		{
			if (pawList[i]->checkPosition(finalPosition))
			{
				pawNumber = i;
			}
		}
		if (kingPosition == startingPosition)
		{
			if (isFieldSafe(finalPosition, team, pawNumber))
			{
				sucess = true;
			}
		}
		else
		{
			if (isFieldSafe(kingPosition, team, pawNumber))
			{
				sucess = true;
			}
		}
	}
	else
	{
		if (kingPosition == startingPosition)
		{
			if (isFieldSafe(finalPosition, team))
			{
				sucess = true;
			}
		}
		else if (isFieldSafe(kingPosition, team))
		{
			sucess = true;
		}
	}
	gField[(int)startingPosition.y - 1][(int)startingPosition.x - 1] = (team == 1 ? FLD_PAW1 : FLD_PAW2);
	gField[(int)finalPosition.y - 1][(int)finalPosition.x - 1] = secondPosition;

	return sucess;
}

bool GameBoard::checkMovingPossibilities(int team)
{
	std::string possibleMoves;
	sf::Vector2f startingPosition;
	bool moved = false;
	for (int i = 0; i < pawList.size(); i++)
	{
		if (pawList[i]->getTeam() == team)
		{
			possibleMoves = "";
			startingPosition = pawList[i]->getPosition();
			pawList[i]->checkMoves(gField, possibleMoves);
			for (int k = 0; k < possibleMoves.size(); k += 3)
			{
				int posX = possibleMoves[k] - '0';
				int posY = possibleMoves[k + 1] - '0';
				if (testChangingPosition(startingPosition, sf::Vector2f(posX, posY), team))
				{
					moved = true;
					break;
				}
 			}
		}
	}
	return moved;
}

GameBoard::GameBoard()
{
	for (int i = 0; i < 8; i++)
	{
		gField[0][i] = FLD_PAW1;
		gField[1][i] = FLD_PAW1;
		gField[6][i] = FLD_PAW2;
		gField[7][i] = FLD_PAW2;
	}
	for (int i = 1; i < 9; i++)
	{
		pawList.push_back(new ClassicPaw(sf::Vector2f(i, 2), 1,false));
		pawList.push_back(new ClassicPaw(sf::Vector2f(i, 7), 2,false));
	}
	pawList.push_back(new Rook(sf::Vector2f(1, 1), 1, true));
	pawList.push_back(new Rook(sf::Vector2f(8, 1), 1, true));
	pawList.push_back(new Rook(sf::Vector2f(1, 8), 2, true));
	pawList.push_back(new Rook(sf::Vector2f(8, 8), 2, true));

	pawList.push_back(new Knight(sf::Vector2f(2, 1), 1, true));
	pawList.push_back(new Knight(sf::Vector2f(7, 1), 1, true));
	pawList.push_back(new Knight(sf::Vector2f(2, 8), 2, true));
	pawList.push_back(new Knight(sf::Vector2f(7, 8), 2, true));

	pawList.push_back(new Bishop(sf::Vector2f(3, 1), 1, true));
	pawList.push_back(new Bishop(sf::Vector2f(6, 1), 1, true));
	pawList.push_back(new Bishop(sf::Vector2f(3, 8), 2, true));
	pawList.push_back(new Bishop(sf::Vector2f(6, 8), 2, true));

	pawList.push_back(new Queen(sf::Vector2f(4, 1), 1, true));
	pawList.push_back(new Queen(sf::Vector2f(4, 8), 2, true));

	pawList.push_back(new King(sf::Vector2f(5, 1), 1, true));
	pawList.push_back(new King(sf::Vector2f(5, 8), 2, true));

}


GameBoard::~GameBoard()
{
	for (int i = 0; i < pawList.size(); i++)
		delete pawList[i];
}

bool Paw::checkPosition(sf::Vector2f position)
{
	if (this->position.x == position.x && this->position.y == position.y)
	{
		return true;
	}
	return false;
}

Paw::~Paw()
{

}


Knight::Knight(sf::Vector2f position, int team, bool transformed): Paw(position,team,transformed)
{
}

bool Knight::checkMoves(FIELD gField[][8], std::string & moves)
{
	sf::Vector2f currentPosition = getPosition();
	int team = getTeam() == FLD_PAW1 ? FLD_PAW1 : FLD_PAW2;

	std::list<sf::Vector2f>knightMoves;
	knightMoves.push_back(sf::Vector2f(currentPosition.x - 2, currentPosition.y + 1));
	knightMoves.push_back(sf::Vector2f(currentPosition.x - 2, currentPosition.y - 1));
	knightMoves.push_back(sf::Vector2f(currentPosition.x + 2, currentPosition.y + 1));
	knightMoves.push_back(sf::Vector2f(currentPosition.x + 2, currentPosition.y - 1));

	knightMoves.push_back(sf::Vector2f(currentPosition.x - 1, currentPosition.y + 2));
	knightMoves.push_back(sf::Vector2f(currentPosition.x - 1, currentPosition.y - 2));
	knightMoves.push_back(sf::Vector2f(currentPosition.x + 1, currentPosition.y + 2));
	knightMoves.push_back(sf::Vector2f(currentPosition.x + 1, currentPosition.y - 2));

	for (auto i = knightMoves.begin(); i != knightMoves.end(); i++)
	{
		if (i->x < 9 && i->x >0 && i->y < 9 && i->y > 0)
		{
			if (gField[(int)i->y - 1][(int)i->x - 1] != team)
			{
				moves += std::to_string((int)i->x);
				moves += std::to_string((int)i->y);
				moves += "/";
			}
		}
	}
	if (moves.size() > 0)
	{
		return true;
	}
	return false;
}

Bishop::Bishop(sf::Vector2f position, int team, bool transformed) : Paw(position,team,transformed)
{
}

bool Bishop::checkMoves(FIELD gField[][8], std::string & moves)
{
	sf::Vector2f currentPosition = getPosition();
	int team = getTeam() == FLD_PAW1 ? FLD_PAW1 : FLD_PAW2;
	std::list<sf::Vector2f>bishopMoves;
	/////////////////////////LEFT UP///////////////////////////////
	currentPosition.x--;
	currentPosition.y--;
	while (currentPosition.x > 0 && currentPosition.y > 0)
	{
		if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] == FLD_EMPTY)
		{
			bishopMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
		}
		else if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] != team)
		{
			bishopMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
			break;
		}
		else
			break;
		currentPosition.x--;
		currentPosition.y--;
	}
	///////////////////////////////LEFT DOWN///////////////////////////////////////
	currentPosition = getPosition();
	currentPosition.x--;
	currentPosition.y++;
	while (currentPosition.x > 0 && currentPosition.y < 9)
	{
		if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] == FLD_EMPTY)
		{
			bishopMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
		}
		else if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] != team)
		{
			bishopMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
			break;
		}
		else
			break;
		currentPosition.x--;
		currentPosition.y++;
	}
	//////////////////////////////////////////////RIGHT UP///////////////////////////
	currentPosition = getPosition();
	currentPosition.x++;
	currentPosition.y--;
	while (currentPosition.x < 9 && currentPosition.y > 0)
	{
		if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] == FLD_EMPTY)
		{
			bishopMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
		}
		else if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] != team)
		{
			bishopMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
			break;
		}
		else
			break;
		currentPosition.x++;
		currentPosition.y--;
	}
	/////////////////////////////////////////RIGHT DOWN/////////////////////////////////
	currentPosition = getPosition();
	currentPosition.x++;
	currentPosition.y++;
	while (currentPosition.x < 9 && currentPosition.y < 9)
	{

		if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] == FLD_EMPTY)
		{
			bishopMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
		}
		else if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] != team)
		{
			bishopMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
			break;
		}
		else
			break;
		currentPosition.x++;
		currentPosition.y++;
	}

	for (auto i = bishopMoves.begin(); i != bishopMoves.end(); i++)
	{
				moves += std::to_string((int)i->x);
				moves += std::to_string((int)i->y);
				moves += "/";
	}


	if (moves.size() > 0)
	{
		return true;
	}
	return false;
}

Rook::Rook(sf::Vector2f position, int team, bool transformed) : Paw(position,team,transformed)
{
}

bool Rook::checkMoves(FIELD gField[][8], std::string & moves)
{
	sf::Vector2f currentPosition = getPosition();
	int team = getTeam() == FLD_PAW1 ? FLD_PAW1 : FLD_PAW2;
	std::list<sf::Vector2f>rookMoves;

	///////////////////////////////UP///////////////////////////
	currentPosition.y--;
	while (currentPosition.y > 0)
	{
		if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] == FLD_EMPTY)
		{
			rookMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
		}
		else if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] != team)
		{
			rookMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
			break;
		}
		else
			break;
		currentPosition.y--;
	}
	/////////////////////////DOWN/////////////////////////////////////////////
	currentPosition = getPosition();
	currentPosition.y++;
	while (currentPosition.y < 9)
	{
		if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] == FLD_EMPTY)
		{
			rookMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
		}
		else if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] != team)
		{
			rookMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
			break;
		}
		else
			break;
		currentPosition.y++;
	}
	//////////////////////LEFT/////////////////////////////////////
	currentPosition = getPosition();
	currentPosition.x--;

	while (currentPosition.x > 0)
	{
		if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] == FLD_EMPTY)
		{
			rookMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
		}
		else if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] != team)
		{
			rookMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
			break;
		}
		else
			break;
		currentPosition.x--;
	}
	/////////////////////////////////////RIGHT///////////////////////////
	currentPosition = getPosition();
	currentPosition.x++;
	while (currentPosition.x < 9)
	{
		if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] == FLD_EMPTY)
		{
			rookMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
		}
		else if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] != team)
		{
			rookMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
			break;
		}
		else
			break;
		currentPosition.x++;
	}

	for (auto i = rookMoves.begin(); i != rookMoves.end(); i++)
	{
		moves += std::to_string((int)i->x);
		moves += std::to_string((int)i->y);
		moves += "/";
	}
	
	if (moves.size() > 0)
	{
		return true;
	}
	return false;
}

King::King(sf::Vector2f position, int team, bool transformed) : Paw(position, team,transformed)
{
}

bool King::checkMoves(FIELD gField[][8], std::string & moves)
{
	sf::Vector2f currentPosition = getPosition();
	int team = getTeam() == FLD_PAW1 ? FLD_PAW1 : FLD_PAW2;
	std::list<sf::Vector2f>kingMoves;

	kingMoves.push_back(sf::Vector2f(currentPosition.x - 1, currentPosition.y));
	kingMoves.push_back(sf::Vector2f(currentPosition.x - 1, currentPosition.y-1));
	kingMoves.push_back(sf::Vector2f(currentPosition.x - 1, currentPosition.y+1));
	kingMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y + 1));
	kingMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y - 1));
	kingMoves.push_back(sf::Vector2f(currentPosition.x + 1, currentPosition.y));
	kingMoves.push_back(sf::Vector2f(currentPosition.x + 1, currentPosition.y+1));
	kingMoves.push_back(sf::Vector2f(currentPosition.x + 1, currentPosition.y-1));

	for (auto i = kingMoves.begin(); i != kingMoves.end(); i++)
	{
		if (i->y < 9 && i->y>0 && i->x > 0 && i->x < 9)
		{
			if (gField[(int)i->y - 1][(int)i->x - 1] != team)
			{
				moves += std::to_string((int)i->x);
				moves += std::to_string((int)i->y);
				moves += "/";
			}
		}
	}
	if (moves.size() > 0)
	{
		return true;
	}
	return false;
}

ClassicPaw::ClassicPaw(sf::Vector2f position, int team, bool transformed) : Paw(position,team,transformed)
{

}

bool ClassicPaw::checkMoves(FIELD gField[][8], std::string & moves)
{
	sf::Vector2f currentPosition = getPosition();
	int team = getTeam() == FLD_PAW1 ? FLD_PAW1 : FLD_PAW2;
	int enemyTeam= getTeam() == FLD_PAW1 ? FLD_PAW2 : FLD_PAW1;
	int direction = team == FLD_PAW1 ? 1 : -1;
	std::list<sf::Vector2f>pawMoves;
	bool firstMove = false;
	if (team == FLD_PAW1 && currentPosition.y == 2)
		firstMove = true;
	if (team == FLD_PAW2 && currentPosition.y == 7)
		firstMove = true;
	currentPosition.y += direction;
	if (currentPosition.y < 9 && currentPosition.y > 0)
	{
		if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1]==FLD_EMPTY)
		{
			pawMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
			if (firstMove == true && gField[(int)currentPosition.y - 1 + direction][(int)currentPosition.x - 1] == FLD_EMPTY)
			{
				pawMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y+direction));

			}
		}
	}

	currentPosition = getPosition();
	currentPosition.x++;
	currentPosition.y+=direction;
	if (currentPosition.x < 9 && currentPosition.y>0 && currentPosition.y < 9)
	{
		if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] == enemyTeam)
		{
			pawMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
		}
	}
	currentPosition.x -= 2;
	if (currentPosition.x > 0 && currentPosition.y>0 && currentPosition.y < 9)
	{
		if (gField[(int)currentPosition.y - 1][(int)currentPosition.x - 1] == enemyTeam)
		{
			pawMoves.push_back(sf::Vector2f(currentPosition.x, currentPosition.y));
		}
	}

	for (auto i = pawMoves.begin(); i != pawMoves.end(); i++)
	{
			moves += std::to_string((int)i->x);
			moves += std::to_string((int)i->y);
			moves += "/";
	}

	if (moves.size() > 0)
	{
		return true;
	}
	return false;

}

Queen::Queen(sf::Vector2f position, int team, bool transformed) : Paw(position, team,transformed),Rook(position,team,transformed),Bishop(position,team,transformed)
{
}

bool Queen::checkMoves(FIELD gField[][8], std::string & moves)
{
	Rook::checkMoves(gField, moves);
	Bishop::checkMoves(gField, moves);

	if (moves.size() > 0)
	{
		return true;
	}
	return false;
}
