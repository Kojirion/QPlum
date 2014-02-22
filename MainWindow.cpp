#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include <fstream>
#include "NodesModel.hpp"
#include "ElementsModel.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto nodesModel = new NodesModel; //parent?
    ui->nodeView->setModel(nodesModel);
    connect(ui->addNodeButton, SIGNAL(clicked()), nodesModel, SLOT(appendRow()));
    connect(ui->pushButton_2, SIGNAL(clicked()), nodesModel, SLOT(createFile()));

    auto elementsModel = new ElementsModel;
    ui->elementView->setModel(elementsModel);
    connect(ui->addElementButton, SIGNAL(clicked()), elementsModel, SLOT(appendRow()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
