#include "gameships.h"
#include "vars.h"
#include <vector>

gameships::gameships(short int sn)
{
    placed = false;
    direction = Right;
    shipnum = sn;
    switch(shipnum)
    {
    case 1:
        size = 2;
        break;
    case 2:
        size = 3;
        break;
    case 3:
        size = 4;
        break;
    case 4:
        size = 5;
        break;
    }
    for(int q=0; q<size; ++q)
    {
        points.push_back(QPoint(0,q));
        hitpoints.push_back(false);
    }
}

gameships::~gameships()
{

}


void gameships::setDirection(short int direct)
{
    switch(direct)
    {
    case Up:
        for(int z=1; z<size; ++z)
        {
            points[z].setX(0);
            points[z].setY(z);
        }
        break;
    case Right:
        for(int z=1; z<size; ++z)
        {
            points[z].setX(z);
            points[z].setY(0);
        }
        break;
    case Down:
        for(int z=1; z<size; ++z)
        {
            points[z].setX(0);
            points[z].setY(z);
        }
        break;
    case Left:
        for(int z=1; z<size; ++z)
        {
            points[z].setX(z);
            points[z].setY(0);
        }
        break;
    default:
        return;
        break;
    }
    direction = direct;
}

bool gameships::placeThisShip(MYPOINT* theGrid[WID][LEN], QPoint noseSquare)
{
    bool canPlace = false;
    if(direction == Left || direction == Right)
    {
        if((size - 1 + noseSquare.x()) < WID)
        {
            if(shipsAreInLine(theGrid,noseSquare,size,true) == false)
            {
                canPlace = true;
            }
        }
    }
    else if(direction == Up || direction == Down)
    {
        if((size - 1 + noseSquare.y()) < LEN)
        {
            if(shipsAreInLine(theGrid,noseSquare,size,false) == false)
            {
                canPlace = true;
            }
        }
    }

    if(canPlace)
    {
        if(direction == Left || direction == Right)
        {
            for(int x = 0; x < size; ++x)
            {
                theGrid[noseSquare.x() + x][noseSquare.y()]->shipis = shipnum;
                points[x].setX(noseSquare.x() + x);
                points[x].setY(noseSquare.y());
            }
        }
        else if(direction == Up || direction == Down)
        {
            for(int y = 0; y < size; ++y)
            {
                theGrid[noseSquare.x()][noseSquare.y() + y]->shipis = shipnum;
                points[y].setX(noseSquare.x());
                points[y].setY(noseSquare.y() + y);
            }
        }
        placed = true;

        return true;
    }

    return false;
}

void gameships::removeThisShip(MYPOINT* theGrid[WID][LEN])
{
    if(direction == Left || direction == Right)
    {
        for(int x = 0; x < size; ++x)
        {
            theGrid[points[x].x()][points[x].y()]->shipis = 0;
        }
    }
    else if(direction == Up || direction == Down)
    {
        for(int y = 0; y < size; ++y)
        {
            theGrid[points[y].x()][points[y].y()]->shipis = 0;
        }
    }

    setDirection(direction);
    placed = false;
}

bool gameships::getHitAt(short int x,short int y)
{
    short int thepointtouched = pointTouched(x,y);
    if(thepointtouched != -1)
    {
        if(hitpoints[thepointtouched] == true)
        {
            return false;
        }
        hitpoints[thepointtouched] = true;
        return true;
    }
    return false;
}

short int gameships::pointTouched(short int x, short int y)
{
    for(int q=0;q<size;++q)
    {
        if(points[q].x() == x && points[q].y() == y)
        {
            return q;
        }
    }
    return -1;
}


bool shipsAreInLine(MYPOINT* theGrid[WID][LEN], QPoint beginSquare, short int size, bool toRight)
{
    if(toRight)
    {
        for(int x = 0; x < size; ++x)
        {
            if(theGrid[beginSquare.x() + x][beginSquare.y()]->shipis > 0)
                return true;
        }
    }
    else
    {
        for(int y = 0; y < size; ++y)
        {
            if(theGrid[beginSquare.x()][beginSquare.y() + y]->shipis > 0)
                return true;
        }
    }
    return false;
}
