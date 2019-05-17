#pragma once

#ifndef GRAPHICBOARD_H
#define GRAPHICBOARD_H

#include "logicboard.h"

#include <QApplication>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QObject>
#include <QPushButton>
#include <QWidget>

#define PLAYER1_ID 1
#define PLAYER2_ID 2

class LogicBoard;

class GraphicBoard : public QWidget
{
    Q_OBJECT

private:
    QHBoxLayout *mainLayout;
    QGridLayout *boardLayout;
    QGridLayout *statusLayout;

    QFrame *frame;
    QPushButton *buttons[24];
    QLabel * rowLabels[7];
    QLabel * colLabels[7];
    LogicBoard *logic_board;

    QPushButton * resetButton;

    QLabel * statsLabel;
    QLabel * turnLabel;
    QLabel * gameRulesLabel;
    QLabel * gamesWonLabel;
    QLabel * gamesLostLabel;
    QListWidget * statusList;

    QLabel * turnTextLabel;
    QLabel * gamesWonTextLabel;
    QLabel * gamesLostTextLabel;
    QLabel * statusTextLabel;
    QLabel * gameRulesTextLabel;

    QSpacerItem * spacerItem1;
    QSpacerItem * spacerItem2;
    QSpacerItem *spacer2;

    int turn;
    int gamePhase;
    int millDetected;
    int gamesWon, gamesLost;

    const char * gameRules [3] = {
        "The game has begun!\nYou have blue pieces, the computer has red pieces. You take turns placing pieces on the intersections of the lines on the board.\n\nYour goal is to form a mill (= three pieces in a horizontal or vertical line). If you have formed a mill, you can remove one of the computer's pieces off the board.\n\nThis phase ends when you have placed all of your nine pieces.",
        "Phase 2 has begun!\nYou can now move your pieces to connected free points on the board.\n\nYour goal is to form a mill (= three pieces in a horizontal or vertical line). If you have formed a mill, you can remove one of the computer's pieces off the board.\n\nThis phase ends when you have three pieces left on the board.",
        "Phase 3 has begun!\nYou can now move your pieces freely to any free points on the board.\n\nYour goal is to form a mill (= three pieces in a horizontal or vertical line). If you have formed a mill, you can remove one of the computer's pieces off the board.\n\nThe game ends when one of the players has no more legal moves or less than three pieces left on the board."
      };

public slots:
    void incTurn();
    /*!
      *  This method is connected to the intersecting points on the game board (the points where game pieces can be placed)
      *  It contains the logic for what happens when the user clicks on one of the points
      *
      *  If a mill is detected, the human player will be allowed to remove one of the AI player's pieces
      *  If no mill is detected and the game phase is 1, the player will be allowed to place a piece
      *                                               2, the player will be allowed to move a piece
      *                                               3, the player will be allowed to move a piece freely
      */

     void pointSelected(int pos);

     /*!
      *  Resets the game board
      *  This method is connected to the "Restart game" button
      */

     void resetGame();

     /*!
      *  Changes the application language
      *  This method is connected to the language menu items
      */


public:
     // Constructor
     GraphicBoard(QWidget *parent = nullptr);

     // Setters
     void incGamePhase();
//     void incGamePhase(Player * player);
     void setLogicBoard(LogicBoard *&logic_b);

     /*!
      *  Set stylesheet methods
      *  The stylesheets create the hover effects and make the UI feel more responsive
      */

     void setPlaceHoverStylesheet();
     void setMoveHoverStylesheet();
     void setRemoveHoverStylesheet();

     /*!
      *  UI update methods
      *  These methods are called when any of the game state variables change (for example the game phase)
      *  Or an error message needs to be displayed
      *  Or the UI needs to be retranslated
      */

     void updateTurnLabel(int turn);
     void updateGameRulesLabel(QString str);
     void updateStatusLabel(QString str);
     void updateGamePhaseLabel(QString str);
     void updateGamesWonLabel(QString str);
     void updateGamesLostLabel(QString str);
     void showErrorMessage(QString str);
     void retranslateUi();

     void addPiece(const int &pos);


signals:


};

#endif // GRAPHICBOARD_H
