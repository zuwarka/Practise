#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QSlider>
#include <QString>
#include <QLCDNumber>
#include <QGraphicsScene>
#include <serialuse.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void GetSerial(Serialuse *obj);
private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene; //для закраски "ламп"
    Serialuse* serialuse; //соединение с портами
    QString controllerData; //строка данных с МК

private slots:
    void on_slider_temp_valueChanged(int value);
    void on_slider_co_valueChanged(int value);
    void on_slider_air_valueChanged(int value);
    void on_slider_ground_valueChanged(int value);
    void on_button_on_wifi_clicked();
    void on_button_off_wifi_clicked();
    void on_button_on_window_clicked();
    void on_button_off_window_clicked();
    void on_button_on_pump_clicked();
    void on_button_off_pump_clicked();
    void on_slider_temp_sliderReleased();
    void GetData(); //Запрос на получение данных
    void datain(QByteArray indata); //Получение данных с МК
};

#endif // MAINWINDOW_H
