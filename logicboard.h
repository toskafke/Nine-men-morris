#pragma once

#ifndef LOGICBOARD_H
#define LOGICBOARD_H

#include "graphicboard.h"
#include <iostream>

class LogicBoard
{
public:
    LogicBoard();

    void incrementTurn();

    //getters, setters
    int getPlayerTurn();
    int getPiecesLeft(int turn);
    bool checkPosition(int pos);
    void addPiece(int pos);


private:
    int turn;
    int game_phase;
    int turn_counter;
    int *board;
    int first_player_pieces;
    int second_player_pieces;
    //Board contains information about pawns on the board.
    // 0 - field is empty
    // 1 - player1 occcupies the field
    // 2 - player2 occupies the field
    //Field are flatten into 1d array

    void updateTurnLabel(int turn);
    void inicializeBoard();


};

#endif // LOGICBOARD_H
