#pragma once

#ifndef LOGICBOARD_H
#define LOGICBOARD_H

#include "graphicboard.h"
#include "player.h"
#include "aiplayer.h"
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <memory>
#include <map>
#include <fstream>
#include <unistd.h>

//#define PLAYER1_ID 1
//#define PLAYER2_ID 2
#define BOARD_SIZE 24

class GraphicBoard;

class LogicBoard
{
public:
    LogicBoard();
    ~LogicBoard();
    void incrementTurn();

    //gameplay methods
    void addPiece(int pos);
    void addPiece(int pos, int player_turn);
    bool undoAdd(int moveTo, int player_id);
    void movePiece(int &move_from, int &pos);
    void movePiece(int &move_from, int &pos, int &player_id);
    void undoMovePiece(int move_from, int pos, int player_id);
    bool removePiece(int pos);
    bool removePiece(int pos, int player_id, int opponent_id);
    bool undoRemovePiece(int pos, int player_id, int opponent_id);
    void addPlayer(Player *&player);
    void endGame();


    // checking bool methods
    bool millIsBroken(int &move_from, int &pos);
    bool millIsBroken(int &move_from, int &pos, int &player_id);
    bool millFormed(int pos);
    bool millFormedNoAction(int turn, int pos);
    bool checkPosition(int pos);
    bool checkPositionToRemove(int pos);
    bool checkIsAllowedMove(int move_from, int pos);
    bool checkIsAllowedMoveFreely(int pos);
    bool checkIsInPlayerPawns(int pos);
    bool hasSecondPhaseEnded();
    bool hasLegalMove(Player *player);
    int gameHasEnded();
    bool firstPhaseHasEnded();

    // AI methods
    int getBestAdd();
    std::pair<int, int> getBestMove();

    //other
    void playerDataToFile();

    //getters, setters
    int getPlayerTurn();
    int getOpponentTurn();
    int getPiecesLeft(int turn);
    int getGamePhase();
    void setGamePhase(int game_phase);
    int getTurnCounter();
    void setMillJustFormed(bool formed);
    bool getMillJustFormed();


    int getPlayerPawnsCount();
    std::vector<Player*> *&getPlayers();
    std::map<int,std::vector<int>> getLegalMoves(int player_id);
    std::vector<int> getLegalAdd();
    void setPlayer(std::string type, int player_id);
    std::vector<int> getLegalPawnMoves(int player_id, int &pos);
    PlayerType getActualPlayerType();



private:
    int turn;
    int last_mill;
    int game_phase;
    int turn_counter;
    int *board;
    bool mill_just_formed;
    std::vector<std::vector<int>> adjacency_matrix;

    std::vector<Player*> *players;
    //Board contains information about pawns on the board.
    // 0 - field is empty
    // 1 - player1 occcupies the field, player_id = 1
    // 2 - player2 occupies the field, player_id = 2
    //Field are flatten into 1d array

    std::vector<int> &getFieldNeighbours(int pos);
    std::vector<std::vector<int>> &getAdjacencyMatrix();
    void updateTurnLabel(int turn);
    void inicializeBoard();
    void fillAdjacencyMatrix();
    void displayAdjacencyMatrix();

    std::array<std::array<int, 3>, 16> possibleMillPositions = {{
                                                                    {{0,1,2}},
                                                                    {{0,9,21}},
                                                                    {{1,4,7}},
                                                                    {{2,14,23}},
                                                                    {{3,4,5}},
                                                                    {{3,10,18}},
                                                                    {{5,13,20}},
                                                                    {{6,7,8}},
                                                                    {{6,11,15}},
                                                                    {{8,12,17}},
                                                                    {{9,10,11}},
                                                                    {{12,13,14}},
                                                                    {{15,16,17}},
                                                                    {{16,19,22}},
                                                                    {{18,19,20}},
                                                                    {{21,22,23}}
                                                                }};


};

#endif // LOGICBOARD_H
