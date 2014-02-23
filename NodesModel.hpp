#ifndef NODESMODEL_HPP
#define NODESMODEL_HPP
#include <QAbstractTableModel>
#include <QList>
#include <fstream>

struct Node {
    double x,y;
};

std::ostream& operator<<(std::ostream& stream, const Node& node);

class NodesModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit NodesModel(QObject *parent = 0);

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
    QList<Node> nodes;
    
};

#endif // NODESMODEL_HPP
