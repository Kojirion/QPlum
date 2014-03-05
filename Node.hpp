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

struct OptionalVector {
    boost::optional<double> x, y, z;
};

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
};

std::ostream& operator<<(std::ostream& stream, const Node& node);

#endif // NODE_HPP
