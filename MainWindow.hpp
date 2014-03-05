#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QStateMachine>

class Scene;

namespace Ui {
class MainWindow;
}

class NodeModel;
class ElementModel;
class QGraphicsLineItem;
class QTableView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void createAnalysisFile() const;
    void sceneClicked(const QPointF& point);
    void click_1();
    void tempElement(const QPointF &point);
    void newElement(const QPointF &point_2);
    void mouseMoved(const QPointF& point);
    void save();
    
private:
    enum class State {
        Movable,
        Click_1,
        Click_2
    };

    unsigned int getGrabberIndex(const QPointF &point); //has potential side effect

    Ui::MainWindow *ui;
    Scene* m_scene;
    NodeModel* nodesModel;
    ElementModel* elementsModel;
    QStateMachine m_stateMachine;
    State m_state;
    QGraphicsLineItem* m_tempElement;
    unsigned int m_node_1;
    QTableView* m_nodeView;
    QTableView* m_elementView;
};

#endif // MAINWINDOW_HPP
