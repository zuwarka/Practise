#include "serialuse.h"

Serialuse::Serialuse(QObject *parent) : QObject(parent)
{
    connected = false;
    connect(&port,SIGNAL(readyRead()),this,SLOT(readData()));
    connect(&port, &QSerialPort::errorOccurred, this, &Serialuse::SerialErrors);
}
Serialuse::~Serialuse()
{

}

void Serialuse::initPort()
{
    stopPort();
    port.setPortName(portName);
    port.dataTerminalReadyChanged(0);
    port.parity();
    port.stopBits();
    if (port.open(QIODevice::ReadWrite))
    {
        connected = true;
    }
}

void Serialuse::writePort(QByteArray inData)
{
    port.write(inData);
    inDataSignal(inData);
    port.waitForBytesWritten();
}

void Serialuse::readData()
{
    data = port.readAll();
    outData(data);
}

void Serialuse::stopPort()
{
    if(port.isOpen())
    {
        port.close();
    }
    connected = false;
}

void Serialuse::SerialErrors(QSerialPort::SerialPortError error) //Обработчик ошибок
{
    ErrorSerial(port.errorString());
    if (error == QSerialPort::ResourceError && port.isOpen() && connected)
    {
        stopPort();
        emit CriticalError();
        msgBox.setText("Критическая ошибка последовательного порта!");
        msgBox.exec();
        qDebug() << "Критическая ошибка порта";
    }
}
