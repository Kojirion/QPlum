#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include <fstream>
#include "Scene.hpp"
#include <QGraphicsEllipseItem>
#include "NodeModel.hpp"
#include "ElementModel.hpp"
#include <QPlainTextEdit>
#include <QFile>
#include <QProcess>
#include <QGraphicsLineItem>
#include <QTableView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_scene(new Scene),
    nodesModel(new NodeModel(*m_scene)),
    elementsModel(new ElementModel(*nodesModel, *m_scene)),
    m_state(State::Movable),
    m_tempElement(new QGraphicsLineItem),
    m_nodeView(new QTableView),
    m_elementView(new QTableView)
{
    m_scene->setSceneRect(0, 0, 10, 10);
    m_scene->addItem(m_tempElement);
    m_tempElement->setVisible(false);

    m_nodeView->setModel(nodesModel);
    m_elementView->setModel(elementsModel);

    ui->setupUi(this);

    ui->graphicsView->setScene(m_scene);

//    ui->nodeView->setModel(nodesModel);
    connect(ui->addNodeButton, SIGNAL(clicked()), nodesModel, SLOT(appendRow()));

    connect(ui->nodeListButton, SIGNAL(clicked()), m_nodeView, SLOT(show()));

//    ui->elementView->setModel(elementsModel);
    connect(ui->addElementButton, SIGNAL(clicked()), this, SLOT(click_1()));
    //connect(m_scene, SIGNAL())


    connect(ui->analysisButton, SIGNAL(clicked()), this, SLOT(createAnalysisFile()));

    connect(m_scene, SIGNAL(clicked(QPointF)), this, SLOT(sceneClicked(QPointF)));
    connect(m_scene, SIGNAL(mouseMoved(QPointF)), this, SLOT(mouseMoved(QPointF)));

    connect(ui->elementListButton, SIGNAL(clicked()), m_elementView, SLOT(show()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createAnalysisFile() const
{
    std::ofstream file("data.txt");
    file << "DOF  DX  DY  ROTZ" << std::endl
           << "NEL " << elementsModel->rowCount() << std::endl
           << "NNOD " << nodesModel->rowCount() << std::endl;

    nodesModel->saveTo(file);
    elementsModel->saveTo(file);
    nodesModel->saveFixesLoadsTo(file);
    file << "A" << std::endl;

    QProcess process;
    process.start("./plum");
    if (process.waitForStarted()){
        process.write("HP\n");
        process.write("output.txt\n");
        process.write("I data.txt\n");
    }
    process.waitForFinished();

    QFile output("output.txt");
    output.open(QIODevice::ReadOnly);

    auto text = new QPlainTextEdit(output.readAll().data());
    text->show();

}

void MainWindow::sceneClicked(const QPointF &point)
{
    switch (m_state) {
    case State::Movable:
        return;
        break;
    case State::Click_1:
        tempElement(point);
        break;
    case State::Click_2:
        newElement(point);
        break;
    default:
        break;
    }
}

void MainWindow::click_1()
{
    nodesModel->makeImmovable();
    m_state = State::Click_1;
}

void MainWindow::tempElement(const QPointF& point)
{
    ui->graphicsView->setMouseTracking(true);
    m_tempElement->setLine(QLineF(point, point));
    m_tempElement->setVisible(true);
    m_state = State::Click_2;
    m_node_1 = getGrabberIndex(point);
}

void MainWindow::newElement(const QPointF &point_2)
{
    nodesModel->makeMovable();
    m_state = State::Movable;
    m_tempElement->setVisible(false);
    ui->graphicsView->setMouseTracking(false);
    auto index_2 = getGrabberIndex(m_tempElement->line().p2());
    auto& element = elementsModel->appendRow(m_node_1, index_2, m_tempElement->line());
    nodesModel->attachEdge(m_node_1, index_2, element);
}

void MainWindow::mouseMoved(const QPointF &point)
{
    if (m_state != State::Click_2)
        return;

    m_tempElement->setLine(QLineF(m_tempElement->line().p1(), point));
}

unsigned int MainWindow::getGrabberIndex(const QPointF& point)
{
    auto item = m_scene->itemAt(point, QTransform());
    if (item){
        auto index = item->data(0);
        if (index.isValid())
            return index.toUInt();
    }

    nodesModel->appendRow(point);
    return nodesModel->rowCount();
}
