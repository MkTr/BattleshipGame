#ifndef displayships_H
#define displayships_H

#include <QPainter>
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>

class displayships  : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    displayships(QGraphicsScene *Mscene, int num);

    static bool phaseIsSetupShips;
    static int x_pos[4];
    static int y_pos[4];
    static bool orientations[4];
    static bool allocated[4];

    void DrawShip();
    void ReOrientShip();

    int shipsize;
    int shipnum;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    bool orient;
    bool Pressed;

    QGraphicsScene *scene;
    QGraphicsRectItem *ship;

    QBrush *redBrush;
    QBrush *blueBrush;
    QBrush *grayBrush;
    QBrush *definedBrush;
    QPen *blackpen;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    void displayship_clicked(int s_num);
};


#endif // displayships_H
