#include "Element.hpp"
#include <QGraphicsLineItem>
#include "Node.hpp"
#include <QDebug>


Element::Element(const Node& node_1, const Node& node_2, const QLineF &line):
    type(1),
    node_1(&node_1), node_2(&node_2),
    youngsModulus(2E-8),
    area(0.01),
    area_2(3E-4),
    m_sprite(new QGraphicsLineItem(line))
{
    //m_sprite->grabMouse();
    //m_sprite->setLine(point.x(), point.y());
}

unsigned int Element::nodeIndex_1() const
{
    return node_1->getIndex();
}

unsigned int Element::nodeIndex_2() const
{
    return node_2->getIndex();
}

void Element::setNode_1(const Node &node)
{
    node_1 = &node;
}

void Element::setNode_2(const Node &node)
{
    node_2 = &node;
}

void Element::adjust()
{
    QLineF line(node_1->position(), node_2->position());
    qDebug() << line;
    m_sprite->setLine(line);
}

QGraphicsItem *Element::getItem()
{
    return m_sprite;
}

std::ostream &operator <<(std::ostream &stream, const Element &element)
{
    stream << element.type << " "
           << element.node_1->getIndex() << " "
           << element.node_2->getIndex() << " "
           << element.youngsModulus << " "
           << element.area << " "
           << element.area_2;
    return stream;
}
