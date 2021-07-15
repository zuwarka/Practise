#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this); //ставим цвета ламп на черные по умолчанию
    QBrush pen(Qt::black);
    ui->lamp_wifi->setScene(scene);
    ui->lamp_wifi->scene()->setBackgroundBrush(pen);
    scene = new QGraphicsScene(this);
    ui->lamp_pump->setScene(scene);
    ui->lamp_pump->scene()->setBackgroundBrush(pen);
    scene = new QGraphicsScene(this);
    ui->lamp_window->setScene(scene);
    ui->lamp_window->scene()->setBackgroundBrush(pen);

    QAction *quit = new QAction("&Выход", this);
    QAction *status = new QAction("&Статистика", this);

    // Создаем объект класса QMenu (меню)
    QMenu *file;
    file = menuBar()->addMenu("&Файл");

    // Помещаем действие "Quit" (Выход) в меню с помощью метода addAction()
    file->addAction(quit);
    file->addAction(status);

    // Когда мы выбираем в меню опцию "Quit", то приложение сразу же завершает свое выполнение
    connect(quit, &QAction::triggered, qApp, QApplication::quit);
    connect(status, &QAction::triggered, qApp, QApplication::quit); //Вместо quit будет вывод окна со статистикой

    timer = new QTimer; //Создаем объект таймера

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_slider_temp_valueChanged(int value)
{
    ui->panel_temp->display(value);
    serialuse->writePort(QByteArray().append("tempchanged"));
}

void MainWindow::on_slider_co_valueChanged(int value)
{
    ui->panel_co->display(value);
    serialuse->writePort(QByteArray().append("cochanged"));
}

void MainWindow::on_slider_air_valueChanged(int value)
{
    ui->panel_air->display(value);
    serialuse->writePort(QByteArray().append("airchanged"));
}

void MainWindow::on_slider_ground_valueChanged(int value)
{
    ui->panel_ground->display(value);
    serialuse->writePort(QByteArray().append("groundchanged"));
}

void MainWindow::on_button_on_wifi_clicked()
{
    ui->lamp_wifi->setBackgroundBrush(Qt::green);
    serialuse->writePort(QByteArray().append("wifion"));
    ui->label_wifi->setText("Wi-Fi модуль активирован");
}

void MainWindow::on_button_on_window_clicked()
{
    serialuse->writePort(QByteArray().append("openwin"));
}

void MainWindow::on_button_off_window_clicked()
{

    serialuse->writePort(QByteArray().append("closewin"));

}

void MainWindow::on_button_on_pump_clicked()
{

    serialuse->writePort(QByteArray().append("poweronpump"));
}

void MainWindow::on_button_off_pump_clicked()
{
    serialuse->writePort(QByteArray().append("poweroffpump"));
}

void MainWindow::GetSerial(Serialuse *obj)
{
    serialuse = obj;
}

void MainWindow::GetData() //Запрос на получение данных
{
    if (serialuse->connected)
    {
        serialuse->writePort(QByteArray().append("getpar")); //здесь поменял
    }
    qDebug()<<"Что то отправили";
}

void MainWindow::datain(QByteArray indata) //Обработка полученных с контроллера данных
{
    QString DataString = QString::fromLatin1(indata); //Перевод в QString
    QString BufferString;
    controllerData += DataString;
    int val = 0;
    bool ok;
     qDebug()<<"Что то приняли";
    if (indata.indexOf("paramde") != -1) //Признак конца передачи (поправил здесь, ибо обрабатываем всё в конце)
    {
        qDebug()<<"Получили все данные";
        qDebug()<<controllerData;
        if (controllerData.indexOf("temp2:") != -1) //перед нами часть строки с температурой, поменял на !=-1 признак того что найдено
        {
            QString str="temp2:";
            int start=controllerData.indexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()); //Находим начало цифирь
            int finish=controllerData.lastIndexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()+5); //Находим конец цифирь
            QString BufferString=controllerData.mid(start,finish-start+1);
            val = BufferString.toInt(&ok, 10);
            ui->panel_out_temp->display(val);
            BufferString.clear();
        }

        if (controllerData.indexOf("humidair:") != -1)
        {
            QString str="humidair:";
            int start=controllerData.indexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()); //Находим начало цифирь
            int finish=controllerData.lastIndexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()+5); //Находим конец цифирь
            QString BufferString=controllerData.mid(start,finish-start+1); //Запоминаем значения от начала и конца + 1
            val = BufferString.toInt(&ok, 10);
            ui->panel_out_air->display(val);
            BufferString.clear();
        }

        if (controllerData.indexOf("humidearth:") != -1)
        {
            QString str="humidearth:";
            int start=controllerData.indexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()); //Находим начало цифирь
            int finish=controllerData.lastIndexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()+5); //Находим конец цифирь
            QString BufferString=controllerData.mid(start,finish-start+1);
            val = BufferString.toInt(&ok, 10);
            ui->panel_out_ground->display(val);
            BufferString.clear();
        }

        if (controllerData.indexOf("gasq:") != -1)
        {
            QString str="gasq:";
            int start=controllerData.indexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()); //Находим начало цифирь
            int finish=controllerData.lastIndexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()+5); //Находим конец цифирь
            QString BufferString=controllerData.mid(start,finish-start+1);
            val = BufferString.toInt(&ok, 10);
            ui->panel_out_co->display(val);
            BufferString.clear();
        }

        if (controllerData.indexOf("psimmhg:") != -1)
        {QString str="psimmhg:";
            int start=controllerData.indexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()); //Находим начало цифирь
            int finish=controllerData.lastIndexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()+5); //Находим конец цифирь
            QString BufferString=controllerData.mid(start,finish-start+1);
            val = BufferString.toInt(&ok, 10);
            ui->panel_out_hg->display(val);
            BufferString.clear();
        }
        if (controllerData.indexOf("needtemp:") != -1)
        {QString str="needtemp:";
            int start=controllerData.indexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()); //Находим начало цифирь
            int finish=controllerData.lastIndexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()+5); //Находим конец цифирь
            QString BufferString=controllerData.mid(start,finish-start+1);
            val = BufferString.toInt(&ok, 10);
            ui->panel_temp->display(val);
            BufferString.clear();
        }
        if (controllerData.indexOf("needhum:") != -1)
        {QString str="needhum:";
            int start=controllerData.indexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()); //Находим начало цифирь
            int finish=controllerData.lastIndexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()+5); //Находим конец цифирь
            QString BufferString=controllerData.mid(start,finish-start+1);
            val = BufferString.toInt(&ok, 10);
            ui->panel_air->display(val);
            BufferString.clear();
        }
        if (controllerData.indexOf("needhumearth:") != -1)
        {QString str="needhumearth:";
            int start=controllerData.indexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()); //Находим начало цифирь
            int finish=controllerData.lastIndexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()+5); //Находим конец цифирь
            QString BufferString=controllerData.mid(start,finish-start+1);
            val = BufferString.toInt(&ok, 10);
            ui->panel_ground->display(val);
            BufferString.clear();
        }
        if (controllerData.indexOf("maxco2:") != -1)
        {QString str="maxco2:";
            int start=controllerData.indexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()); //Находим начало цифирь
            int finish=controllerData.lastIndexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()+5); //Находим конец цифирь
            QString BufferString=controllerData.mid(start,finish-start+1);
            val = BufferString.toInt(&ok, 10);
            ui->panel_co->display(val);
            BufferString.clear();
        }
        if (controllerData.indexOf("ispumpon:") != -1)
        {QString str="ispumpon:";
            int start=controllerData.indexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()); //Находим начало цифирь
            int finish=controllerData.lastIndexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()+5); //Находим конец цифирь
            QString BufferString=controllerData.mid(start,finish-start+1);
            val = BufferString.toInt(&ok, 10);
            if (val==0){
            ui->lamp_pump->setBackgroundBrush(Qt::red);
            ui->label_pump->setText("Насос деактивирован");
            }
            if (val==1){
            ui->lamp_pump->setBackgroundBrush(Qt::green);
            ui->label_pump->setText("Насос активирован");
            }
            BufferString.clear();

        }
        if (controllerData.indexOf("iswindowopen:") != -1)
        {QString str="iswindowopen:";
            int start=controllerData.indexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()); //Находим начало цифирь
            int finish=controllerData.lastIndexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()+5); //Находим конец цифирь
            QString BufferString=controllerData.mid(start,finish-start+1);
            val = BufferString.toInt(&ok, 10);
            if (val==0){
            ui->lamp_window->setBackgroundBrush(Qt::red);
       ui->label_window->setText("Окно закрыто");
            }
            if (val==1){
             ui->lamp_window->setBackgroundBrush(Qt::green);
                ui->label_window->setText("Окно открыто");
            }
            BufferString.clear();
        }
        if (controllerData.indexOf("autowork:") != -1)
        {QString str="autowork:";
            int start=controllerData.indexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()); //Находим начало цифирь
            int finish=controllerData.lastIndexOf(QRegExp("[0-9]"),controllerData.indexOf(str)+str.size()+5); //Находим конец цифирь
            QString BufferString=controllerData.mid(start,finish-start+1);
            val = BufferString.toInt(&ok, 10);
            if (val==0)
            {

            }
            if (val==1)
            {

            }
            BufferString.clear();
        }
        controllerData.clear();
    }
}

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    showEventHelper();
}
void MainWindow::showEventHelper() //События при появлении окна не экране, при отладке заккоментировать, чтоб программа не вылетела
{
    connect(timer, SIGNAL(timeout()), this, SLOT(GetData())); //Объединяем таймер и запрос
    timer->start(250); //Период таймера 2000 мс
    connect(serialuse,&Serialuse::outData,this,&MainWindow::datain); //Получение данных
}

void MainWindow::on_setauto_clicked()
{
 serialuse->writePort(QByteArray().append("setautowork1"));
}


void MainWindow::on_setmanual_clicked()
{
 serialuse->writePort(QByteArray().append("setautowork0"));
}
