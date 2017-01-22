#include "player.h"
#include "gameships.h"


Player::Player()
{
    isCPU = unknown;
    for(int x=0;x<WID;++x)
    {
        for(int y=0;y<WID;++y)
        {
            PlayerGrid[x][y] = new MYPOINT;
            PlayerGrid[x][y]->hit = false;
            PlayerGrid[x][y]->shipis = 0;
        }
    }
    for(int bazinga=0;bazinga<4;++bazinga)
    {
        PlayerShips[bazinga] = new gameships(bazinga+1);
    }
    targetedPoint.cpupnt.pointis = -1;
    targetedPoint.cpupnt.shipis = -1;
    targetedPoint.x = -1;
    targetedPoint.y = -1;
}

Player::~Player()
{
    for(int x=0;x<WID;++x)
    {
        for(int y=0;y<WID;++y)
        {
            delete PlayerGrid[x][y];
        }
    }
    for(int bazinga=0;bazinga<4;++bazinga)
    {
        delete PlayerShips[bazinga];
    }
}

bool Player::placeShip(short int thisship, int x, int y)//x and y are grid coords
{
    QPoint theSquare;
    theSquare.setX(x);
    theSquare.setY(y);

    if(PlayerShips[thisship-1]->placeThisShip(PlayerGrid,theSquare))
    {
        return true;
    }
return false;
}
void Player::removeShip(short int thisship)
{
    if(thisship > 0 && PlayerShips[thisship-1]->placed == true)
    {
        PlayerShips[thisship-1]->removeThisShip(PlayerGrid);
    }
}

TARGET_POINT Player::fireAtPoint(MYPOINT* OPGrid[WID][LEN], gameships* OPShips[4])
{
    TARGET_POINT doubtingPoint;
    if(isCPU)
    {
        targetedPoint = Turn(0,0,OPGrid);
        targetedPoint.cpupnt.shipis = OPGrid[targetedPoint.x][targetedPoint.y]->shipis;

        if(targetedPoint.cpupnt.shipis > 0)
        {
            OPShips[targetedPoint.cpupnt.shipis-1]->getHitAt(targetedPoint.x,targetedPoint.y);
        }
        return targetedPoint;
    }
    else
    {
        doubtingPoint = Turn(targetedPoint.x,targetedPoint.y,OPGrid);
        if(doubtingPoint.cpupnt.pointis == -1)
        {
            return doubtingPoint;
        }
        targetedPoint = doubtingPoint;
        if(targetedPoint.cpupnt.shipis > 0)
        {
            OPShips[targetedPoint.cpupnt.shipis-1]->getHitAt(targetedPoint.x,targetedPoint.y);
        }
        return targetedPoint;
    }
}
