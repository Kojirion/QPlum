#include "ElementsModel.hpp"

ElementsModel::ElementsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int ElementsModel::rowCount(const QModelIndex &parent) const
{
    return elements.size();
}

int ElementsModel::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant ElementsModel::data(const QModelIndex &index, int role) const
{
    if ((!index.isValid()) ||(index.row() >= elements.size()))
        return QVariant();

    if (role == Qt::DisplayRole){
        auto& toReturn = elements[index.row()];

        switch (index.column()) {
        case 0:
            return toReturn.type;
        case 1:
            return QVariant(toReturn.node_1);
        case 2:
            return QVariant(toReturn.node_2);
        case 3:
            return QVariant(toReturn.youngsModulus);
        case 4:
            return QVariant(toReturn.area);
        case 5:
            return QVariant(toReturn.area_2);
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QVariant ElementsModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool ElementsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole){
        if ((!index.isValid()) ||(index.row() >= elements.size()))
            return false;

        auto& toSet = elements[index.row()];

        switch (index.column()) {
        case 0:
            toSet.type = value.toInt();
            break;
        case 1:
            toSet.node_1 = value.toInt();
            break;
        case 2:
            toSet.node_2 = value.toInt();
            break;
        case 3:
            toSet.youngsModulus = value.toReal();
            break;
        case 4:
            toSet.area = value.toReal();
            break;
        case 5:
            toSet.area_2 = value.toReal();
            break;
        default:
            return false;
        }

        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags ElementsModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

void ElementsModel::saveTo(std::ofstream &stream) const
{
    for (int i=0; i<elements.size(); ++i)
        stream << "E " << elements[i] << std::endl;
}

void ElementsModel::appendRow()
{
    emit beginInsertRows(QModelIndex(), elements.size(), elements.size());
    elements.append(Element{1, 0, 0, 2E-8, 0.01, 3E-4});
    emit endInsertRows();
}


std::ostream &operator <<(std::ostream &stream, const Element &element)
{
    stream << element.node_1 << " "
           << element.node_2 << " "
           << element.youngsModulus << " "
           << element.area << " "
           << element.area_2;
    return stream;
}
