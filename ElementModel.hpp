#ifndef ELEMENTMODEL_HPP
#define ELEMENTMODEL_HPP

#include <QAbstractTableModel>
#include <fstream>
#include "Element.hpp"

class QGraphicsScene;
class QLineF;
class NodeModel;

class ElementModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ElementModel(const NodeModel& nodeModel, QGraphicsScene& scene, QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void saveTo(std::ofstream& stream) const;
    
signals:
    
public slots:
    Element& appendRow(unsigned int node_1, unsigned int node_2, const QLineF& line);

private:
    const NodeModel& m_nodeModel;
    QGraphicsScene& m_scene;
    QList<Element> elements;

    friend QDataStream& operator<<(QDataStream& stream, const ElementModel& elementModel);
    friend QDataStream& operator>>(QDataStream& stream, ElementModel& elementModel);
    
};

QDataStream& operator<<(QDataStream& stream, const ElementModel& elementModel);
QDataStream& operator>>(QDataStream& stream, ElementModel& elementModel);

#endif // ELEMENTMODEL_HPP
