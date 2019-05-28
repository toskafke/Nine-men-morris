#pragma once

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <player.h>
#include <array>
#include <vector>

#include <logicboard.h>

class LogicBoard;
class Player;

#define DEPTH 5

class AIPlayer : public Player
{
public:
    AIPlayer();

    int minMax(int *&board, int depth, Player *opponent, bool isMax,
               int &total_evaluated);
    int alphaBeta(int *&board, int depth, Player *opponent, bool isMax,
                  int &total_evaluated, int alpha, int beta);
    int getBestAdd(int *&board, Player *opponent);
    int getBestAddMiniMax(int *&board, Player *opponent);
    int getBestAddAlphaBeta(int *&board, Player *opponent);
    // pair<from, to>
    std::pair<int, int> getBestMove(int *&board, Player *opponent);

    //getters, setters

    void setLogicBoard(LogicBoard *&logic_board);
    void setLogicBoard(std::shared_ptr<LogicBoard> logic_board);
    void setHeuristic(std::string heuristic);
    void setAlgorithm(std::string algorithm);


//private:

    std::shared_ptr<LogicBoard>logic_board;

    //heuristic = own pawn - opponent pawns on board
    int evaluateStateSimple(int *board, int &total_evaluated);

    //heuristic = captured pawns - captured pawns by opponent / all captured pawns
    int evaluateStateSimple2(int *board, int &total_evaluated, Player *opponent);

    //
    int evaluateState(int *&board, int &total_evaluated, Player *opponent);

    std::string heuristic;
    std::string algorithm;

};


#endif // AIPLAYER_H
