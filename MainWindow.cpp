#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include <fstream>
#include "NodesModel.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto nodesModel = new NodesModel; //parent?
    ui->tableView->setModel(nodesModel);
    connect(ui->pushButton, SIGNAL(clicked()), nodesModel, SLOT(appendRow()));
    connect(ui->pushButton_2, SIGNAL(clicked()), nodesModel, SLOT(createFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addRow()
{
    auto model = dynamic_cast<NodesModel*>(ui->tableView->model());
    model->appendRow();
    //ui->tableWidget->insertRow(ui->tableWidget->rowCount());
}

void MainWindow::prepareFile()
{
//    auto nr_rows = ui->tableWidget->rowCount();
//    auto nr_cols = ui->tableWidget->columnCount();

//    std::ofstream file("a.out");

//    for (int i=0; i<nr_rows; ++i) {
//        file << i << " " << ui->tableWidget->row << " " << ui->tableWidget->
//    }
}
