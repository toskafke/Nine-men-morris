#pragma once

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <player.h>
#include <array>
#include <vector>
#include <chrono>

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
    std::pair<int, int> getBestMoveMiniMax(int *&board, Player *opponent, int depth);
    std::pair<int, int> getBestMoveAlphaBeta(int *&board, Player *opponent, int depth);

    //getters, setters
    void setLogicBoard(LogicBoard *&logic_board);
    void setLogicBoard(std::shared_ptr<LogicBoard> logic_board);
    void setHeuristic(std::string heuristic);
    void setAlgorithm(std::string algorithm);
    std::string getHeuristic();
    std::string getAlgorithm();
    std::vector<std::array<int, 6>> getGameplayData();




//private:

    std::shared_ptr<LogicBoard>logic_board;

    //heuristic = own pawn - opponent pawns on board
    int evaluateStateSimple(int &total_evaluated, Player *opponent);

    //heuristic = captured pawns - captured pawns by opponent / all captured pawns
    int evaluateStateMedium(int &total_evaluated, Player *opponent);

    //
    int evaluateState(int *&board, int &total_evaluated, Player *opponent);

    std::string heuristic;
    std::string algorithm;

    // array of <game_phase, pawns_count, turn, time for getting piece, total_evaluated, depth>
    // TODO fill during gameplay
    std::vector<std::array<int, 6>> gameplay_data;


};


#endif // AIPLAYER_H
