#include "Scene.hpp"
#include <QGraphicsSceneMouseEvent>

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
    emit clicked(mouseEvent->scenePos());
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    emit mouseMoved(event->scenePos());
}
