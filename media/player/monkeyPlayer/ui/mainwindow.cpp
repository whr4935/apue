#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<sys/time.h>

#include <iostream>

#include "source/ScreenGrab.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->push, &QPushButton::clicked, this, &MainWindow::onPush);
 //   connect(ui->stop, &QPushButton::clicked, this, &MainWindow::onStop);
    connect(ui->stop, &QPushButton::clicked, &MainWindow::print);

   // connect(ui->stop, &QPushButton::clicked, [] {std::cout << "lambda slot" << std::endl;});

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::print()
{
    std::cout << "hello print" << std::endl;
}

int MainWindow::onPush()
{
    std::cout << "hello onPush" << std::endl;

    ScreenGrab* grab = new ScreenGrab;
    grab->test();

    delete grab;

    return 0;
}

int MainWindow::onStop()
{
    std::cout << "hello onStop" << std::endl;

    return 0;
}
