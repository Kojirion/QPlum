#ifndef ELEMENTSMODEL_HPP
#define ELEMENTSMODEL_HPP

#include <QAbstractTableModel>
#include <fstream>

struct Element {
    unsigned int type, node_1, node_2;
    double youngsModulus, area, area_2;   
};

std::ostream& operator<<(std::ostream& stream, const Element& element);

class ElementsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ElementsModel(QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
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
