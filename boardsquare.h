#ifndef boardsquare_H
#define boardsquare_H

#include <QObject>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsItem>


class boardsquare : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    boardsquare(QGraphicsScene *Mscene, int size, int x, int y, int n, int m);
    // QGraphicsItem square;
    int x_pos, y_pos;
    int squaresize;

    int n_array, m_array;

    int doIt;

    int GetSize(void);

    void DrawSquare();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    QGraphicsScene *scene;
    QGraphicsRectItem *square;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

signals:
    void Clicked(int n, int m);

private slots:
    void OnReceiveOrders(int n, int m, int doThis);
};

#endif // boardsquare_H
