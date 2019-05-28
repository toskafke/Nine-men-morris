#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    logic_board = new LogicBoard();
    board = new GraphicBoard();


    board->setLogicBoard(logic_board);
    setWindowTitle(tr("Nine Men's Morris"));
    this->setCentralWidget(board);
    
}

MainWindow::~MainWindow()
{
    delete ui;
}
