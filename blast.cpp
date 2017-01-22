#include "blast.h"
#include "vars.h"

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

Blast::Blast(qreal x, qreal y)
{
    blastframe = 0;

    int StartX = 0;
    int StartY = 0;
    /*
    if((qrand() % 1))
    {
        StartX = (qrand() % 150);
        StartY = (qrand() % 100);
    }
    else
    {
        StartX = (qrand() % -150);
        StartY = (qrand() % -100);
    }
    */
    StartX = x;
    StartY = y;
    setPos(mapToParent(StartX,StartY));
}

QRectF Blast::boundingRect() const
{
    return QRect((-blastframe),(-blastframe),blastframe*2,blastframe*2);
}

void Blast::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rec = boundingRect();
    QBrush Brush(Qt::red);

    painter->drawEllipse(rec);
}

void Blast::advance()
{
    advance(1);
}

void Blast::advance(int phase)
{
    if(!phase) return;

    if(blastframe < ANIMEFRAMES)
        ++blastframe;
    update();
}
