#include "player.h"

Player::Player ()
{
    captured_pawns_count=0;
    pieces_to_put = 9;
}

Player::~Player()
{
    pawns_on_board.clear();
    pawns_in_mill.clear();
    adjacency_matrix.clear();
}

void Player::putPawn(int pos)
{
    pawns_on_board.push_back(pos);
}

bool Player::removePawn(int pos)
{
    pawns_on_board.remove(pos);
    return true;
}

std::list<int> Player::getPlayerPawns()
{
    return pawns_on_board;
}

void Player::updatePawns(int prev_pos, int actual_pos)
{
    pawns_on_board.remove(prev_pos);
    pawns_on_board.push_back(actual_pos);
}

void Player::setPlayerType(PlayerType type)
{
    player_type=type;
}

PlayerType &Player::getPlayerType()
{
    return this->player_type;
}

void Player::setPlayerId(const int &id)
{
    player_id=id;
}

int &Player::getPlayerId()
{
    return this->player_id;
}

void Player::incCapturedPawns()
{
    captured_pawns_count++;
}

void Player::decCapturedPawns()
{
    captured_pawns_count--;
}

void Player::incPiecesLeftToPut()
{
    pieces_to_put++;
}

void Player::decPiecesLeftToPut()
{
    pieces_to_put--;
}

int &Player::getCapturedPawnsCount()
{
    return this->captured_pawns_count;
}

int Player::getNumberOfPieces()
{
    return pawns_on_board.size();
}


int Player::getMillsCount()
{
    return this->pawns_in_mill.size();
}

void Player::addPawnsToMill(int &pos1, int &pos2, int &pos3)
{
    pawns_in_mill.push_back(std::array<int,3>{pos1,pos2,pos3});
}

bool Player::removeFromMill(int &pos)
{
    for(auto &arr: pawns_in_mill)
    {
        auto it = std::find(arr.begin(), arr.end(), pos);
        if (it != arr.end())
        {
            pawns_in_mill.remove(arr);
        }
    }
}

bool Player::isInMill(int &pos)
{
    bool inMill=false;
    for(auto it = pawns_in_mill.begin(); it != pawns_in_mill.end() && !inMill; it++)
    {
        auto itt = std::find((*it).begin(), (*it).end(), pos);
        if (itt != (*it).end())
        {
            inMill = true;
        }
    }
    return inMill;
}

void Player::setPossibleMills(std::array<std::array<int, 3>, 16> &possible_mills)
{
    this->possibleMillPositions = possible_mills;
}

void Player::setAdjacencyMatrix(std::vector<std::vector<int> > &adjacency_matrix)
{
    this->adjacency_matrix=adjacency_matrix;
}

bool Player::closeMill(int *board, int pos) {
    bool close_to_mill = false;
    std::vector<int> edges = adjacency_matrix.at(pos);
    for(auto it = edges.begin(); it != edges.end() && !close_to_mill; it++)
    {
        if(board[*it] == board[pos])
            close_to_mill=true;
    }

    return close_to_mill;
}


int Player::getCloseMill(int *board)
{
    int close_mill = 0;

    for(auto &pos: pawns_on_board)
    {
        if(closeMill(board, pos) && !isInMill(pos))
        {
            close_mill++;
        }
    }
    //we divide because we get twice as much close_mill as we have, one close mill,
    //so two next ot each other gives as two points
    return close_mill/2;
}

int Player::getPiecesLeftToPut()
{
    return pieces_to_put;
}
