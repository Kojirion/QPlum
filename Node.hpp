#ifndef NODE_HPP
#define NODE_HPP
#include <ostream>
#include <boost/optional.hpp>
#include <QPointF>
#include <QList>
#include <QGraphicsEllipseItem>

//class QGraphicsItem;
//class QGraphicsEllipseItem;
class Element;
class QDataStream;

typedef boost::optional<double> OptionalDouble;

QDataStream& operator<<(QDataStream& stream, const OptionalDouble& number);
QDataStream& operator>>(QDataStream& stream, OptionalDouble& number);

struct OptionalVector {
    OptionalDouble x, y, z;
};

//QDataStream& operator<<(QDataStream& stream, const OptionalVector& vector);
//QDataStream& operator>>(QDataStream& stream, OptionalVector& vector);

struct Vector {
    double x,y,z;
};

class Node : public QGraphicsEllipseItem {
public:
    Node(unsigned int index);

    OptionalVector fixity;
    Vector load;

    void addEdge(Element &element);
    unsigned int getIndex() const;

    QPointF position() const;
    void setX(qreal x);
    void setY(qreal y);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QList<Element*> m_edges;

    friend QDataStream& operator<<(QDataStream& stream, const Node& node);
};

std::ostream& operator<<(std::ostream& stream, const Node& node);
QDataStream& operator<<(QDataStream& stream, const Node& node);

#endif // NODE_HPP
