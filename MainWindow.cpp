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
    std::ofstream file("data.txt");
    file << "DOF  DX  DY  ROTZ" << std::endl
           << "NEL " << elementsModel->rowCount() << std::endl
           << "NNOD " << nodesModel->rowCount() << std::endl;

    nodesModel->saveTo(file);
    elementsModel->saveTo(file);
    nodesModel->saveFixesLoadsTo(file);
    file << "A" << std::endl;
}
