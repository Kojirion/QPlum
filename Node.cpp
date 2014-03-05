#include "Node.hpp"
#include <QGraphicsEllipseItem>
#include <QDebug>
#include "Element.hpp"

static const qreal radius = 5;

Node::Node(unsigned int index):
    QGraphicsEllipseItem(0., 0, 2*radius, 2*radius),
    load{0., 0., 0.}
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setData(0, index);
}

void Node::addEdge(Element &element)
{
    m_edges.append(&element);
}

unsigned int Node::getIndex() const
{
    return data(0).toUInt();
}

QPointF Node::position() const
{
    return rect().center() + pos();
}

void Node::setX(qreal x)
{
    auto rectangle = rect();
    rectangle.setLeft(x - radius);
    rectangle.setRight(x+radius);
    setRect(rectangle);
}

void Node::setY(qreal y)
{
    auto rectangle = rect();
    rectangle.setTop(y - radius);
    rectangle.setBottom(y+radius);
    setRect(rectangle);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        for (auto& edge : m_edges)
            edge->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

std::ostream &operator <<(std::ostream &stream, const Node &node)
{
    stream << node.position().x() << " " << node.position().y();
    return stream;
}
