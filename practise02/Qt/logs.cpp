#include "logs.h"
#include "ui_logs.h"

Logs::Logs(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Logs)
{
    ui->setupUi(this);
    ui->ReciveTable_2->clear();
    ui->ReciveTable_2->setColumnCount(2);
    ui->ReciveTable_2->setHorizontalHeaderItem(0,new QTableWidgetItem(QString("Время")));
    ui->ReciveTable_2->setHorizontalHeaderItem(1,new QTableWidgetItem(QString("Сообщение")));
    ui->SendTable_2->clear();
    ui->SendTable_2->setColumnCount(2);
    ui->SendTable_2->setHorizontalHeaderItem(0,new QTableWidgetItem(QString("Время")));
    ui->SendTable_2->setHorizontalHeaderItem(1,new QTableWidgetItem(QString("Сообщение")));
}

Logs::~Logs()
{
    delete ui;
}

void Logs::SErrors(QString mess) //Вывод ошибок в таблицу
{
    ui->ReciveTable_2->insertRow(ui->ReciveTable_2->rowCount());
    ui->ReciveTable_2->setItem(ui->ReciveTable_2->rowCount()-1,0,new QTableWidgetItem(QTime::currentTime().toString()));
    ui->ReciveTable_2->setItem(ui->ReciveTable_2->rowCount()-1,1,new QTableWidgetItem(mess));
}

void Logs::inMess(QByteArray mess) //Вывод сообщений в таблицу
{
    ui->ReciveTable_2->insertRow(ui->ReciveTable_2->rowCount());
    ui->ReciveTable_2->setItem(ui->ReciveTable_2->rowCount()-1,0,new QTableWidgetItem(QTime::currentTime().toString()));
    ui->ReciveTable_2->setItem(ui->ReciveTable_2->rowCount()-1,1,new QTableWidgetItem(QString::fromUtf8(mess)));
    if (ui->ReciveTable_2->rowCount()>1000)
    {
        ui->ReciveTable_2->clear();
        ui->ReciveTable_2->setRowCount(0);
        ui->ReciveTable_2->setColumnCount(2);
        ui->ReciveTable_2->setHorizontalHeaderItem(0,new QTableWidgetItem(QString("Время")));
        ui->ReciveTable_2->setHorizontalHeaderItem(1,new QTableWidgetItem(QString("Сообщение")));
    }
}

void Logs::outMess(QByteArray mess)  //Вывод сообщений в таблицу
{
    ui->SendTable_2->insertRow(ui->SendTable_2->rowCount());
    ui->SendTable_2->setItem(ui->SendTable_2->rowCount()-1,0,new QTableWidgetItem(QTime::currentTime().toString()));
    ui->SendTable_2->setItem(ui->SendTable_2->rowCount()-1,1,new QTableWidgetItem(QString::fromUtf8(mess)));
    if (ui->SendTable_2->rowCount()>1000)
    {
        ui->SendTable_2->clear();
        ui->SendTable_2->setRowCount(0);
        ui->SendTable_2->setColumnCount(2);
        ui->SendTable_2->setHorizontalHeaderItem(0,new QTableWidgetItem(QString("Время")));
        ui->SendTable_2->setHorizontalHeaderItem(1,new QTableWidgetItem(QString("Сообщение")));
    }
}

void Logs::on_clearSend_clicked()
{
    ui->SendTable_2->clear();
    ui->SendTable_2->setRowCount(0);
    ui->SendTable_2->setColumnCount(2);
    ui->SendTable_2->setHorizontalHeaderItem(0,new QTableWidgetItem(QString("Время")));
    ui->SendTable_2->setHorizontalHeaderItem(1,new QTableWidgetItem(QString("Сообщение")));
}


void Logs::on_clearRecive_clicked()
{
    ui->ReciveTable_2->clear();
    ui->ReciveTable_2->setRowCount(0);
    ui->ReciveTable_2->setColumnCount(2);
    ui->ReciveTable_2->setHorizontalHeaderItem(0,new QTableWidgetItem(QString("Время")));
    ui->ReciveTable_2->setHorizontalHeaderItem(1,new QTableWidgetItem(QString("Сообщение")));
}
