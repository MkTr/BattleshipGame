#include "human.h"
#include "gameships.h"
#include <QDebug>


Human::Human()
{
    isCPU = false;
    //displayships
    //selectships
}

Human::~Human()
{

}


TARGET_POINT Human::Turn(int n, int m, MYPOINT* OPGrid[WID][LEN])
{
    return Human_Turn(n,m,OPGrid);
}

TARGET_POINT Human::Human_Turn(int n, int m, MYPOINT*OPGrid[WID][LEN])//Note: Unlike the SFML version's PLAYER_Fire, this function doesn't check if the square is already hit (as that's already covered with the boardsquare's ability to disable itself after being clicked).
{
     TARGET_POINT ret;

     ret.x = n;
     ret.y = m;

     OPGrid[n][m]->hit = true;
     if (OPGrid[n][m]->shipis > 0)
     {
         ret.cpupnt.shipis = OPGrid[n][m]->shipis;
         ret.cpupnt.pointis = hit;
     }
     else
     {
         ret.cpupnt.shipis = 0;
         ret.cpupnt.pointis = miss;
     }

     return ret;
}

bool Human::allShipsArePlaced()
{
    if(displayships::allocated[0]==true)
    {
        if(displayships::allocated[1]==true)
        {
            if(displayships::allocated[2]==true)
            {
                if(displayships::allocated[3]==true)
                {
                    QRectF* d_ship[4];
                    if(displayships::orientations[0]==0)    {d_ship[0] = new QRectF(displayships::x_pos[0],displayships::y_pos[0],SQUARE,SQUARE*2);}
                                                    else    {d_ship[0] = new QRectF(displayships::x_pos[0]-SQUARE*2,displayships::y_pos[0],SQUARE*2,SQUARE);}
                    if(displayships::orientations[1]==0)    {d_ship[1] = new QRectF(displayships::x_pos[1],displayships::y_pos[1],SQUARE,SQUARE*3);}
                                                    else    {d_ship[1] = new QRectF(displayships::x_pos[1]-SQUARE*3,displayships::y_pos[1],SQUARE*3,SQUARE);}
                    if(displayships::orientations[2]==0)    {d_ship[2] = new QRectF(displayships::x_pos[2],displayships::y_pos[2],SQUARE,SQUARE*4);}
                                                    else    {d_ship[2] = new QRectF(displayships::x_pos[2]-SQUARE*4,displayships::y_pos[2],SQUARE*4,SQUARE);}
                    if(displayships::orientations[3]==0)    {d_ship[3] = new QRectF(displayships::x_pos[3],displayships::y_pos[3],SQUARE,SQUARE*5);}
                                                    else    {d_ship[3] = new QRectF(displayships::x_pos[3]-SQUARE*5,displayships::y_pos[3],SQUARE*5,SQUARE);}
                    for(int first=1;first<4;++first)
                    {
                        for(int second=0;second<first;++second)
                        {
                            if(d_ship[first]->intersects(*d_ship[second]) || d_ship[first]->contains(*d_ship[second]))
                            {   /*
                                qDebug()<<"Ships are placed incorrectly.";
                                qDebug()<<"Ship 0: x" << d_ship[0]->x() << " y" << d_ship[0]->y();
                                qDebug()<<"Ship 1: x" << d_ship[1]->x() << " y" << d_ship[1]->y();
                                qDebug()<<"Ship 2: x" << d_ship[2]->x() << " y" << d_ship[2]->y();
                                qDebug()<<"Ship 3: x" << d_ship[3]->x() << " y" << d_ship[3]->y();  */
                                delete d_ship[0];
                                delete d_ship[1];
                                delete d_ship[2];
                                delete d_ship[3];
                                return false;
                            }
                        }
                    }   /*
                    qDebug()<<"Ships are placed correctly.";
                    qDebug()<<"Ship 0: x" << d_ship[0]->x() << " y" << d_ship[0]->y();
                    qDebug()<<"Ship 1: x" << d_ship[1]->x() << " y" << d_ship[1]->y();
                    qDebug()<<"Ship 2: x" << d_ship[2]->x() << " y" << d_ship[2]->y();
                    qDebug()<<"Ship 3: x" << d_ship[3]->x() << " y" << d_ship[3]->y();  */
                    delete d_ship[0];
                    delete d_ship[1];
                    delete d_ship[2];
                    delete d_ship[3];
                    return true;
                }
            }
        }
    }
    return false;
}
