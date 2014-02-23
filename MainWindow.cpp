#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include <fstream>
#include "NodesModel.hpp"
#include "ElementsModel.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    nodesModel(new NodesModel),
    elementsModel(new ElementsModel)
{
    ui->setupUi(this);

    ui->nodeView->setModel(nodesModel);
    connect(ui->addNodeButton, SIGNAL(clicked()), nodesModel, SLOT(appendRow()));

    ui->elementView->setModel(elementsModel);
    connect(ui->addElementButton, SIGNAL(clicked()), elementsModel, SLOT(appendRow()));

    connect(ui->analysisButton, SIGNAL(clicked()), this, SLOT(createAnalysisFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createAnalysisFile() const
{
    std::ofstream file("a.out");
    nodesModel->saveTo(file);
    file << std::endl;
    elementsModel->saveTo(file);
}
