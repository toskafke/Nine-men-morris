//#include "player.h"
//#include "aiplayer.h"
//#include "logicboard.h"

//int main(int argc, char *argv[])
//{
//    LogicBoard *logic_board = new LogicBoard();

//    Player *player1 = new AIPlayer();
//    Player *player2 = new Player();
//    logic_board->addPlayer(player1);
//    logic_board->addPlayer(player2);
//    player1->setPlayerId(PLAYER1_ID);
//    player2->setPlayerId(PLAYER2_ID);
//    player1->setAdjacencyMatrix(logic_board->getAdjacencyMatrix());
//    player2->setAdjacencyMatrix(logic_board->getAdjacencyMatrix());

//    dynamic_cast<AIPlayer*>(player1)->setLogicBoard(logic_board);

//    logic_board->addPiece(0);
//    logic_board->incrementTurn();
//    logic_board->addPiece(1);
//    logic_board->incrementTurn();
////    logic_board->incrementTurn();
////    logic_board->addPiece(2);
////    logic_board->incrementTurn();
////    logic_board->addPiece(9);
////    logic_board->incrementTurn();
////    logic_board->addPiece(3);
//    std::cout << player1->getCloseMill(logic_board->getBoard()) << "\n";;
//    std::cout << player1->getNumberOfPieces() << "\n";
//    std::cout << player2->getCloseMill(logic_board->getBoard()) << "\n";;
//    std::cout << player2->getNumberOfPieces() << "\n";

//    int own_blocked = 0;
//    std::list<int>my_pawns = player1->getPlayerPawns();
//    for (auto &pos: my_pawns)
//    {
//        if(logic_board->getLegalPawnMoves(player1->getPlayerId()-1, pos).size() == 0)
//            own_blocked++;
//    }


//    std::cout << "own blocked: "<<own_blocked << "\n";
//    int total_evaluated = 0;
//    std::cout<<"own close to mill: "<<dynamic_cast<AIPlayer*>(player1)->getCloseMill(logic_board->getBoard())<<"\n";
//    double heuristic1 = dynamic_cast<AIPlayer*>(player1)->evaluateStateSimple(logic_board->getBoard(), total_evaluated);
//    std::cout << "simple heuristic: "<<heuristic1<<"\n";
//    double heuristic2 = dynamic_cast<AIPlayer*>(player1)->evaluateStateSimple2(logic_board->getBoard(), total_evaluated, player2);
//    std::cout << "simple heuristic2: "<<heuristic2<<"\n";
//    double heuristic3 = dynamic_cast<AIPlayer*>(player1)->evaluateState(logic_board->getBoard(), total_evaluated, player2);
//    std::cout << "heuristic: "<<heuristic3<<"\n";

//    for(int i=0;i<24;i++)
//    {
//        std::cout << logic_board->getBoard()[i] << " ";
//    }
//    int result = dynamic_cast<AIPlayer*>(player1)->getAddPiecePos(logic_board->getBoard(), player2);
//    std::cout <<"result: "<<result<<"\n";
//    return 0;
//}
