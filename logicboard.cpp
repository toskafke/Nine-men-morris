#include "logicboard.h"


LogicBoard::LogicBoard()
{
    inicializeBoard();
    turn_counter = 0;
    turn = PLAYER1_ID;
    first_player_pieces = 9;
    second_player_pieces = 9;
}

void LogicBoard::incrementTurn()
{
    if (turn == PLAYER1_ID)
        turn = PLAYER2_ID;
    else {
        turn = PLAYER1_ID;
    }
    turn_counter++;
}

int LogicBoard::getPlayerTurn()
{
    return this->turn;
}

int LogicBoard::getPiecesLeft(int turn)
{
    if(turn == PLAYER1_ID)
        return this->first_player_pieces;
    else {
        return this->second_player_pieces;
    }
}

bool LogicBoard::checkPosition(int pos)
{
    if(board[pos] != 0)
        return false;

    return true;

}

void LogicBoard::addPiece(int pos)
{
    board[pos] = turn;
    if(turn == PLAYER1_ID)
        first_player_pieces--;
    else {
        second_player_pieces--;
    }
    incrementTurn();
    for(int i=0;i<24;i++)
        std::cout<<board[i] << ", ";
}



void LogicBoard::inicializeBoard()
{
    board = new int[24];
    for(int i=0;i<24;i++)
    {
        board[i] = 0;
    }
}
