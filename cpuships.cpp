#include "cpuships.h"
#include "vars.h"

cpuships::cpuships(QGraphicsScene *Mscene, int num)
{
    scene = Mscene;

    shipnum = num;

    shipsize = shipnum + 2;

    Show = false;
    /*
    redBrush = new QBrush(Qt::red);
    blueBrush = new QBrush(Qt::blue);
    grayBrush = new QBrush(Qt::lightGray);

    definedBrush = new QBrush(Qt::lightGray);

    blackpen = new QPen(Qt::black);
    blackpen->setWidth(2);
    */

    orient = 0;
    //setFlag(ItemIsMovable);
    // DrawShip();
}

QRectF cpuships::boundingRect() const
{
    switch(shipsize)
    {
    case 2:
        return QRectF(130,90,30,20);
        break;
    case 3:
        return QRectF(110,60,50,20);
        break;
    case 4:
        return QRectF(90,30,70,20);
        break;
    case 5:
        return QRectF(70,0,90,20);
        break;
    }
    return QRectF(0,0,0,0);
}


void cpuships::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::blue);

    if(Show)
    {
        brush.setColor(Qt::red);
        painter->fillRect(rec,brush);
        painter->drawRect(rec);
    }
    else
    {
        brush.setColor(Qt::white);
        painter->fillRect(rec,brush);
    }

    /*
    if(Change)
    {
        brush.setColor(Qt::blue);
    }
    else
    {
        brush.setColor(Qt::red);
    }
    */

    // painter->drawEllipse(rec);
}


void cpuships::OnSunk(int shipnumber)
{
    if (shipnumber == this->shipnum)
    {
        Show = true;
        update();
    }
}

