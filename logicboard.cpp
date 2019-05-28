#include "logicboard.h"


LogicBoard::LogicBoard()
{
    inicializeBoard();
    turn_counter = 0;
    turn = PLAYER1_ID;
    game_phase=0;
    first_player_pieces = 9;
    second_player_pieces = 9;
    mill_just_formed=false;
    players = new std::vector<Player*>();
    players->reserve(2);
    fillAdjacencyMatrix();
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

void LogicBoard::addPiece(int pos)
{
    board[pos] = turn;
    if(turn == PLAYER1_ID)
    {
        first_player_pieces--;
        players->at(0)->putPawn(pos);
    }
    else {
        second_player_pieces--;
        players->at(1)->putPawn(pos);
    }

}


bool LogicBoard::millFormed(int pos)
{
    bool mill_formed=false;
    for(auto it=possibleMillPositions.begin(); it != possibleMillPositions.end() && !mill_formed; it++)
    {
        int pos1=(*it)[0];
        int pos2=(*it)[1];
        int pos3=(*it)[2];
        if(board[pos1] == turn && board[pos2] == turn && board[pos3] == turn)
        {
            if(pos1 == pos || pos2 == pos || pos3 == pos)
            {
                mill_formed=true;
                mill_just_formed=true;
                players->at(turn-1)->addPawnsToMill(pos1, pos2, pos3);

            }
        }

    }
    return mill_formed;
}

bool LogicBoard::millFormedNoAction(int turn, int pos)
{
    bool mill_formed=false;
    for(auto it=possibleMillPositions.begin(); it != possibleMillPositions.end() && !mill_formed; it++)
    {
        int pos1=(*it)[0];
        int pos2=(*it)[1];
        int pos3=(*it)[2];
        if(board[pos1] == turn && board[pos2] == turn && board[pos3] == turn)
        {
            if(pos1 == pos || pos2 == pos || pos3 == pos)
            {
                mill_formed=true;
                //                mill_just_formed=true;
                players->at(turn-1)->addPawnsToMill(pos1, pos2, pos3);

            }
        }

    }
    return mill_formed;
}

int LogicBoard::getPlayerTurn()
{
    return this->turn;
}

int LogicBoard::getOpponentTurn()
{
    if(turn == PLAYER1_ID)
        return PLAYER2_ID;
    else {
        return PLAYER1_ID;
    }
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

bool LogicBoard::checkPositionToRemove(int pos)
{
    if(board[pos] == getOpponentTurn())
        return true;
    return false;
}

void LogicBoard::addPiece(int pos, int player_turn)
{
    board[pos] = player_turn;
    if(player_turn == PLAYER1_ID)
    {
        first_player_pieces--;
        players->at(0)->putPawn(pos);
    }
    else {
        second_player_pieces--;
        players->at(1)->putPawn(pos);
    }

}

bool LogicBoard::undoAdd(int moveTo, int player_id)
{
    board[moveTo] = 0;
    if(player_id == PLAYER1_ID)
    {
        first_player_pieces++;
        players->at(0)->removePawn(moveTo);
    }
    else {
        second_player_pieces++;
        players->at(1)->removePawn(moveTo);
    }
    if(players->at(player_id-1)->isInMill(moveTo))
    {
        players->at(player_id-1)->removeFromMill(moveTo);
    }
    return true;
}

bool LogicBoard::removePiece(int pos)
{
    board[pos] = 0;
    players->at(getOpponentTurn()-1)->removePawn(pos);
    players->at(turn-1)->incCapturedPawns();
    return true;
}

bool LogicBoard::removePiece(int pos, int player_id, int opponent_id)
{
    board[pos] = 0;
    players->at(opponent_id-1)->removePawn(pos);
    players->at(player_id-1)->incCapturedPawns();
    return true;
}

bool LogicBoard::undoRemovePiece(int pos, int player_id, int opponent_id)
{
    board[pos] = opponent_id;
    players->at(opponent_id-1)->putPawn(pos);
    players->at(player_id-1)->decCapturedPawns();

}

void LogicBoard::addPlayer(Player *&player)
{
    players->push_back(player);
}

bool LogicBoard::millIsBroken(int &move_from, int &pos)
{
    if(players->at(turn-1)->isInMill(move_from))
        return true;
    return false;
}

bool LogicBoard::millIsBroken(int &move_from, int &pos, int &player_id)
{
    if(players->at(player_id-1)->isInMill(move_from))
        return true;
    return false;
}

bool LogicBoard::checkIsAllowedMove(int move_from, int pos)
{
    if(board[pos] == 0)
    {
        std::vector<int> field_neighbours = getFieldNeighbours(move_from);
        auto it = std::find(field_neighbours.begin(), field_neighbours.end(), pos);
        if(it != field_neighbours.end())
            return true;
        return false;
    }
    else
    {
        return false;
    }
}

bool LogicBoard::checkIsAllowedMoveFreely(int pos)
{
    if(board[pos] == 0)
        return true;
    return false;
}

bool LogicBoard::checkIsInPlayerPawns(int pos)
{
    std::list<int> player_pawns = players->at(turn-1)->getPlayerPawns();
    auto it = std::find (player_pawns.begin(), player_pawns.end(), pos);
    if (it != player_pawns.end())
        return true;
    else {
        return false;
    }

}

bool LogicBoard::hasSecondPhaseEnded()
{
    for(int i=0;i<2;i++)
    {
        if(players->at(i)->getPlayerPawns().size() <= 3)
            return true;
    }
    return false;
}

bool LogicBoard::hasLegalMove(Player *player)
{
    bool can_move=false;
    std::list<int>player_pawns = player->getPlayerPawns();
    for(auto itt = player_pawns.begin(); itt != player_pawns.end() && !can_move; itt++)
    {
        std::vector<int>edges = adjacency_matrix.at(*itt);
        for(auto it = edges.begin(); it != edges.end() && !can_move; it++)
        {
            if(board[*it] == 0)
                can_move=true;
        }
    }

    return can_move;
}


int LogicBoard::getBestAdd()
{
    return dynamic_cast<AIPlayer*>(players->at(turn-1))->
            getBestAddMiniMax(board, players->at(getOpponentTurn()-1));
}

std::pair<int, int> LogicBoard::getBestMove()
{
    return dynamic_cast<AIPlayer*>(players->at(turn-1))->
            getBestMove(board, players->at(getOpponentTurn()-1));
}



void LogicBoard::movePiece(int &move_from, int &pos)
{
    board[move_from] = 0;
    board[pos] = turn;
    players->at(turn-1)->updatePawns(move_from, pos);
    if(millIsBroken(move_from, pos))
    {
        players->at(turn-1)->removeFromMill(move_from);
    }

}

void LogicBoard::movePiece(int &move_from, int &pos, int &player_id)
{
    board[move_from] = 0;
    board[pos] = player_id;
    players->at(player_id-1)->updatePawns(move_from, pos);
    if(millIsBroken(move_from, pos, player_id))
    {
        players->at(player_id-1)->removeFromMill(move_from);
    }
}

void LogicBoard::undoMovePiece(int move_from, int pos, int player_id)
{
    board[move_from] = player_id;
    board[pos] = 0;
    players->at(player_id-1)->updatePawns(pos, move_from);
    millFormedNoAction(player_id, move_from);
}

int LogicBoard::gameHasEnded()
{
    int end=-1;
    Player *player1 = players->at(0);
    Player *player2 = players->at(1);
    if(player1->getPlayerPawns().size() < 3 || !hasLegalMove(player1))
        end=PLAYER1_ID;
    else if(player2->getPlayerPawns().size() < 3 || !hasLegalMove(player2))
        end=PLAYER2_ID;
    return end;
}

std::vector<std::vector<int> > &LogicBoard::getAdjacencyMatrix()
{
    return this->adjacency_matrix;
}

int *&LogicBoard::getBoard()
{
    return this->board;
}

int LogicBoard::getGamePhase()
{
    return this->game_phase;
}

int LogicBoard::getTurnCounter()
{
    return this->turn_counter;
}

void LogicBoard::setMillJustFormed(bool formed)
{
    mill_just_formed=formed;
}

bool LogicBoard::getMillJustFormed()
{
    return this->mill_just_formed;
}


void LogicBoard::setGamePhase(int game_phase)
{
    this->game_phase=game_phase;
}



void LogicBoard::inicializeBoard()
{
    board = new int[24];
    for(int i=0;i<24;i++)
    {
        board[i] = 0;
    }
}


std::vector<int> &LogicBoard::getFieldNeighbours(int pos)
{
    return adjacency_matrix.at(pos);
}




void LogicBoard::fillAdjacencyMatrix()
{
    adjacency_matrix.reserve(24);
    adjacency_matrix.insert(adjacency_matrix.end(), {1,9});
    adjacency_matrix.insert(adjacency_matrix.end(),{0,2,4});
    adjacency_matrix.insert(adjacency_matrix.end(), {1,14});
    adjacency_matrix.insert(adjacency_matrix.end(),{4,10});
    adjacency_matrix.insert(adjacency_matrix.end(), {1,3,5,7});
    adjacency_matrix.insert(adjacency_matrix.end(),{4,13});
    adjacency_matrix.insert(adjacency_matrix.end(), {7,11});
    adjacency_matrix.insert(adjacency_matrix.end(),{4,6,8});
    adjacency_matrix.insert(adjacency_matrix.end(),{7,12});
    adjacency_matrix.insert(adjacency_matrix.end(),{0,10,21});

    adjacency_matrix.insert(adjacency_matrix.end(),{3,9, 11,18});
    adjacency_matrix.insert(adjacency_matrix.end(),{6,10,15});
    adjacency_matrix.insert(adjacency_matrix.end(),{8,13,17});
    adjacency_matrix.insert(adjacency_matrix.end(),{5,12,14,20});
    adjacency_matrix.insert(adjacency_matrix.end(),{2,13,23});
    adjacency_matrix.insert(adjacency_matrix.end(),{11,16});
    adjacency_matrix.insert(adjacency_matrix.end(),{15,17, 19});
    adjacency_matrix.insert(adjacency_matrix.end(),{12,16});
    adjacency_matrix.insert(adjacency_matrix.end(),{10,19});
    adjacency_matrix.insert(adjacency_matrix.end(),{16,18,20,22});

    adjacency_matrix.insert(adjacency_matrix.end(),{13,19});
    adjacency_matrix.insert(adjacency_matrix.end(),{9,22});
    adjacency_matrix.insert(adjacency_matrix.end(),{19,21,23});
    adjacency_matrix.insert(adjacency_matrix.end(),{14,22});
}

void LogicBoard::displayAdjacencyMatrix()
{
    for(auto it=adjacency_matrix.begin(); it!= adjacency_matrix.end(); it++)
    {
        std::vector<int> tmp_vec = *it;
        for(auto it2 = tmp_vec.begin(); it2 != tmp_vec.end();it2++)
        {
            std::cout << *it2 << ", ";
        }
        std::cout<<"\n";
    }

}

int LogicBoard::getPlayerPawnsCount()
{
    return players->at(turn-1)->getPlayerPawns().size();
}

std::vector<Player *> *&LogicBoard::getPlayers()
{
    return players;
}

std::map<int, std::vector<int> > LogicBoard::getLegalMoves(int player_id)
{
    std::map<int, std::vector<int>>legal_moves;
    std::list<int>player_pawns = players->at(player_id-1)->getPlayerPawns();
    for(auto &pawn: player_pawns)
    {
        std::vector<int>legal_pawn_moves = getLegalPawnMoves(player_id, pawn);
        if(!legal_pawn_moves.empty())
            legal_moves.insert({pawn, legal_pawn_moves});
    }
    return legal_moves;
}

std::vector<int> LogicBoard::getLegalAdd()
{
    std::vector<int>result;
    for(int i=0;i<BOARD_SIZE;i++)
        if(board[i] == 0)
            result.push_back(i);
    return result;
}

void LogicBoard::setPlayer(std::string type, int player_id)
{
    if (type == "Human")
    {
        Player *player = new Player();
        player->setPlayerId(player_id);
        player->setPlayerType(Human);
        player->setAdjacencyMatrix(getAdjacencyMatrix());
        player->setPossibleMills(possibleMillPositions);
        players->push_back(player);

    }
    else {
        Player *player = new AIPlayer();
        player->setPlayerId(player_id);
        player->setPlayerType(Computer);
        player->setPossibleMills(possibleMillPositions);

        player->setAdjacencyMatrix(getAdjacencyMatrix());

        players->push_back(player);

    }
}

PlayerType &LogicBoard::getCurrentPlayerType()
{
    return players->at(turn-1)->getPlayerType();
}

std::vector<int> LogicBoard::getLegalPawnMoves(int player_id, int &pos)
{
    std::vector<int>result;
    if(players->at(player_id-1)->getNumberOfPieces()>3)
    {
        std::vector<int>edges = adjacency_matrix.at(pos);
        for(auto const &e: edges)
        {
            if (board[e] == 0)
            {
                result.push_back(e);
            }
        }
    }
    // has 3 pawns
    else {
        for(int i=0;i<BOARD_SIZE;i++)
        {
            if(board[i] == 0)
                result.push_back(i);
        }
    }
    return result;

}

PlayerType LogicBoard::getActualPlayerType()
{
    return players->at(turn-1)->getPlayerType();
}
