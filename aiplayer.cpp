#include "aiplayer.h"

AIPlayer::AIPlayer()
{
}


int AIPlayer::minMax(int *&board, int depth, Player *opponent, bool isMax, int &total_evaluated)
{
    if (depth == 0)
    {
        return evaluateState(board, total_evaluated, opponent);
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
    else {
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
}

int AIPlayer::alphaBeta(int *&board, int depth, Player *opponent, bool isMax, int &total_evaluated, int alpha, int beta)
{
    //return score for leaf node
    if(depth == 0)
    {
        if(heuristic == "complex")
        {
            return evaluateState(board, total_evaluated, opponent);
        }
        else if (heuristic == "medium") {
            //medium heuristic
        }
        else {
            //simple heuristic
        }
    }
    if(logic_board->getGamePhase() == 0)
    {
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
                    logic_board->undoAdd(moveTo, player_id);
                    if(res > best)
                        best = res;
                    if(best>alpha)
                        alpha = best;
                    if(beta <= alpha)
                        break;
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
                    logic_board->undoAdd(moveTo, opponent->getPlayerId());
                    if(res < best)
                        best=res;
                    if(best < beta)
                        beta = best;
                    if(beta <= alpha)
                        break;
                }
                return best;
            }
        }
    }
    std::cout <<"error\n";
    return 0;//error
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
    int alpha = -1000;
    int beta = 1000;
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
                res = minMax(board, DEPTH, opponent, false, total_evaluated);
                logic_board->undoRemovePiece(to_remove, player_id, opponent->getPlayerId());
            }
            else {
                res = minMax(board, DEPTH, opponent, false, total_evaluated);
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

int AIPlayer::evaluateStateSimple(int *board, int &total_evaluated)
{
    total_evaluated++;
    int own_pawns=0;
    int opponent_pawns=0;
    //board size = 24
    for(int i=0;i<24;i++)
    {
        if(board[i] == player_id)
            own_pawns++;
        else if(board[i] == 0){

        }
        else {
            opponent_pawns++;
        }
    }
    return own_pawns-opponent_pawns;


}

int AIPlayer::evaluateStateSimple2(int *board, int &total_evaluated, Player *opponent)
{
    //number of captured pawns my actual player - captured by opponent / all captured pawns
    total_evaluated++;
    int all_captured = getCapturedPawnsCount() + opponent->getCapturedPawnsCount();
    if(all_captured == 0)
        return 0;
    else {
        return ((getCapturedPawnsCount() - opponent->getCapturedPawnsCount()) /
                getCapturedPawnsCount() + opponent->getCapturedPawnsCount());
    }

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
    return ((2*own_pawns_on_board) - (2*opponent_pawns_on_board) + (4*my_mills) - (4*opponent_mills)
            - own_blocked + opponent_blocked + own_close_to_mill - opponent_close_to_mill);
}

