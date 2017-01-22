#ifndef CPUSHIPS_H
#define CPUSHIPS_H

#include <QObject>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "vars.h"

class cpuships : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit cpuships(QGraphicsScene *Mscene, int num);

    int shipsize;
    int shipnum;

    void DrawShip();
    void ReOrientShip();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    int orient;
    bool Show;

    QGraphicsScene *scene;
    QGraphicsRectItem *ship;

    QBrush *redBrush;
    QBrush *blueBrush;
    QBrush *grayBrush;
    QBrush *definedBrush;
    QPen *blackpen;

private slots:
    void OnSunk(int shipnumber);

signals:
    void SunkTest(int n);

};

#endif // CPUSHIPS_H
