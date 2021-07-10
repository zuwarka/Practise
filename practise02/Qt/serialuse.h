#ifndef SERIALUSE_H
#define SERIALUSE_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>

class Serialuse : public QObject
{
    Q_OBJECT
public:
    Serialuse(QObject *parent = nullptr);
    ~Serialuse();
    QSerialPort port;
    QByteArray data;
    QString portName;
    bool connected;
signals:
    void outData(QByteArray outData);
    void inDataSignal(QByteArray inData);
    void ErrorSerial(QString Error);
    void CriticalError();
public slots:
    void initPort();
    void readData();
    void stopPort();
    void writePort(QByteArray inData);
private slots:
    void SerialErrors(QSerialPort::SerialPortError error);
private:
    QMessageBox msgBox;
};

#endif // SERIALUSE_H
