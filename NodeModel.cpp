#include "NodeModel.hpp"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPointF>

NodeModel::NodeModel(QGraphicsScene &scene, QObject *parent) :
    QAbstractTableModel(parent),
    m_scene(scene)
{
}

int NodeModel::rowCount(const QModelIndex &parent) const
{
    return nodes.size();
}

int NodeModel::columnCount(const QModelIndex &parent) const
{
    return 8;
}

QVariant NodeModel::data(const QModelIndex &index, int role) const
{
    if ((!index.isValid()) ||(index.row() >= nodes.size()))
        return QVariant();

    if (role == Qt::DisplayRole){
        auto& toReturn = nodes[index.row()];

        switch (index.column()) {
        case 0:
            return toReturn->position().x();
        case 1:
            return toReturn->position().y();
        case 2:
            if (toReturn->fixity.x)
                return QVariant(*toReturn->fixity.x);
            else return "None";
        case 3:
            if (toReturn->fixity.y)
                return QVariant(*toReturn->fixity.y);
            else return "None";
        case 4:
            if (toReturn->fixity.z)
                return QVariant(*toReturn->fixity.z);
            else return "None";
        case 5:
            return toReturn->load.x;
        case 6:
            return toReturn->load.y;
        case 7:
            return toReturn->load.z;
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QVariant NodeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole){
        if (orientation == Qt::Horizontal){
            switch (section) {
            case 0:
                return "X";
            case 1:
                return "Y";
            case 2:
                return "Fix x";
            case 3:
                return "Fix y";
            case 4:
                return "Fix z";
            case 5:
                return "Load x";
            case 6:
                return "Load y";
            case 7:
                return "Load z";
            default:
                return QVariant();
            }
        }else if (orientation == Qt::Vertical){
            return section+1;
        }
    }

    return QVariant();
}

bool NodeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole){
        if ((!index.isValid()) ||(index.row() >= nodes.size()))
            return false;

        auto& toSet = nodes[index.row()];

        switch (index.column()) {
        case 0:
            toSet->setX(value.toReal());
            break;
        case 1:
            toSet->setY(value.toReal());
            break;
        case 2:
            if (value.toString() == "None")
                toSet->fixity.x.reset();
            else toSet->fixity.x = value.toReal();
            break;
        case 3:
            if (value.toString() == "None")
                toSet->fixity.y.reset();
            else toSet->fixity.y = value.toReal();
            break;
        case 4:
            if (value.toString() == "None")
                toSet->fixity.z.reset();
            else toSet->fixity.z = value.toReal();
            break;
        case 5:
            toSet->load.x = value.toReal();
            break;
        case 6:
            toSet->load.y = value.toReal();
            break;
        case 7:
            toSet->load.z = value.toReal();
            break;
        default:
            return false;
        }

        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags NodeModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

void NodeModel::appendRow(const QPointF &point)
{
    auto index = nodes.size();
    emit beginInsertRows(QModelIndex(), index, index);
    nodes.append(new Node(index));
    auto& node = nodes.back();
    m_scene.addItem(node);
    node->setX(point.x());
    node->setY(point.y());
    emit endInsertRows();
}

void NodeModel::saveTo(std::ofstream &stream) const
{
    for (int i=0; i<nodes.size(); ++i)
        stream << "N " << (i+1) << " " << nodes[i] << std::endl;
}

void NodeModel::saveFixesLoadsTo(std::ofstream &stream) const
{
    for (int i=0; i<nodes.size(); ++i){
        if (nodes[i]->fixity.x)
            stream << "DX " << (i+1) << " " << *nodes[i]->fixity.x << std::endl;
        if (nodes[i]->fixity.y)
            stream << "DY " << (i+1) << " " << *nodes[i]->fixity.y << std::endl;
        if (nodes[i]->fixity.z)
            stream << "ROTZ " << (i+1) << " " << *nodes[i]->fixity.z << std::endl;

        if (nodes[i]->load.x != 0.)
            stream << "FX " << (i+1) << " " << nodes[i]->load.x << std::endl;
        if (nodes[i]->load.y != 0.)
            stream << "FY " << (i+1) << " " << nodes[i]->load.y << std::endl;
        if (nodes[i]->load.z != 0.)
            stream << "MZ " << (i+1) << " " << nodes[i]->load.z << std::endl;
    }
}

void NodeModel::makeMovable()
{
    for (auto& node : nodes)
        node->setFlag(QGraphicsItem::ItemIsMovable);
}

void NodeModel::makeImmovable()
{
    for (auto& node :nodes)
        node->setFlag(QGraphicsItem::ItemIsMovable, false);
}

const Node &NodeModel::itemAt(unsigned int index) const
{
    return *nodes[index];
}

void NodeModel::attachEdge(unsigned int node_1, unsigned int node_2, Element &element)
{
    nodes[node_1]->addEdge(element);
    nodes[node_2]->addEdge(element);
}


QDataStream &operator<<(QDataStream &stream, const NodeModel &nodeModel)
{
    stream << quint32(nodeModel.nodes.size());
    for (const auto& node : nodeModel.nodes)
        operator<<(stream, *node);
    return stream;
}


QDataStream &operator>>(QDataStream &stream, NodeModel &nodeModel)
{
    auto& list = nodeModel.nodes;
    list.clear();
    quint32 c;
    stream >> c;
    list.reserve(c);
    nodeModel.beginInsertRows(QModelIndex(), 0, c-1);
    for(quint32 i = 0; i < c; ++i)
    {
        QPointF position;
        OptionalVector fixity;
        Vector load;
        stream >> position >> fixity.x >> fixity.y >> fixity.z >> load.x >> load.y >> load.z;

        Node* node = new Node(i);

        node->setX(position.x());
        node->setY(position.y());
        node->fixity = fixity;
        node->load = load;

        list.append(node);
        if (stream.atEnd())
            break;
    }
    nodeModel.endInsertRows();
    return stream;
}
