#include "startwindow.h"
#include "ui_startwindow.h"

startwindow::startwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::startwindow)
{
    ui->setupUi(this);
    mainwindow = new MainWindow();
    //debugwindow = new DebugWindow();
    connect(mainwindow,&MainWindow::startwindow,this,&MainWindow::show);
    //connect(debugwindow,&DebugWindow::startwindow,this,&MainWindow::show);
    //mainwindow->GetDebug(debugwindow);
    serialuse = new Serialuse();
    //connect(serialuse,&Serialuse::outData,debugwindow,&DebugWindow::inMess);
    //connect(serialuse,&Serialuse::inDataSignal,debugwindow,&DebugWindow::outMess);
    //connect(serialuse,&Serialuse::ErrorSerial,debugwindow,&DebugWindow::SErrors);
    //connect(serialuse,&Serialuse::CriticalError,debugwindow,&DebugWindow::show);
    //connect(serialuse,&Serialuse::CriticalError,mainwindow,&MainWindow::onCriticalError);
}

startwindow::~startwindow()
{
    delete ui;
}

void startwindow::on_pushButton_3_clicked()
{
    ui->comboBox->clear();
    qDebug()<<portsinfo->availablePorts().length();
    for (int i=0;i<portsinfo->availablePorts().length();i++)
    {
        ui->comboBox->addItem(portsinfo->availablePorts()[i].portName());
    }
    qDebug()<<"update";
}


void startwindow::on_pushButton_clicked()
{
    serialuse->portName=ui->comboBox->currentText();
    serialuse->baund=9600;
    serialuse->initPort();
    serialuse->writePort(QByteArray().append("checkcon"));
    qDebug()<<"send";
}
void startwindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    showEventHelper();
}

void startwindow::datain(QByteArray indata)  //Проверка полученых данных
{
    //QString DataString = QString::fromLatin1(indata); //Перевод в QString
    //controllerData += DataString;
    if (indata.indexOf("gut") != -1)
    {
        mainwindow->GetSerial(serialuse);
        mainwindow->show();
        this->close();
        disconnect(serialuse,&Serialuse::outData,this,&startwindow::datain);
        return;
    }
    else
    {
        qDebug()<<"wrong";
    }
}

void startwindow::showEventHelper() //Открытие окна
{
    ui->comboBox->clear();
    qDebug()<<portsinfo->availablePorts().length();
    for (int i=0;i<portsinfo->availablePorts().length();i++)
    {
        ui->comboBox->addItem(portsinfo->availablePorts()[i].portName());
    }
    connect(serialuse,&Serialuse::outData,this,&startwindow::datain);
    //ui->pushButton->setEnabled(false);
}
