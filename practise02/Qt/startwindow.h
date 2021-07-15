#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QtDebug>
#include <mainwindow.h>
//#include <debugwindow.h>
#include <QMessageBox>
#include <serialuse.h>
#include <QThread>
#include <QString>

namespace Ui {
class startwindow;
}

class startwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit startwindow(QWidget *parent = nullptr);
    ~startwindow();
    Serialuse *serialuse;
private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void datain(QByteArray indata);

private:
    Ui::startwindow *ui;
    MainWindow *mainwindow;
    //DebugWindow *debugwindow;
    QSerialPortInfo *portsinfo;
    void showEventHelper();
    QString controllerData;
protected:
    void showEvent(QShowEvent *ev);
};

#endif // STARTWINDOW_H
