#ifndef NODEMODEL_HPP
#define NODEMODEL_HPP
#include <QAbstractTableModel>
#include <QList>
#include <fstream>
#include "Node.hpp"

class QGraphicsScene;
class QPointF;
class QDataStream;

class NodeModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit NodeModel(QGraphicsScene& scene, QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void saveTo(std::ofstream& stream) const;
    void saveFixesLoadsTo(std::ofstream& stream) const;
    void makeMovable();
    void makeImmovable();
    const Node& itemAt(unsigned int index) const;
    Node& itemAt(unsigned int index);
    void attachEdge(unsigned int node_1, unsigned int node_2, Element& element);


signals:
    
public slots:
    void appendRow(const QPointF& point = {0,0});

private:
    QGraphicsScene& m_scene;
    QList<Node*> nodes;

    friend QDataStream& operator<<(QDataStream& stream, const NodeModel& nodeModel);
    friend QDataStream& operator>>(QDataStream& stream, NodeModel& nodeModel);
};

QDataStream& operator<<(QDataStream& stream, const NodeModel& nodeModel);
QDataStream& operator>>(QDataStream& stream, NodeModel& nodeModel);

#endif // NODEMODEL_HPP
