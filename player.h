#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <array>
#include <algorithm>
#include <memory>
#include <vector>
#include <set>
#include <iostream>



enum PlayerType {Human, Computer};

class Player
{
public:
    Player();
    virtual ~Player() = default;
    void putPawn(int pos);
    bool removePawn(int pos);
    std::list<int> getPlayerPawns();
    void updatePawns(int prev_pos, int actual_pos);
    void setPlayerType(PlayerType type);
    PlayerType &getPlayerType();
    void setPlayerId(const int &id);
    int &getPlayerId();
    void incCapturedPawns();
    void decCapturedPawns();

    int &getCapturedPawnsCount();
    int getNumberOfPieces();
    int getMillsCount();
    void addPawnsToMill(int &pos1, int &pos2, int &pos3);
    bool removeFromMill(int &pos);
    bool isInMill(int &pos);
    bool closeMill(int *board, int pos);
    int getCloseMill(int *board);

    //getters, setters
    void setPossibleMills(std::array<std::array<int, 3>, 16> &possible_mills);
    void setAdjacencyMatrix(std::vector<std::vector<int>> &adjacency_matrix);

protected:
    int player_id;
    PlayerType player_type;
    std::list<int> pawns_on_board;
    int captured_pawns_count;
    std::list<std::array<int,3>> pawns_in_mill;

    std::array<std::array<int, 3>, 16> possibleMillPositions;
    std::vector<std::vector<int>> adjacency_matrix;




private:

};

#endif // PLAYER_H
