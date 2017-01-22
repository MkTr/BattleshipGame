#ifndef SELECTSHIP_H
#define SELECTSHIP_H

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QObject>

class selectship  :  public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    selectship(QGraphicsScene *Mscene, int num);

    int shipsize;
    int shipnum;

    void DrawShip();
    void ReOrientShip();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    int orient;
    bool Show;
    bool Sunk;

    QGraphicsScene *scene;
    QGraphicsRectItem *ship;

    QBrush *redBrush;
    QBrush *blueBrush;
    QBrush *grayBrush;
    QBrush *definedBrush;
    QPen *blackpen;

signals:
    void Clicked(int shipsize);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*);

private slots:
    void OnSunk(int shipsize);
};


#endif // SELECTSHIP_H
