#ifndef ELEMENTSMODEL_HPP
#define ELEMENTSMODEL_HPP

#include <QAbstractTableModel>
#include <fstream>
#include <boost/optional.hpp>

struct OptionalVector {
    boost::optional<double> x, y, z;
};

struct Vector {
    double x,y,z;
};

struct Element {

    Element(unsigned int node_1, unsigned int node_2, double youngsModulus, double area, double area_2);

    unsigned int node_1, node_2;
    double youngsModulus, area, area_2;


    OptionalVector fixity;
    Vector load;
};

std::ostream& operator<<(std::ostream& stream, const Element& element);

class ElementsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ElementsModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void saveTo(std::ofstream& stream) const;
    
signals:
    
public slots:
    void appendRow();

private:
    QList<Element> elements;
    
};

#endif // ELEMENTSMODEL_HPP
