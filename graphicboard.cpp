#include "graphicboard.h"

#include <QMessageBox>
#include <QSignalMapper>

GraphicBoard::GraphicBoard(QWidget *parent) : QWidget(parent)
{
//     *logic_board=board;
     turn = 1;
     gamePhase = 0;
     millDetected = 0;

     gamesWon = 0;
     gamesLost = 0;

     QSignalMapper *signalMapper = new QSignalMapper(this);

     connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(pointSelected(int)));

     for(int i = 0; i < 24; i++)
     {
       buttons[i] = new QPushButton("", this);

       connect(buttons[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
       signalMapper->setMapping(buttons[i], i);

       buttons[i]->setFixedHeight(50);
       buttons[i]->setFixedWidth(50);
       buttons[i]->setObjectName("empty");
     }


     // Set stylesheet
     setPlaceHoverStylesheet();

     boardLayout = new QGridLayout();
     statusLayout = new QGridLayout();

     // Set up the status layout
     // Displays: status message, current turn, current game phase rules, reset button

     statsLabel = new QLabel(tr("Stats:"));
     statsLabel->setObjectName("boldLabel");
     statusLayout->addWidget(statsLabel, 0, 0, 1, 2);


     turnTextLabel = new QLabel(tr("Current turn:"));
     turnTextLabel->setObjectName("boldLabel");
     statusLayout->addWidget(turnTextLabel, 1, 0);

     turnLabel = new QLabel(QString::number(turn));
     statusLayout->addWidget(turnLabel, 1, 1);


     gamesWonTextLabel = new QLabel(tr("Games won:"));
     gamesWonTextLabel->setObjectName("boldLabel");
     statusLayout->addWidget(gamesWonTextLabel, 2, 0);


     gamesWonLabel = new QLabel(QString::number(gamesWon));
     statusLayout->addWidget(gamesWonLabel, 2, 1);


     gamesLostTextLabel = new QLabel(tr("Games lost:"));
     gamesLostTextLabel->setObjectName("boldLabel");
     statusLayout->addWidget(gamesLostTextLabel, 3, 0);


     gamesLostLabel = new QLabel(QString::number(gamesLost));
     statusLayout->addWidget(gamesLostLabel, 3, 1);


     statusTextLabel = new QLabel(tr("Status messages:"));
     statusTextLabel->setObjectName("boldLabel");
     statusLayout->addWidget(statusTextLabel, 4, 0, 1, 2);

     statusList = new QListWidget;
     statusList->setWordWrap(true);
     statusList->addItem(tr("A new game has started."));
     statusLayout->addWidget(statusList, 5, 0, 1, 2);


     spacerItem1 = new QSpacerItem(400, 100);
     statusLayout->addItem(spacerItem1, 6, 0, 1, 2);


     gameRulesTextLabel = new QLabel(tr("How to play:"));
     gameRulesTextLabel->setObjectName("boldLabel");
     statusLayout->addWidget(gameRulesTextLabel, 7, 0, 1, 2);


     gameRulesLabel = new QLabel(tr(gameRules[0]));
     gameRulesLabel->setWordWrap(true);
     statusLayout->addWidget(gameRulesLabel, 8, 0, 1, 2);


     spacerItem2 = new QSpacerItem(400, 75);
     statusLayout->addItem(spacerItem2, 9, 0, 1, 2);


//     resetButton = new QPushButton(tr("Restart game"));
//     connect(resetButton, SIGNAL(clicked()), SLOT(resetGame()));
//     statusLayout->addWidget(resetButton, 10, 0, 1, 2);

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

void GraphicBoard::incTurn() {
  logic_board->incrementTurn();
}

void GraphicBoard::incGamePhase() {
  updateGameRulesLabel(tr(gameRules[gamePhase]));
  gamePhase++;
  updateStatusLabel(tr("Game phase %n begins.", 0, gamePhase));
}

void GraphicBoard::setLogicBoard(LogicBoard *&logic_b)
{
    logic_board=logic_b;
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
                      "QPushButton#selected { border: 0px; background-image: url(:/images/red_stone_selected.png); } "
                      "QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } "
                      "QPushButton#empty:hover { border: 0px; background-image: url(:/images/grey_stone_selected.png); } "
                      "QPushButton#selected:hover { border: 0px; background-image: url(:/images/blue_stone.png); }");
}

void GraphicBoard::setMoveHoverStylesheet()
{
  // The player is selecting one of his own pieces to move

  qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } QPushButton#player1:hover { border: 0px; background-image: url(:/images/blue_stone_selected.png); }");
}

void GraphicBoard::setRemoveHoverStylesheet()
{
  // The player is selecting one of the opposing player's pieces to remove

  qApp->setStyleSheet("QLabel#boldLabel { font-weight: bold; } QPushButton#empty { border: 0px; background-image: url(:/images/empty.png); } QPushButton#player1 { border: 0px; background-image: url(:/images/blue_stone.png); } QPushButton#player2 { border: 0px; background-image: url(:/images/red_stone.png); } QPushButton#player2:hover { border: 0px; background-image: url(:/images/red_stone_selected.png); }");
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

void GraphicBoard::updateGamesWonLabel(QString str)
{
  gamesWonLabel->setText(str);
}

void GraphicBoard::updateGamesLostLabel(QString str)
{
  gamesLostLabel->setText(str);
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

void GraphicBoard::addPiece(const int &pos)
{

    int player_turn = logic_board->getPlayerTurn();
    if(player_turn == PLAYER1_ID)
    {
        buttons[pos]->setObjectName("player1");
        updateStatusLabel(tr("Player %1 have placed a piece. You have %2 piece(s) left.")
                          .arg(player_turn).arg(logic_board->getPiecesLeft(player_turn)));
    }
    else if(player_turn == PLAYER2_ID)
    {
        buttons[pos]->setObjectName("player2");
        updateStatusLabel(tr("Player %1 have placed a piece. You have %2 piece(s) left.")
                          .arg(player_turn).arg(logic_board->getPiecesLeft(player_turn)));
    }

    buttons[pos]->setStyle(qApp->style());
    buttons[pos]->repaint();


}

void GraphicBoard::pointSelected(int pos)
{
    switch(gamePhase){
    //not all pawn on the board
    case 0:
        if(logic_board->checkPosition(pos))
        {
            addPiece(pos);
            logic_board->addPiece(pos);
        }

        //TODO check if there's a mill built by player after adding the pawn
        break;
    //all pawn on the board
    case 1:
        
        break;
    //one player has 3 pawns left
    case 2:
        
        break;
    }
    if(logic_board->getPiecesLeft(PLAYER2_ID) == 0 && logic_board->getPiecesLeft(PLAYER1_ID) == 0)
    {
        gamePhase++;
    }


}



void GraphicBoard::resetGame()
{

}


