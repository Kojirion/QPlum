#ifndef SCENE_HPP
#define SCENE_HPP

#include <QGraphicsScene>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);

signals:
    void clicked(const QPointF& point);
    void mouseMoved(const QPointF& point);

public slots:

private:
    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

};

#endif // SCENE_HPP
