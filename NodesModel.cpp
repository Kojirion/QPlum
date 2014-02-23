#include "NodesModel.hpp"

NodesModel::NodesModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int NodesModel::rowCount(const QModelIndex &parent) const
{
    return nodes.size();
}

int NodesModel::columnCount(const QModelIndex &parent) const
{
    return 8;
}

QVariant NodesModel::data(const QModelIndex &index, int role) const
{
    if ((!index.isValid()) ||(index.row() >= nodes.size()))
        return QVariant();

    if (role == Qt::DisplayRole){
        auto& toReturn = nodes[index.row()];

        switch (index.column()) {
        case 0:
            return QVariant(toReturn.x);
        case 1:
            return QVariant(toReturn.y);
        case 2:
            if (toReturn.fixity.x)
                return QVariant(*toReturn.fixity.x);
            else return "None";
        case 3:
            if (toReturn.fixity.y)
                return QVariant(*toReturn.fixity.y);
            else return "None";
        case 4:
            if (toReturn.fixity.z)
                return QVariant(*toReturn.fixity.z);
            else return "None";
        case 5:
            return toReturn.load.x;
        case 6:
            return toReturn.load.y;
        case 7:
            return toReturn.load.z;
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QVariant NodesModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool NodesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole){
        if ((!index.isValid()) ||(index.row() >= nodes.size()))
            return false;

        auto& toSet = nodes[index.row()];

        switch (index.column()) {
        case 0:
            toSet.x = value.toReal();
            break;
        case 1:
            toSet.y = value.toReal();
            break;
        case 2:
            if (value.toString() == "None")
                toSet.fixity.x.reset();
            else toSet.fixity.x = value.toReal();
            break;
        case 3:
            if (value.toString() == "None")
                toSet.fixity.y.reset();
            else toSet.fixity.y = value.toReal();
            break;
        case 4:
            if (value.toString() == "None")
                toSet.fixity.z.reset();
            else toSet.fixity.z = value.toReal();
            break;
        case 5:
            toSet.load.x = value.toReal();
            break;
        case 6:
            toSet.load.y = value.toReal();
            break;
        case 7:
            toSet.load.z = value.toReal();
            break;
        default:
            return false;
        }

        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags NodesModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

void NodesModel::appendRow()
{
    emit beginInsertRows(QModelIndex(), nodes.size(), nodes.size());
    nodes.append(Node{0., 0.});
    emit endInsertRows();
}

void NodesModel::saveTo(std::ofstream &stream) const
{
    for (int i=0; i<nodes.size(); ++i)
        stream << "N " << nodes[i] << std::endl;
}

void NodesModel::saveFixesLoadsTo(std::ofstream &stream) const
{
    for (int i=0; i<nodes.size(); ++i){
        if (nodes[i].fixity.x)
            stream << "DX" << (i+1) << *nodes[i].fixity.x << std::endl;
        if (nodes[i].fixity.y)
            stream << "DY" << (i+1) << *nodes[i].fixity.y << std::endl;
        if (nodes[i].fixity.z)
            stream << "ROTZ" << (i+1) << *nodes[i].fixity.z << std::endl;

        if (nodes[i].load.x != 0.)
            stream << "FX" << (i+1) << nodes[i].load.x << std::endl;
        if (nodes[i].load.y != 0.)
            stream << "FY" << (i+1) << nodes[i].load.y << std::endl;
        if (nodes[i].load.z != 0.)
            stream << "MZ" << (i+1) << nodes[i].load.z << std::endl;
    }
}


std::ostream &operator <<(std::ostream &stream, const Node &node)
{
    stream << node.x << " " << node.y;
    return stream;
}
