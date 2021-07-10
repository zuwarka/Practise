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

void MainWindow::on_button_off_wifi_clicked()
{
    ui->lamp_wifi->setBackgroundBrush(Qt::red);
    serialuse->writePort(QByteArray().append("wifioff"));
    ui->label_wifi->setText("Wi-Fi модуль деактивирован");
}

void MainWindow::on_button_on_window_clicked()
{
    ui->lamp_window->setBackgroundBrush(Qt::green);
    serialuse->writePort(QByteArray().append("winon"));
    ui->label_window->setText("Окно открыто");
}

void MainWindow::on_button_off_window_clicked()
{
    ui->lamp_window->setBackgroundBrush(Qt::red);
    serialuse->writePort(QByteArray().append("winoff"));
    ui->label_window->setText("Окно закрыто");
}

void MainWindow::on_button_on_pump_clicked()
{
    ui->lamp_pump->setBackgroundBrush(Qt::green);
    serialuse->writePort(QByteArray().append("pumpon"));
    ui->label_pump->setText("Насос активирован");
}

void MainWindow::on_button_off_pump_clicked()
{
    ui->lamp_pump->setBackgroundBrush(Qt::red);
    serialuse->writePort(QByteArray().append("pumpoff"));
    ui->label_pump->setText("Насос деактивирован");
}

void MainWindow::GetSerial(Serialuse *obj)
{
    serialuse = obj;
}

void MainWindow::GetData() //Запрос на получение данных
{
    if (serialuse->connected)
    {
        serialuse->writePort(QByteArray().append("getdata"));
    }
}

void MainWindow::datain(QByteArray indata) //Обработка полученных с контроллера данных
{
    QString DataString = QString::fromLatin1(indata); //Перевод в QString
    QString BufferString;
    controllerData += DataString;
    int val = 0;
    bool ok;
    if (indata.indexOf("paramds") != -1) //Признак начала передачи
    {

        if (controllerData.indexOf("temp2:") == 1) //перед нами часть строки с температурой
        {
            for(int i = 1; ; i++)
            {
                if(controllerData[controllerData.indexOf("temp2:")+i] >= 0 ||
                   controllerData[controllerData.indexOf("temp2:")+i] <= 9)   //в буффере теперь число из символов
                {
                    BufferString = controllerData[controllerData.indexOf("temp2:")+i];
                }
                else
                {
                    break;
                }
            }
            val = BufferString.toInt(&ok, 10);
            ui->panel_out_temp->display(val);
            BufferString.clear();
        }

        if (controllerData.indexOf("humidair:") == 1)
        {
            for(int i = 1; ; i++)
            {
                if(controllerData[controllerData.indexOf("humidair:")+i] >= 0 ||
                   controllerData[controllerData.indexOf("humidair:")+i] <= 9)   //в буффере теперь число из символов
                {
                    BufferString = controllerData[controllerData.indexOf("humidair:")+i];
                }
                else
                {
                    break;
                }
            }
            val = BufferString.toInt(&ok, 10);
            ui->panel_out_air->display(val);
            BufferString.clear();
        }

        if (controllerData.indexOf("humidearth:") == 1)
        {
            for(int i = 1; ; i++)
            {
                if(controllerData[controllerData.indexOf("humidearth:")+i] >= 0 ||
                   controllerData[controllerData.indexOf("humidearth:")+i] <= 9)   //в буффере теперь число из символов
                {
                    BufferString = controllerData[controllerData.indexOf("humidearth:")+i];
                }
                else
                {
                    break;
                }
            }
            val = BufferString.toInt(&ok, 10);
            ui->panel_out_ground->display(val);
            BufferString.clear();
        }

        if (controllerData.indexOf("gasq:") == 1)
        {
            for(int i = 1; ; i++)
            {
                if(controllerData[controllerData.indexOf("gasq:")+i] >= 0 ||
                   controllerData[controllerData.indexOf("gasq:")+i] <= 9)   //в буффере теперь число из символов
                {
                    BufferString = controllerData[controllerData.indexOf("gasq:")+i];
                }
                else
                {
                    break;
                }
            }
            val = BufferString.toInt(&ok, 10);
            ui->panel_out_co->display(val);
            BufferString.clear();
        }

        if (controllerData.indexOf("psimmhg:") == 1)
        {
            for(int i = 1; ; i++)
            {
                if(controllerData[controllerData.indexOf("psimmhg:")+i] >= 0 ||
                   controllerData[controllerData.indexOf("psimmhg:")+i] <= 9)   //в буффере теперь число из символов
                {
                    BufferString = controllerData[controllerData.indexOf("psimmhg:")+i];
                }
                else
                {
                    break;
                }
            }
            val = BufferString.toInt(&ok, 10);
            ui->panel_out_hg->display(val);
            BufferString.clear();
        }
        controllerData.clear();
    }
}
