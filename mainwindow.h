#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <graphicboard.h>
#include <logicboard.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LogicBoard *logic_board;
    GraphicBoard *board;
    QHBoxLayout *horizontalLayout;
};

#endif // MAINWINDOW_H
