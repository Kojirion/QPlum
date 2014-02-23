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
    return 11;
}

QVariant ElementsModel::data(const QModelIndex &index, int role) const
{
    if ((!index.isValid()) ||(index.row() >= elements.size()))
        return QVariant();

    if (role == Qt::DisplayRole){
        auto& toReturn = elements[index.row()];

        switch (index.column()) {
        case 0:
            return QVariant(toReturn.node_1);
        case 1:
            return QVariant(toReturn.node_2);
        case 2:
            return QVariant(toReturn.youngsModulus);
        case 3:
            return QVariant(toReturn.area);
        case 4:
            return QVariant(toReturn.area_2);
        case 5:
            if (toReturn.fixity.x)
                return QVariant(*toReturn.fixity.x);
            else return "None";
        case 6:
            if (toReturn.fixity.y)
                return QVariant(*toReturn.fixity.y);
            else return "None";
        case 7:
            if (toReturn.fixity.z)
                return QVariant(*toReturn.fixity.z);
            else return "None";
        case 8:
            return toReturn.load.x;
        case 9:
            return toReturn.load.y;
        case 10:
            return toReturn.load.z;
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
                return "Node 1";
            case 1:
                return "Node 2";
            case 2:
                return "Young's Modulus";
            case 3:
                return "Area";
            case 4:
                return "Area 2nd moment";
            case 5:
                return "Fix x";
            case 6:
                return "Fix y";
            case 7:
                return "Fix z";
            case 8:
                return "Load x";
            case 9:
                return "Load y";
            case 10:
                return "Load z";
            default:
                return QVariant();
            }
        }else if (orientation == Qt::Vertical){
            return section;
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
            toSet.node_1 = value.toInt();
            break;
        case 1:
            toSet.node_2 = value.toInt();
            break;
        case 2:
            toSet.youngsModulus = value.toReal();
            break;
        case 3:
            toSet.area = value.toReal();
            break;
        case 4:
            toSet.area_2 = value.toReal();
            break;
        case 5:
            if (value.toString() == "None")
                toSet.fixity.x.reset();
            else toSet.fixity.x = value.toReal();
            break;
        case 6:
            toSet.fixity.y = value.toReal();
            break;
        case 7:
            toSet.fixity.z = value.toReal();
            break;
        case 8:
            toSet.load.x = value.toReal();
            break;
        case 9:
            toSet.load.y = value.toReal();
            break;
        case 10:
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
    elements.append(Element(0, 0, 2E-8, 0., 0.));
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


Element::Element(unsigned int node_1, unsigned int node_2, double youngsModulus, double area, double area_2):
    node_1(node_1), node_2(node_2), youngsModulus(youngsModulus), area(area), area_2(area_2), load{0, 0, 0}
{

}
