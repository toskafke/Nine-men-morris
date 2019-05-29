#include "graphicboard.h"

#include <QMessageBox>
#include <QSignalMapper>
#include <unistd.h>

GraphicBoard::GraphicBoard(QWidget *parent) : QWidget(parent), timeElapsed(0), moveFrom(-1)
{
    signalMapper = new QSignalMapper(this);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(pointSelected(int)));

    for(int i = 0; i < 24; i++)
    {
        buttons[i] = new QPushButton("", this);
        connect(buttons[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(buttons[i], i);
        buttons[i]->setFixedHeight(50);
        buttons[i]->setFixedWidth(50);
        buttons[i]->setObjectName("empty");
        buttons[i]->setEnabled(false);
    }
    // Set stylesheet
    setPlaceHoverStylesheet();

    boardLayout = new QGridLayout();
    statusLayout = new QGridLayout();

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timeHandler()));

    // Set up the status layout
    // Displays: status message, current turn, current game phase rules, reset button

    //    statsLabel = new QLabel(tr("Stats:"));
    //    statsLabel->setObjectName("boldLabel");
    //    statusLayout->addWidget(statsLabel, 0, 0, 1, 2);

    firstPlayerSelectTextLabel = new QLabel(tr("Player 1:"));
    firstPlayerSelectTextLabel->setObjectName("boldLabel");
    statusLayout->addWidget(firstPlayerSelectTextLabel, 0, 0);
    firstPlayerSelect = new QComboBox();
    firstPlayerSelect->addItem(tr("Human"));
    firstPlayerSelect->addItem(tr("Computer"));
    statusLayout->addWidget(firstPlayerSelect, 0, 1);

    heuristicSelectTextLabel = new QLabel(tr("Heuristic:"));
    heuristicSelectTextLabel->setObjectName("boldLabel");
    statusLayout->addWidget(heuristicSelectTextLabel, 1, 0);
    heuristicSelect = new QComboBox();
    heuristicSelect->addItem(tr("Simple"));
    heuristicSelect->addItem(tr("Medium"));
    heuristicSelect->addItem(tr("Complex"));
    statusLayout->addWidget(heuristicSelect, 1, 1);

    algorithmSelectTextLabel = new QLabel(tr("Algorithm:"));
    algorithmSelectTextLabel->setObjectName("boldLabel");
    statusLayout->addWidget(algorithmSelectTextLabel, 2, 0);
    algorithmSelect = new QComboBox();
    algorithmSelect->addItem(tr("MiniMax"));
    algorithmSelect->addItem(tr("AlphaBeta"));
    statusLayout->addWidget(algorithmSelect, 2, 1);

    secondPlayerSelectTextLabel = new QLabel(tr("Player 2:"));
    secondPlayerSelectTextLabel->setObjectName("boldLabel");
    statusLayout->addWidget(secondPlayerSelectTextLabel, 3, 0);
    secondPlayerSelect = new QComboBox();
    secondPlayerSelect->addItem(tr("Human"));
    secondPlayerSelect->addItem(tr("Computer"));
    statusLayout->addWidget(secondPlayerSelect, 3, 1);

    heuristicSelectTextLabel2 = new QLabel(tr("Heuristic:"));
    heuristicSelectTextLabel2->setObjectName("boldLabel");
    statusLayout->addWidget(heuristicSelectTextLabel2, 4, 0);
    heuristicSelect2 = new QComboBox();
    heuristicSelect2->addItem(tr("Simple"));
    heuristicSelect2->addItem(tr("Medium"));
    heuristicSelect2->addItem(tr("Complex"));
    statusLayout->addWidget(heuristicSelect2, 4, 1);

    algorithmSelectTextLabel2 = new QLabel(tr("Algorithm:"));
    algorithmSelectTextLabel2->setObjectName("boldLabel");
    statusLayout->addWidget(algorithmSelectTextLabel2, 5, 0);
    algorithmSelect2 = new QComboBox();
    algorithmSelect2->addItem(tr("MiniMax"));
    algorithmSelect2->addItem(tr("AlphaBeta"));
    statusLayout->addWidget(algorithmSelect2, 5, 1);


    startButton = new QPushButton(tr("Start game"));
    connect(startButton, SIGNAL(clicked()), SLOT(startGame()));
    statusLayout->addWidget(startButton, 7, 0);

    resetButton = new QPushButton(tr("Restart game"));
    //    connect(resetButton, SIGNAL(clicked()), SLOT(resetGame()));
    statusLayout->addWidget(resetButton, 7, 1);

    turnTextLabel = new QLabel(tr("Current turn:"));
    turnTextLabel->setObjectName("boldLabel");
    statusLayout->addWidget(turnTextLabel, 9, 0);
    turnLabel = new QLabel(QString::number(0));
    statusLayout->addWidget(turnLabel, 9, 1);

    timeElapsedTextLabel = new QLabel(tr("Current time:"));
    timeElapsedTextLabel->setObjectName("boldLabel");
    statusLayout->addWidget(timeElapsedTextLabel, 10, 0);
    mTimeElapsed = new QLabel();
    statusLayout->addWidget(mTimeElapsed, 10, 1);

    statusTextLabel = new QLabel(tr("Status messages:"));
    statusTextLabel->setObjectName("boldLabel");
    statusLayout->addWidget(statusTextLabel, 12, 0, 1, 2);

    statusList = new QListWidget;
    statusList->setWordWrap(true);
    statusList->addItem(tr("A new game has started."));
    statusLayout->addWidget(statusList, 13, 0, 1, 2);

    spacerItem1 = new QSpacerItem(400, 100);
    statusLayout->addItem(spacerItem1, 14, 0, 1, 2);

    spacerItem2 = new QSpacerItem(400, 75);
    statusLayout->addItem(spacerItem2, 12, 0, 1, 2);

    // Setup the board layout
    boardLayout->setSpacing(0);

    // Step 1/4: Add the row and column labels
    for(int i = 7; i > 0; i--) {
        rowLabels[i-1] = new QLabel(QString::number(i));
        rowLabels[i-1]->setStyleSheet("color: grey; text-align: center; padding: 5px;");
        boardLayout->addWidget(rowLabels[i-1], (7-i)*2, 0);
    }

    for(int i = 65; i < 72; i++) {
        char ch = i;
        QString str = QString(ch);
        colLabels[i-65] = new QLabel(str);
        colLabels[i-65]->setAlignment(Qt::AlignCenter);
        colLabels[i-65]->setStyleSheet("color: grey; text-align: center; padding: 5px;");
        boardLayout->addWidget(colLabels[i-65], 13, (i-64)*2-1);
    }

    // Step 2/4: Add the buttons

    boardLayout->addWidget(buttons[0], 0, 1);
    boardLayout->addWidget(buttons[1], 0, 7);
    boardLayout->addWidget(buttons[2], 0, 13);

    boardLayout->addWidget(buttons[3], 2, 3);
    boardLayout->addWidget(buttons[4], 2, 7);
    boardLayout->addWidget(buttons[5], 2, 11);

    boardLayout->addWidget(buttons[6], 4, 5);
    boardLayout->addWidget(buttons[7], 4, 7);
    boardLayout->addWidget(buttons[8], 4, 9);

    boardLayout->addWidget(buttons[9], 6, 1);
    boardLayout->addWidget(buttons[10], 6, 3);
    boardLayout->addWidget(buttons[11], 6, 5);
    boardLayout->addWidget(buttons[12], 6, 9);
    boardLayout->addWidget(buttons[13], 6, 11);
    boardLayout->addWidget(buttons[14], 6, 13);

    boardLayout->addWidget(buttons[15], 8, 5);
    boardLayout->addWidget(buttons[16], 8, 7);
    boardLayout->addWidget(buttons[17], 8, 9);

    boardLayout->addWidget(buttons[18], 10, 3);
    boardLayout->addWidget(buttons[19], 10, 7);
    boardLayout->addWidget(buttons[20], 10, 11);

    boardLayout->addWidget(buttons[21], 12, 1);
    boardLayout->addWidget(buttons[22], 12, 7);
    boardLayout->addWidget(buttons[23], 12, 13);

    // Step 3/4: Add the vertical lines

    QLabel *verticalLine [40];

    for(int i = 0; i < 40; i++) {
        verticalLine[i] = new QLabel();
        verticalLine[i]->setPixmap(QPixmap(":/images/vertical.png"));
    }

    boardLayout->addWidget(verticalLine[0],1,1);
    boardLayout->addWidget(verticalLine[1],1,7);
    boardLayout->addWidget(verticalLine[2],1,13);

    boardLayout->addWidget(verticalLine[3],2,1);
    boardLayout->addWidget(verticalLine[4],2,13);

    boardLayout->addWidget(verticalLine[5],3,1);
    boardLayout->addWidget(verticalLine[6],3,3);
    boardLayout->addWidget(verticalLine[7],3,7);
    boardLayout->addWidget(verticalLine[8],3,11);
    boardLayout->addWidget(verticalLine[9],3,13);

    boardLayout->addWidget(verticalLine[10],4,1);
    boardLayout->addWidget(verticalLine[11],4,3);
    boardLayout->addWidget(verticalLine[12],4,11);
    boardLayout->addWidget(verticalLine[13],4,13);

    boardLayout->addWidget(verticalLine[14],5,1);
    boardLayout->addWidget(verticalLine[15],5,3);
    boardLayout->addWidget(verticalLine[16],5,5);
    boardLayout->addWidget(verticalLine[17],5,9);
    boardLayout->addWidget(verticalLine[18],5,11);
    boardLayout->addWidget(verticalLine[19],5,13);

    boardLayout->addWidget(verticalLine[20],7,1);
    boardLayout->addWidget(verticalLine[21],7,3);
    boardLayout->addWidget(verticalLine[22],7,5);
    boardLayout->addWidget(verticalLine[23],7,9);
    boardLayout->addWidget(verticalLine[24],7,11);
    boardLayout->addWidget(verticalLine[25],7,13);

    boardLayout->addWidget(verticalLine[26],8,1);
    boardLayout->addWidget(verticalLine[27],8,3);
    boardLayout->addWidget(verticalLine[28],8,11);
    boardLayout->addWidget(verticalLine[29],8,13);

    boardLayout->addWidget(verticalLine[30],9,1);
    boardLayout->addWidget(verticalLine[31],9,3);
    boardLayout->addWidget(verticalLine[32],9,7);
    boardLayout->addWidget(verticalLine[33],9,11);
    boardLayout->addWidget(verticalLine[34],9,13);

    boardLayout->addWidget(verticalLine[35],10,1);
    boardLayout->addWidget(verticalLine[36],10,13);

    boardLayout->addWidget(verticalLine[37],11,1);
    boardLayout->addWidget(verticalLine[38],11,7);
    boardLayout->addWidget(verticalLine[39],11,13);

    // Step 4/4: Add the horizontal lines

    QLabel *horizontalLine [40];

    for(int i = 0; i < 40; i++) {
        horizontalLine[i] = new QLabel();
        horizontalLine[i]->setPixmap(QPixmap(":/images/horizontal.png"));
    }

    boardLayout->addWidget(horizontalLine[0],0,2);
    boardLayout->addWidget(horizontalLine[1],0,3);
    boardLayout->addWidget(horizontalLine[2],0,4);
    boardLayout->addWidget(horizontalLine[3],0,5);
    boardLayout->addWidget(horizontalLine[4],0,6);
    boardLayout->addWidget(horizontalLine[5],0,8);
    boardLayout->addWidget(horizontalLine[6],0,9);
    boardLayout->addWidget(horizontalLine[7],0,10);
    boardLayout->addWidget(horizontalLine[8],0,11);
    boardLayout->addWidget(horizontalLine[9],0,12);

    boardLayout->addWidget(horizontalLine[10],2,4);
    boardLayout->addWidget(horizontalLine[11],2,5);
    boardLayout->addWidget(horizontalLine[12],2,6);
    boardLayout->addWidget(horizontalLine[13],2,8);
    boardLayout->addWidget(horizontalLine[14],2,9);
    boardLayout->addWidget(horizontalLine[15],2,10);

    boardLayout->addWidget(horizontalLine[16],4,6);
    boardLayout->addWidget(horizontalLine[17],4,8);

    boardLayout->addWidget(horizontalLine[18],6,2);
    boardLayout->addWidget(horizontalLine[19],6,4);
    boardLayout->addWidget(horizontalLine[20],6,10);
    boardLayout->addWidget(horizontalLine[21],6,12);

    boardLayout->addWidget(horizontalLine[22],8,6);
    boardLayout->addWidget(horizontalLine[23],8,8);

    boardLayout->addWidget(horizontalLine[24],10,4);
    boardLayout->addWidget(horizontalLine[25],10,5);
    boardLayout->addWidget(horizontalLine[26],10,6);
    boardLayout->addWidget(horizontalLine[27],10,8);
    boardLayout->addWidget(horizontalLine[28],10,9);
    boardLayout->addWidget(horizontalLine[29],10,10);

    boardLayout->addWidget(horizontalLine[30],12,2);
    boardLayout->addWidget(horizontalLine[31],12,3);
    boardLayout->addWidget(horizontalLine[32],12,4);
    boardLayout->addWidget(horizontalLine[33],12,5);
    boardLayout->addWidget(horizontalLine[34],12,6);
    boardLayout->addWidget(horizontalLine[35],12,8);
    boardLayout->addWidget(horizontalLine[36],12,9);
    boardLayout->addWidget(horizontalLine[37],12,10);
    boardLayout->addWidget(horizontalLine[38],12,11);
    boardLayout->addWidget(horizontalLine[39],12,12);

    mainLayout = new QHBoxLayout();
    spacer2 = new QSpacerItem(50,100);
    mainLayout->addLayout(boardLayout);
    mainLayout->addSpacerItem(spacer2);
    mainLayout->addLayout(statusLayout);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(mainLayout);
}

void GraphicBoard::timeHandler()
{
    timeElapsed++;
    mTimeElapsed->setText(QString::number(timeElapsed/10));
}

void GraphicBoard::incTurn() {
    logic_board->incrementTurn();
}

void GraphicBoard::incGamePhase() {
    int game_phase=logic_board->getGamePhase();
    logic_board->setGamePhase(game_phase+1);
    updateGameRulesLabel(tr(gameRules[game_phase+1]));
    updateStatusLabel(tr("Game phase %n begins.", 0, gamePhase+1));
}

void GraphicBoard::setLogicBoard(LogicBoard *&logic_b)
{
    this->logic_board = std::make_shared<LogicBoard>(*logic_b);
}

/*!
 *  Set stylesheet methods
 */

void GraphicBoard::setPlaceHoverStylesheet()
{
    // The player is selecting a point to place a piece
    qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } "
                        "QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } "
                        "QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } "
                        "QPushButton#selected_player1 { border: 0px; background-image: url(:/images/blue_stone_selected.png); } "
                        "QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } "
                        "QPushButton#selected_player2 { border: 0px; background-image: url(:/images/red_stone_selected.png); } "
                        "QPushButton#empty:hover { border: 0px; background-image: url(:/images/grey_stone_selected.png); } "
                        "QPushButton#selected:hover { border: 0px; background-image: url(:/images/blue_stone.png); }");
}

void GraphicBoard::setMoveHoverStylesheet()
{
    // The player is selecting one of his own pieces to move
    qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } "
                        "QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } "
                        "QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } "
                        "QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } "
                        "QPushButton#player1:hover { border: 0px; background-image: url(:/images/blue_stone_selected.png); }"
                        "QPushButton#player2:hover { border: 0px; background-image: url(:/images/red_stone_selected.png); }");
}

void GraphicBoard::setRemoveHoverStylesheet()
{
    // The player is selecting one of the opposing player's pieces to remove
    qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } "
                        "QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } "
                        "QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } "
                        "QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } "
                        "QPushButton#player2:hover { border: 0px; background-image: url(:/images/red_stone_selected.png); }");
}

void GraphicBoard::startGame()
{
    std::array<std::string, 2> heuristic_arr;
    heuristic_arr[0] = heuristicSelect->currentText().toStdString();
    heuristic_arr[1] = heuristicSelect2->currentText().toStdString();
    heuristicSelect->setEnabled(false);
    heuristicSelect2->setEnabled(false);
    std::array<std::string, 2>algorithm_arr;
    algorithm_arr[0] = algorithmSelect->currentText().toStdString();
    algorithm_arr[1] = algorithmSelect2->currentText().toStdString();
    algorithmSelect->setEnabled(false);
    algorithmSelect2->setEnabled(false);
    std::string player1_type = firstPlayerSelect->currentText().toStdString();
    std::string player2_type = secondPlayerSelect->currentText().toStdString();
    firstPlayerSelect->setEnabled(false);
    secondPlayerSelect->setEnabled(false);
    logic_board->setPlayer(player1_type, PLAYER1_ID);
    logic_board->setPlayer(player2_type, PLAYER2_ID);
    std::vector<Player*> *players = logic_board->getPlayers();
    for(int i=0;i<2;i++)
    {
        if(players->at(i)->getPlayerType() == Computer)
        {
            auto sp_logic_board = logic_board;
            dynamic_cast<AIPlayer*>(players->at(i))->setLogicBoard(sp_logic_board);
            dynamic_cast<AIPlayer*>(players->at(i))->setHeuristic(heuristic_arr[i]);
            dynamic_cast<AIPlayer*>(players->at(i))->setAlgorithm(algorithm_arr[i]);
        }
    }
    for(int i = 0; i < 24; i++)
    {
        buttons[i]->setEnabled(true);
    }
    startButton->setEnabled(false);

    timer->start(100);

    if(player1_type == "Computer" && player2_type == "Computer")
        pointSelected(0);
}

/*!
 *  UI update methods
 */

void GraphicBoard::updateTurnLabel(int turn)
{
    turnLabel->setText(QString::number(turn));
}

void GraphicBoard::updateGameRulesLabel(QString str)
{
    gameRulesLabel->setText(str);
}


void GraphicBoard::updateStatusLabel(QString str)
{
    statusList->insertItem(0, str);
    statusList->repaint();
}

void GraphicBoard::showErrorMessage(QString str)
{
    QMessageBox messageBox;
    messageBox.critical(0,tr("Error"),str);
    messageBox.setFixedSize(500,200);
}

void GraphicBoard::addPiece(const int &pos, int &game_phase)
{
    if(logic_board->checkPosition(pos))
    {
        addPieceOnBoard(pos);
        logic_board->addPiece(pos);

        //checking if player formed a mill
        if(!logic_board->millFormed(pos))
        {
            //if not we change the turn, if yes, it's still the same player turn - he has to remove a piece
            logic_board->incrementTurn();
        }
    }
    if(logic_board->firstPhaseHasEnded())
    {
        setMoveHoverStylesheet();
        logic_board->setGamePhase(game_phase+1);
    }
}

void GraphicBoard::addPieceOnBoard(const int &pos)
{

    int player_turn = logic_board->getPlayerTurn();
    if(player_turn == PLAYER1_ID)
    {
        buttons[pos]->setObjectName("player1");
        updateStatusLabel(tr("Player %1 have placed a piece and has %2 piece(s) left.")
                          .arg(player_turn).arg(logic_board->getPiecesLeft(player_turn) - 1));
    }
    else if(player_turn == PLAYER2_ID)
    {
        buttons[pos]->setObjectName("player2");
        updateStatusLabel(tr("Player %1 have placed a piece and has %2 piece(s) left.")
                          .arg(player_turn).arg(logic_board->getPiecesLeft(player_turn) - 1));
    }

    buttons[pos]->setStyle(qApp->style());
    buttons[pos]->repaint();
}

void GraphicBoard::removePiece(const int &pos)
{

    int turn = logic_board->getPlayerTurn();
    // Update button stylesheet
    buttons[pos]->setObjectName("empty");

    updateStatusLabel(tr("Player %1 has removed a piece").arg(turn));
    buttons[pos]->setStyle(qApp->style());
    statusList->repaint();

    //     Reset stylesheet
    int game_phase = logic_board->getGamePhase();
    if(game_phase == 0)
    {
        setPlaceHoverStylesheet();
    }
    else
    {
        setMoveHoverStylesheet();
    }
}

void GraphicBoard::movePiece(int &moveFrom, int &pos, int &turn)
{
    //player clicked first button, now he has to click second one
    if(moveFrom == -1)
    {
        if(logic_board->checkIsInPlayerPawns(pos))
        {
            moveFrom=pos;
            if(turn==PLAYER1_ID)
                buttons[pos]->setObjectName("selected_player1");
            else {
                buttons[pos]->setObjectName("selected_player2");
            }
            // Set stylesheet
            setPlaceHoverStylesheet();
        }
    }
    //player clicked second button, we are moving the pawn
    else if(pos != moveFrom){
        bool allowed=false;
        //checking if the move is allowed
        if(logic_board->getPlayerPawnsCount()==3)
        {
            if(logic_board->checkIsAllowedMoveFreely(pos))
            {
                allowed=true;
            }
        }
        else
        {
            if(logic_board->checkIsAllowedMove(moveFrom, pos))
            {
                allowed=true;
            }
        }
        //moving pawn
        if(allowed)
        {
            movePieceOnBoard(moveFrom, pos);
            logic_board->movePiece(moveFrom, pos);
            moveFrom=-1;
            //checking if player formed a mill
            if(!logic_board->millFormed(pos))
            {
                //if not we change the turn, if yes, it's still the same player turn - he has to remove a piece
                logic_board->incrementTurn();
            }
            setMoveHoverStylesheet();

        }
    }
    else {
        // The button was clicked twice, so it will be reset
        moveFrom = -1;
        buttons[pos]->setObjectName("player"+QString::number(turn));
        // Set stylesheet
        setMoveHoverStylesheet();
    }
}

void GraphicBoard::movePieceOnBoard(int &moveFrom, const int &pos)
{
    int turn = logic_board->getPlayerTurn();
    // Update UI
    buttons[moveFrom]->setObjectName("empty");
    buttons[pos]->setObjectName("player"+QString::number(turn));
    buttons[moveFrom]->setStyle(qApp->style());
    buttons[pos]->setStyle(qApp->style());
    buttons[moveFrom]->repaint();
    buttons[pos]->repaint();

    // Update status message
    updateStatusLabel(tr("Player %1 has moved a piece.").arg(turn));

}

void GraphicBoard::pointSelected(int pos)
{
    PlayerType player_type = logic_board->getActualPlayerType();
    int game_ended = -1;
    int game_phase = logic_board->getGamePhase();
    int turn = logic_board->getPlayerTurn();
    bool mill_just_formed = logic_board->getMillJustFormed();
    if(player_type == Human)
    {

        if(mill_just_formed)
        {
            if(logic_board->checkPositionToRemove(pos))
            {
                logic_board->setMillJustFormed(false);
                removePiece(pos);
                logic_board->removePiece(pos);
                logic_board->incrementTurn();
            }
        }
        //mill not formed
        else {
            switch(game_phase){
            //not all pawn on the board
            case 0:
                addPiece(pos, game_phase);
                break;
                //all pawn on the board
            case 1:
                movePiece(moveFrom, pos, turn);
                game_ended = logic_board->gameHasEnded();
                if(game_ended == PLAYER1_ID)
                {
                    endGame(PLAYER1_ID);
                }
                else if(game_ended == PLAYER2_ID)
                {
                    endGame(PLAYER2_ID);
                }
                else if (game_ended == 0) {
                    endGame(0);
                }
                break;
            }
        }
        updateTurnLabel(logic_board->getTurnCounter());
    }
    //player is computer
    if(logic_board->getActualPlayerType() == Computer && game_ended == -1)
    {
        game_phase=logic_board->getGamePhase();
        switch(game_phase){
        //not all pawn on the board
        case 0:
        {
            int moveTo = logic_board->getBestAdd();
            updateStatusLabel(tr(" %1 has placed a piece on %2 position.")
                              .arg("Computer").arg(moveTo));

            addPieceOnBoard(moveTo);
            logic_board->addPiece(moveTo);

            //checking if player formed a mill
            if(logic_board->millFormed(moveTo))
            {
                int toRemove = *logic_board->getPlayers()->at(logic_board->getOpponentTurn()-1)->getPlayerPawns().begin();
                logic_board->setMillJustFormed(false);
                removePiece(toRemove);
                logic_board->removePiece(toRemove);

            }
            logic_board->incrementTurn();

            //  checking if first game phase has ended
            if(logic_board->firstPhaseHasEnded())
            {
                setMoveHoverStylesheet();
                logic_board->setGamePhase(game_phase+1);
            }
            break;
        }
            //all pawn on the board
        case 1:
        {
            std::pair<int, int> result = logic_board->getBestMove();
            int move_from = result.first;
            int move_to = result.second;

            movePieceOnBoard(move_from, move_to);
            logic_board->movePiece(move_from, move_to);

            //checking if player formed a mill
            if(logic_board->millFormed(move_to))
            {
                int toRemove = *logic_board->getPlayers()->at(logic_board->getOpponentTurn()-1)->getPlayerPawns().begin();
                logic_board->setMillJustFormed(false);
                removePiece(toRemove);
                logic_board->removePiece(toRemove);
            }
            logic_board->incrementTurn();
            setMoveHoverStylesheet();
            game_ended = logic_board->gameHasEnded();

            if(game_ended == PLAYER1_ID)
            {
                showErrorMessage(tr("1"));
                endGame(PLAYER1_ID);
            }
            else if(game_ended == PLAYER2_ID)
            {
                showErrorMessage(tr("2"));
                endGame(PLAYER2_ID);

            }
            else if(game_ended == 0)
            {
                showErrorMessage(tr("0"));
                endGame(0);

            }
            break;
        }
        }
        updateTurnLabel(logic_board->getTurnCounter());
        timeHandler();
        // next turn
        if(logic_board->getActualPlayerType() == Computer && game_ended == -1)
            pointSelected(0);
    }
}


void GraphicBoard::endGame(const int &losing_player_id)
{
    switch(losing_player_id)
    {
    case PLAYER1_ID:
        // player 1 has won
    {
        updateStatusLabel(tr("Player 2 have won the game! Congratulations!"));

    } break;
    case PLAYER2_ID:
        // player 2 has won
    {
        updateStatusLabel(tr("Player 1 has won the game! Congrats."));

    } break;
    case 0:
        updateStatusLabel(tr("It's a tie!"));
    }

    // Disable all buttons
    for(int i = 0; i < 24; i++)
    {
        buttons[i]->setEnabled(false);
    }
    firstPlayerSelect->setEnabled(true);
    secondPlayerSelect->setEnabled(true);
    algorithmSelect->setEnabled(true);
    algorithmSelect2->setEnabled(true);
    heuristicSelect->setEnabled(true);
    heuristicSelect2->setEnabled(true);
    startButton->setEnabled(true);
    logic_board->endGame();
}
void GraphicBoard::resetGame()
{
    // Enable and reset all buttons
    for(int i = 0; i < 24; i++)
    {
        buttons[i]->setObjectName("empty");
    }


    // Reset attributes
    //todo reset logic board
    //      logic_board->reset();

    // Reset players
    //      aiPlayer->reset();
    //      humanPlayer->reset();

    // Reset status list
    statusList->clear();
    statusList->addItem(tr("A new game has started."));

    // Reset labels
    updateTurnLabel(0);

    // Reset stylesheet
    setPlaceHoverStylesheet();
}


