#include "aiplayer.h"

AIPlayer::AIPlayer()
{
}


int AIPlayer::minMax(int *&board, int depth, Player *opponent, bool isMax, int &total_evaluated)
{
    if (depth == 0)
    {
        if(heuristic == "simple")
            return evaluateStateSimple(total_evaluated, opponent);
        else if (heuristic == "medium") {
            return evaluateStateMedium(total_evaluated, opponent)  ;
        }
        else {
            return evaluateState(board, total_evaluated, opponent);
        }
    }
    total_evaluated++;
    if(logic_board->getGamePhase() == 0)
    {
        std::vector<int> moves = logic_board->getLegalAdd();
        if(isMax)
        {
            int best = -1000;
            //for each available move
            for(auto it = moves.begin(); it != moves.end(); it++)
            {
                int moveTo = *it;
                logic_board->addPiece(moveTo, player_id);
                int res;
                if(logic_board->millFormedNoAction(player_id, moveTo))
                {
                    std::list<int> opponent_pawns = opponent->getPlayerPawns();
                    int first_pos = *opponent_pawns.begin();
                    logic_board->removePiece(first_pos);
                    res = minMax(board, depth-1, opponent, false
                                 , total_evaluated);
                    logic_board->undoRemovePiece(first_pos, player_id, opponent->getPlayerId());
                }
                else {
                    res = minMax(board, depth-1, opponent, false
                                 , total_evaluated);
                }
                if(res > best)
                    best = res;

                logic_board->undoAdd(moveTo, player_id);
            }
            return best;
        }
        //isMin
        else {
            int best = 1000;
            for(auto it = moves.begin(); it != moves.end(); it++)
            {
                int moveTo = *it;
                int res;
                logic_board->addPiece(moveTo, opponent->getPlayerId());
                if(logic_board->millFormedNoAction(opponent->getPlayerId(), moveTo))
                {
                    std::list<int> opponent_pawns = getPlayerPawns();
                    int first_pos = *opponent_pawns.begin();
                    logic_board->removePiece(first_pos, opponent->getPlayerId(), player_id);
                    res = minMax(board, depth-1, opponent, true
                                 , total_evaluated);
                    logic_board->undoRemovePiece(first_pos, opponent->getPlayerId(), player_id);
                }
                else {
                    res = minMax(board, depth-1, opponent, true,
                                 total_evaluated);
                }
                if(res < best)
                    best=res;
                logic_board->undoAdd(moveTo, opponent->getPlayerId());
            }
            return best;
        }
    }
    // game phase = 1 -> we are moving pieces
    else if (logic_board->getGamePhase() == 1){
        int res;
        if(isMax)
        {
            int best = -1000;
            std::map<int, std::vector<int>> moves = logic_board->getLegalMoves(player_id);
            for(auto it = moves.begin(); it != moves.end(); it++)
            {
                int move_from = it->first;
                std::vector<int> sub_moves = it->second;
                for(auto &move_to: sub_moves)
                {
                    logic_board->movePiece(move_from, move_to, player_id);
                    if(logic_board->millFormedNoAction(player_id, move_to))
                    {
                        std::list<int>opponent_pawns = opponent->getPlayerPawns();
                        int to_remove = *opponent_pawns.begin();
                        logic_board->removePiece(to_remove, player_id, opponent->getPlayerId());
                        res = minMax(board, depth-1, opponent, false, total_evaluated);
                        logic_board->undoRemovePiece(to_remove, player_id, opponent->getPlayerId());
                    }
                    else {
                        res = minMax(board, depth-1, opponent, false, total_evaluated);
                    }
                    logic_board->undoMovePiece(move_from, move_to, player_id);
                    if(res > best)
                    {
                        best = res;
                    }
                }
            }
            return best;
        }
        //isMin
        else {
            int best = 1000;
            std::map<int, std::vector<int>> moves = logic_board->getLegalMoves(opponent->getPlayerId());
            for(auto it = moves.begin(); it != moves.end(); it++)
            {
                int move_from = it->first;
                std::vector<int> sub_moves = it->second;
                for(auto &move_to: sub_moves)
                {
                    logic_board->movePiece(move_from, move_to, opponent->getPlayerId());
                    if(logic_board->millFormedNoAction(opponent->getPlayerId(), move_to))
                    {
                        std::list<int>opponent_pawns = getPlayerPawns();
                        int to_remove = *opponent_pawns.begin();
                        logic_board->removePiece(to_remove, opponent->getPlayerId(), player_id);
                        res = minMax(board, depth-1, opponent, true, total_evaluated);
                        logic_board->undoRemovePiece(to_remove, opponent->getPlayerId(), player_id);
                    }
                    else {
                        res = minMax(board, depth-1, opponent, true, total_evaluated);
                    }
                    logic_board->undoMovePiece(move_from, move_to, opponent->getPlayerId());
                    if(res < best)
                    {
                        best = res;
                    }
                }
            }
            return  best;
        }
    }
    std::cout << "error\n";
    return 0;
}

int AIPlayer::alphaBeta(int *&board, int depth, Player *opponent, bool isMax, int &total_evaluated, int alpha, int beta)
{
    //return score for leaf node
    if(depth == 0)
    {
        if(heuristic == "simple")
            return evaluateStateSimple(total_evaluated, opponent);
        else if (heuristic == "medium") {
            return evaluateStateMedium(total_evaluated, opponent)  ;
        }
        else {
            return evaluateState(board, total_evaluated, opponent);
        }
    }
    total_evaluated++;
    if(logic_board->getGamePhase() == 0)
    {
        std::vector<int> moves = logic_board->getLegalAdd();
        if(isMax)
        {
            int best = -1000;
            //for each available move
            for(auto it = moves.begin(); it != moves.end(); it++)
            {
                int moveTo = *it;
                logic_board->addPiece(moveTo, player_id);
                int res;
                if(logic_board->millFormedNoAction(player_id, moveTo))
                {
                    std::list<int> opponent_pawns = opponent->getPlayerPawns();
                    int first_pos = *opponent_pawns.begin();
                    logic_board->removePiece(first_pos);
                    res = alphaBeta(board, depth-1, opponent, false
                                 , total_evaluated, alpha, beta);
                    logic_board->undoRemovePiece(first_pos, player_id, opponent->getPlayerId());
                }
                else {
                    res = alphaBeta(board, depth-1, opponent, false
                                 , total_evaluated, alpha, beta);
                }
                logic_board->undoAdd(moveTo, player_id);
                if(res > best)
                    best = res;
                if(best>alpha)
                    alpha = best;
                if(beta <= alpha)
                    break; //pruning, cut off
            }
            return best;
        }
        //isMin
        else {
            int best = 1000;
            for(auto it = moves.begin(); it != moves.end(); it++)
            {
                int moveTo = *it;
                int res;
                logic_board->addPiece(moveTo, opponent->getPlayerId());
                if(logic_board->millFormedNoAction(opponent->getPlayerId(), moveTo))
                {
                    std::list<int> opponent_pawns = getPlayerPawns();
                    int first_pos = *opponent_pawns.begin();
                    logic_board->removePiece(first_pos, opponent->getPlayerId(), player_id);
                    res = alphaBeta(board, depth-1, opponent, true
                                 , total_evaluated, alpha, beta);
                    logic_board->undoRemovePiece(first_pos, opponent->getPlayerId(), player_id);
                }
                else {
                    res = alphaBeta(board, depth-1, opponent, true,
                                 total_evaluated, alpha, beta);
                }
                logic_board->undoAdd(moveTo, opponent->getPlayerId());
                if(res < best)
                    best=res;
                if(best < beta)
                    beta = best;
                if(beta <= alpha)
                    break; //pruning, cut off
            }
            return best;
        }
    }

    else if (logic_board->getGamePhase() == 1) {
        int res;
        if(isMax)
        {
            int best = -1000;
            std::map<int, std::vector<int>> moves = logic_board->getLegalMoves(player_id);
            for(auto it = moves.begin(); it != moves.end(); it++)
            {
                int move_from = it->first;
                std::vector<int> sub_moves = it->second;
                for(auto &move_to: sub_moves)
                {
                    logic_board->movePiece(move_from, move_to, player_id);
                    if(logic_board->millFormedNoAction(player_id, move_to))
                    {
                        std::list<int>opponent_pawns = opponent->getPlayerPawns();
                        int to_remove = *opponent_pawns.begin();
                        logic_board->removePiece(to_remove, player_id, opponent->getPlayerId());
                        res = alphaBeta(board, depth-1, opponent, false, total_evaluated, alpha, beta);
                        logic_board->undoRemovePiece(to_remove, player_id, opponent->getPlayerId());
                    }
                    else {
                        res = alphaBeta(board, depth-1, opponent, false, total_evaluated, alpha, beta);
                    }
                    logic_board->undoMovePiece(move_from, move_to, player_id);
                    if(res > best)
                        best = res;
                    if(best>alpha)
                        alpha = best;
                    if(beta <= alpha)
                        break;  //pruning, cut off
                }
            }
            return best;
        }
        //isMin
        else {
            int best = 1000;
            std::map<int, std::vector<int>> moves = logic_board->getLegalMoves(opponent->getPlayerId());
            for(auto it = moves.begin(); it != moves.end(); it++)
            {
                int move_from = it->first;
                std::vector<int> sub_moves = it->second;
                for(auto &move_to: sub_moves)
                {
                    logic_board->movePiece(move_from, move_to, opponent->getPlayerId());
                    if(logic_board->millFormedNoAction(opponent->getPlayerId(), move_to))
                    {
                        std::list<int>opponent_pawns = getPlayerPawns();
                        int to_remove = *opponent_pawns.begin();
                        logic_board->removePiece(to_remove, opponent->getPlayerId(), player_id);
                        res = alphaBeta(board, depth-1, opponent, true, total_evaluated, alpha, beta);
                        logic_board->undoRemovePiece(to_remove, opponent->getPlayerId(), player_id);
                    }
                    else {
                        res = alphaBeta(board, depth-1, opponent, true, total_evaluated, alpha, beta);
                    }
                    logic_board->undoMovePiece(move_from, move_to, opponent->getPlayerId());
                    if(res < best)
                        best = res;
                    if(best < beta)
                        beta = best;
                    if(beta <= alpha)
                        break; //pruning, cut off
                }
            }
            return  best;
        }
    }
    // TODO error logging
    else {
        std::cout << "error\n";
        return 0;
    }
}

int AIPlayer::getBestAdd(int *&board, Player *opponent)
{
    if(algorithm == "MiniMax")
    {
        return getBestAddMiniMax(board, opponent);
    }
    else {
        return getBestAddAlphaBeta(board, opponent);
    }
}

int AIPlayer::getBestAddMiniMax(int *&board, Player *opponent)
{
    int total_evaluated = 0;
    int res;
    int best_score = -1000;
    std::vector<int> moves = logic_board->getLegalAdd();
    int best_add = *moves.begin();
    for(auto it = moves.begin(); it != moves.end(); it++)
    {
        int pos = *it;
        logic_board->addPiece(pos, player_id);
        if(logic_board->millFormedNoAction(player_id, pos))
        {
            std::list<int>opponent_pawns = opponent->getPlayerPawns();
            int to_remove = *opponent_pawns.begin();
            logic_board->removePiece(to_remove, player_id, opponent->getPlayerId());
            res = minMax(board, DEPTH-2, opponent, false, total_evaluated);
            logic_board->undoRemovePiece(to_remove, player_id, opponent->getPlayerId());
        }
        else {
            res = minMax(board, DEPTH-2, opponent, false, total_evaluated);
        }
        logic_board->undoAdd(pos, player_id);
        if(res > best_score)
        {
            best_score = res;
            best_add = pos;
        }
    }
    return best_add;
}

int AIPlayer::getBestAddAlphaBeta(int *&board, Player *opponent)
{
    int total_evaluated = 0;
    int res;
    int best_score = -1000;
    int alpha = -10000;
    int beta = 10000;
    std::vector<int> moves = logic_board->getLegalAdd();
    int best_add = *moves.begin();
    for(auto it = moves.begin(); it != moves.end(); it++)
    {
        int pos = *it;
        logic_board->addPiece(pos, player_id);
        if(logic_board->millFormedNoAction(player_id, pos))
        {
            std::list<int>opponent_pawns = opponent->getPlayerPawns();
            int to_remove = *opponent_pawns.begin();
            logic_board->removePiece(to_remove, player_id, opponent->getPlayerId());
            res = alphaBeta(board, DEPTH-2, opponent, false, total_evaluated, alpha, beta);
            logic_board->undoRemovePiece(to_remove, player_id, opponent->getPlayerId());
        }
        else {
            res = alphaBeta(board, DEPTH-2, opponent, false, total_evaluated, alpha, beta);
        }
        logic_board->undoAdd(pos, player_id);
        if(res > best_score)
        {
            best_score = res;
            best_add = pos;
        }
    }
    return best_add;
}

std::pair<int, int> AIPlayer::getBestMove(int *&board, Player *opponent)
{
    if(algorithm == "MiniMax")
        return getBestMoveMiniMax(board, opponent, DEPTH);
    else {
        return getBestMoveAlphaBeta(board, opponent, DEPTH);
    }
}

std::pair<int, int> AIPlayer::getBestMoveMiniMax(int *&board, Player *opponent, int depth)
{
//    if(getNumberOfPieces() == 3)
//        depth=-2;
    int total_evaluated = 0;
    int res;
    int best_score = -1000;
    std::map<int, std::vector<int>> moves = logic_board->getLegalMoves(player_id);
    std::pair<int, int>best_move;

    for(auto &it: moves)
    {
        int move_from = it.first;
        std::vector<int> sub_moves = it.second;
        for(auto &move_to: sub_moves)
        {
            logic_board->movePiece(move_from, move_to, player_id);
            if(logic_board->millFormedNoAction(player_id, move_to))
            {
                std::list<int>opponent_pawns = opponent->getPlayerPawns();
                int to_remove = *opponent_pawns.begin();
                logic_board->removePiece(to_remove, player_id, opponent->getPlayerId());
                res = minMax(board, depth, opponent, false, total_evaluated);
                logic_board->undoRemovePiece(to_remove, player_id, opponent->getPlayerId());
            }
            else {
                res = minMax(board, depth, opponent, false, total_evaluated);
            }
            logic_board->undoMovePiece(move_from, move_to, player_id);
            if(res > best_score)
            {
                best_score = res;
                best_move={move_from, move_to};
            }
        }
    }
    return best_move;
}

std::pair<int, int> AIPlayer::getBestMoveAlphaBeta(int *&board, Player *opponent, int depth)
{

    int total_evaluated = 0;
    int res;
    int alpha = -1000;
    int beta = 1000;
    int best_score = -1000;
    std::map<int, std::vector<int>> moves = logic_board->getLegalMoves(player_id);
    std::pair<int, int>best_move;

    for(auto &it: moves)
    {
        int move_from = it.first;
        std::vector<int> sub_moves = it.second;
        for(auto &move_to: sub_moves)
        {
            logic_board->movePiece(move_from, move_to, player_id);
            if(logic_board->millFormedNoAction(player_id, move_to))
            {
                std::list<int>opponent_pawns = opponent->getPlayerPawns();
                int to_remove = *opponent_pawns.begin();
                logic_board->removePiece(to_remove, player_id, opponent->getPlayerId());
                if(getNumberOfPieces() == 3)
                    res = alphaBeta(board, depth-2, opponent, false, total_evaluated, alpha, beta);
                else {
                    res = alphaBeta(board, depth, opponent, false, total_evaluated, alpha, beta);

                }
                logic_board->undoRemovePiece(to_remove, player_id, opponent->getPlayerId());
            }
            else {
                if(getNumberOfPieces() == 3)
                    res = alphaBeta(board, depth-2, opponent, false, total_evaluated, alpha, beta);
                else {
                    res = alphaBeta(board, depth, opponent, false, total_evaluated, alpha, beta);

                }
            }
            logic_board->undoMovePiece(move_from, move_to, player_id);
            if(res > best_score)
            {
                best_score = res;
                best_move={move_from, move_to};
            }
        }
    }
    return best_move;
}


void AIPlayer::setLogicBoard(LogicBoard *&logic_board)
{
    this->logic_board = std::make_shared<LogicBoard>(*logic_board);

}
void AIPlayer::setLogicBoard(std::shared_ptr<LogicBoard> logic_board)
{
    this->logic_board = logic_board;

}

void AIPlayer::setHeuristic(std::string heuristic)
{
    this->heuristic=heuristic;
}

void AIPlayer::setAlgorithm(std::string algorithm)
{
    this->algorithm=algorithm;
}

int AIPlayer::evaluateStateSimple(int &total_evaluated, Player *opponent)
{
    total_evaluated++;
    return getNumberOfPieces() - opponent->getNumberOfPieces();
}

int AIPlayer::evaluateStateMedium(int &total_evaluated, Player *opponent)
{
    total_evaluated++;
    return getNumberOfPieces() - opponent->getNumberOfPieces() + getMillsCount() - opponent->getMillsCount();

}

int AIPlayer::evaluateState(int *&board, int &total_evaluated, Player *opponent)
{
    total_evaluated++;
    int own_pawns_on_board = getNumberOfPieces();
    int opponent_pawns_on_board = opponent->getNumberOfPieces();
    int my_mills = getMillsCount();
    int opponent_mills = opponent->getMillsCount();
    int own_blocked = 0;
    int opponent_blocked = 0;
    int own_close_to_mill = getCloseMill(board);
    int opponent_close_to_mill = opponent->getCloseMill(board);
    std::list<int>my_pawns = this->getPlayerPawns();
    for (auto &pos: my_pawns)
    {
        if(logic_board->getLegalPawnMoves(player_id, pos).size() == 0)
            own_blocked++;
    }
    std::list<int>opponent_pawns = opponent->getPlayerPawns();
    for (auto &pos: opponent_pawns)
    {
        if(logic_board->getLegalPawnMoves(opponent->getPlayerId(), pos).size() == 0)
            opponent_blocked++;
    }
    return ((2*own_pawns_on_board) - (2*opponent_pawns_on_board) + (3*my_mills) - (3*opponent_mills)
            - own_blocked + opponent_blocked + own_close_to_mill - opponent_close_to_mill);
}

