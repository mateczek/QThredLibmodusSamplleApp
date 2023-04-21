#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mbe=new modbusEngine(reg,"127.0.0.1",0,sizeof(reg)/2,this);
    connect(mbe,SIGNAL(dataReaded()),this,SLOT(dataReaded()));
    connect(mbe,SIGNAL(finished()),this,SLOT(reconnect()));  //sygnał koniec wątku do slotu odpowiedzialnego za odświerzanie
    connect(mbe,&modbusEngine::modbusOk,[=](bool ok){
        ui->centralwidget->setEnabled(ok); //wygaszanie interface gdy błąd modbusa
    });
    mbe->start();
    ui->centralwidget->setDisabled(true);
}

MainWindow::~MainWindow()
{
    mbe->quit();
    mbe->wait();
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    bool ok;
    bool val= ui->lineEdit->text().toInt(&ok);
    if (ok)mbe->writeBit(1,val);
}

void MainWindow::dataReaded() //dane odczytane na widget
{
    ui->lcdNumber->display(reg[0]);
    ui->lcdNumber_2->display(reg[1]);
}

void MainWindow::reconnect()
{
    //odświerzanie połączenia po 10 sekundach od zerwania
    QTimer::singleShot(10000,[=](){
        mbe->start();

    });
}


void MainWindow::on_pushButton_2_clicked()
{
    bool ok;
    int val= ui->lineEdit_3->text().toInt(&ok);
    if (ok) mbe->writeWord(0,val);
}

