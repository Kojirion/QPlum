#include "ElementModel.hpp"
#include <QGraphicsScene>
#include <QLineF>
#include "NodeModel.hpp"
#include <QDebug>

ElementModel::ElementModel(NodeModel &nodeModel, QGraphicsScene &scene, QObject *parent) :
    QAbstractTableModel(parent),
    m_nodeModel(nodeModel),
    m_scene(scene)
{
}

int ElementModel::rowCount(const QModelIndex &parent) const
{
    return elements.size();
}

int ElementModel::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant ElementModel::data(const QModelIndex &index, int role) const
{
    if ((!index.isValid()) ||(index.row() >= elements.size()))
        return QVariant();

    if (role == Qt::DisplayRole){
        auto& toReturn = elements[index.row()];

        switch (index.column()) {
        case 0:
            return toReturn->type;
        case 1:
            return toReturn->nodeIndex_1() + 1;
        case 2:
            return toReturn->nodeIndex_2() + 1;
        case 3:
            return QVariant(toReturn->youngsModulus);
        case 4:
            return QVariant(toReturn->area);
        case 5:
            return QVariant(toReturn->area_2);
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QVariant ElementModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole){
        if (orientation == Qt::Horizontal){
            switch (section) {
            case 0:
                return "Type";
            case 1:
                return "Node 1";
            case 2:
                return "Node 2";
            case 3:
                return "Young's Modulus";
            case 4:
                return "Area";
            case 5:
                return "Area 2nd moment";
            default:
                return QVariant();
            }
        }else if (orientation == Qt::Vertical){
            return section+1;
        }
    }

    return QVariant();
}

bool ElementModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole){
        if ((!index.isValid()) ||(index.row() >= elements.size()))
            return false;

        auto& toSet = elements[index.row()];

        switch (index.column()) {
        case 0:
            toSet->type = value.toInt();
            break;
        case 1:{
            auto index = value.toInt()-1;
            if (index >= m_nodeModel.rowCount())
                break;
            auto& oldNode = m_nodeModel.itemAt(toSet->nodeIndex_1());
            oldNode.removeEdge(*toSet);
            auto& newNode = m_nodeModel.itemAt(index);
            toSet->setNode_1(newNode);
            newNode.addEdge(*toSet);
            toSet->adjust();
            break;
        }
        case 2:{
            auto index = value.toInt()-1;
            if (index >= m_nodeModel.rowCount())
                break;
            auto& oldNode = m_nodeModel.itemAt(toSet->nodeIndex_2());
            oldNode.removeEdge(*toSet);
            auto& newNode = m_nodeModel.itemAt(index);
            toSet->setNode_2(newNode);
            newNode.addEdge(*toSet);
            toSet->adjust();
            break;
        }
        case 3:
            toSet->youngsModulus = value.toReal();
            break;
        case 4:
            toSet->area = value.toReal();
            break;
        case 5:
            toSet->area_2 = value.toReal();
            break;
        default:
            return false;
        }

        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags ElementModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

void ElementModel::saveTo(std::ofstream &stream) const
{
    for (int i=0; i<elements.size(); ++i)
        stream << "E " << (i + 1) << " " << elements[i] << std::endl;
}

Element &ElementModel::appendRow(unsigned int node_1, unsigned int node_2, const QLineF &line)
{
    emit beginInsertRows(QModelIndex(), elements.size(), elements.size());
    elements.append(new Element(m_nodeModel.itemAt(node_1), m_nodeModel.itemAt(node_2), line));
    m_scene.addItem(elements.back()->getItem());
    emit endInsertRows();
    return *elements.back();
}


QDataStream &operator<<(QDataStream &stream, const ElementModel &elementModel)
{
    stream << quint32(elementModel.elements.size());
    for (const auto& element : elementModel.elements)
        operator<<(stream, *element);
    return stream;
}


QDataStream &operator>>(QDataStream &stream, ElementModel &elementModel)
{
    auto& list = elementModel.elements;
    list.clear();
    quint32 c;
    stream >> c;
    list.reserve(c);
    elementModel.beginInsertRows(QModelIndex(), 0, c-1);
    for(quint32 i = 0; i < c; ++i)
    {
        quint16 type;
        quint64 nodeIndex_1, nodeIndex_2;
        double youngsModulus, area, area_2;

        stream >> type >> nodeIndex_1 >> nodeIndex_2 >> youngsModulus >> area >> area_2;

        auto& nodeModel = elementModel.m_nodeModel;
        auto& node_1 = nodeModel.itemAt(nodeIndex_1);
        auto& node_2 = nodeModel.itemAt(nodeIndex_2);
        QLineF line(node_1.position(), node_2.position());
        Element* element = new Element(node_1, node_2, line);
        element->type = type;
        element->youngsModulus = youngsModulus;
        element->area = area;
        element->area_2 = area_2;

        list.append(element);
        elementModel.m_scene.addItem(element->getItem());

        node_1.addEdge(*element);
        node_2.addEdge(*element);

        if (stream.atEnd())
            break;
    }
    elementModel.endInsertRows();

    return stream;
}
