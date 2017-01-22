#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "vars.h"
#include "displayships.h"
#include "selectship.h"
#include "battleshipgame.h"
#include "boardsquare.h"
#include <QString>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
//    QGraphicsRectItem *rectangle[WID*10][LEN*10];

    BattleshipGame* bsg;

    QTimer *timesup;
    QBrush *redBrush;
    QBrush *blueBrush;
    QBrush *grayBrush;
    QBrush *definedBrush;
    QPen *blackpen;

    QColor *thiscolor;

    void DrawLayout();
    void NewGame();

protected:

private slots:
    void OnClick(int shipnum);
    void OnDisplayshipClicked(int);
    void OnReceiveNM(int n, int m);
    void whenHumanAnimationDone(int n, int m, int doThis);
    void onPhaseTextSet(QString newtext);

    void on_pushButton_2_clicked();

signals:
    void SendDothis(int n, int m, int doThis);
    void Sunk(int n);
};

#endif // MAINWINDOW_H
