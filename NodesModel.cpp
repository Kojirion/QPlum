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
    return 2;
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
            default:
                return QVariant();
            }
        }else if (orientation == Qt::Vertical){
            return section;
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
    //return Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void NodesModel::appendRow()
{
    emit beginInsertRows(QModelIndex(), nodes.size(), nodes.size());
    nodes.append(Node{0., 0.});
    emit endInsertRows();
}
