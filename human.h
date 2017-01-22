#ifndef HUMAN_H
#define HUMAN_H
#include "player.h"
#include "vars.h"
#include "gameships.h"
#include "displayships.h"
#include "selectship.h"



class Human : public Player
{
public:
    Human();
    virtual ~Human();
    TARGET_POINT Turn(int n, int m, MYPOINT* OPGrid[WID][LEN]);
    TARGET_POINT Human_Turn(int n, int m, MYPOINT* OPGrid[WID][LEN]);
    bool allShipsArePlaced();

    displayships *ship[4];
    selectship *selship[4];
};

#endif // HUMAN_H
