
#include "selectship.h"
#include "displayships.h"
#include "vars.h"

selectship::selectship(QGraphicsScene *Mscene, int num)
{
    scene = Mscene;

    shipnum = num;

    shipsize = shipnum + 2;

    Show = true;
    Sunk = false;
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

QRectF selectship::boundingRect() const
{
    switch(shipsize)
    {
    case 2:
        return QRectF(0,0,30,20);
        break;
    case 3:
        return QRectF(0,30,50,20);
        break;
    case 4:
        return QRectF(0,60,70,20);
        break;
    case 5:
        return QRectF(0,90,90,20);
        break;
    }
    return QRectF(0,0,0,0);
}


void selectship::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::blue);

    if(Show)
    {
        brush.setColor(Qt::green);
    }
    else
    {
        brush.setColor(Qt::white);
    }

    if (Sunk ==true)
    {
        brush.setColor(Qt::red);
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

    painter->fillRect(rec,brush);
    // painter->drawEllipse(rec);
    painter->drawRect(rec);
}


void selectship::DrawShip()
{

    // rectangle[n][m] = scene->addRect(x, y, offset, offset, *blackpen, *definedBrush);
    // scene->addRect(SQUARE, SQUARE, SQUARE, 120, *blackpen, *redBrush);
    //ship->boundingRect(SQUARE, 120);
    // setFlag(QGraphicsItem::ItemIsMovable);
}


void selectship::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{
    emit Clicked(shipnum);

    if(displayships::phaseIsSetupShips)
        delete this;
    // update();
    // QGraphicsItem::mouseDoubleClickEvent(event);
}


void selectship::OnSunk(int shipvalue)
{
    if(shipvalue == shipnum)
    {
        Sunk = true;
    }
    update();
}
