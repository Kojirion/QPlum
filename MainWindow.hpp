#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class NodesModel;
class ElementsModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void createAnalysisFile() const;
    
private:
    Ui::MainWindow *ui;
    NodesModel* nodesModel;
    ElementsModel* elementsModel;
};

#endif // MAINWINDOW_HPP
