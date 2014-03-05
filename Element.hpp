#ifndef ELEMENT_HPP
#define ELEMENT_HPP
#include <ostream>

class QGraphicsLineItem;
class QGraphicsItem;
class QLineF;
class Node;

class Element {
public:
    Element(const Node& node_1, const Node& node_2, const QLineF& line);

    unsigned int type;
    double youngsModulus, area, area_2;

    unsigned int nodeIndex_1() const;
    unsigned int nodeIndex_2() const;

    void setNode_1(const Node &node);
    void setNode_2(const Node &node);
    void adjust();

    QGraphicsItem* getItem();

private:
    QGraphicsLineItem* m_sprite;
    const Node* node_1;
    const Node* node_2;

    friend std::ostream& operator<<(std::ostream& stream, const Element& element);
};

std::ostream& operator<<(std::ostream& stream, const Element& element);

#endif // ELEMENT_HPP
