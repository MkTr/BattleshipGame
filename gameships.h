#ifndef GAMESHIPS_H
#define GAMESHIPS_H

#include <QMainWindow>
#include <vector>
#include "vars.h"


bool shipsAreInLine(MYPOINT* theGrid[WID][LEN], QPoint beginSquare, short int size, bool toRight);



class gameships
{
public:
    gameships(short int sn);
    ~gameships();
    void setDirection(short int direct);
    bool placeThisShip(MYPOINT* theGrid[WID][LEN], QPoint noseSquare);
    void removeThisShip(MYPOINT* theGrid[WID][LEN]);
    bool getHitAt(short int x,short int y);
    short int pointTouched(short int x, short int y);

    bool placed;
    short int direction;
    short int shipnum;
    short int size;
    QVector<QPoint> points;
    QVector<bool> hitpoints;
};

#endif // GAMESHIPS_H
