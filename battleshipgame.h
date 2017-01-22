#ifndef BATTLESHIPGAME_H
#define BATTLESHIPGAME_H

#include "vars.h"
#include "cpu.h"
#include "human.h"
#include "boardsquare.h"
#include "cpuships.h"
#include "blast.h"
#include <QObject>
#include <QString>
#include <QGraphicsRectItem>



class BattleshipGame : public QObject
{
Q_OBJECT

public:
    BattleshipGame();
    ~BattleshipGame();
    bool hasLost(MYPOINT* Grid[WID][LEN]);
    bool allHumanShipsArePlaced();
    void changetoPlayerTurn();
    void changetoCPUTurn();
    void changefromPlayerTurn();
    void changefromCPUTurn();
    void changetoGameOver(bool humanWon);

    QGraphicsScene *scene;
    QGraphicsScene *scene2;
    QGraphicsScene *scene3;
    Blast *blastanimation;
    Human* humanplayer;
    CPU* cpuplayer;
    short int gamephase;
    QGraphicsRectItem *HumanSquares[WID][LEN];
    boardsquare *CPUsquares[WID][LEN];
    QGraphicsRectItem *HumanGridPointsShot[WID*LEN];
    short int NumberOfHumanGridPointsShot;
    short int animeframes;

    cpuships *cpuship[4];

signals:
    void setPhaseText(QString newtext);
    void sinkPlayerShip(int shipvalue);
    void sinkCPUShip(int shipvalue);
    void humanAnimationDone(int n, int m, int doThis);

public slots:
    void advance();
};

#endif
