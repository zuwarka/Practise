#ifndef LOGS_H
#define LOGS_H

#include <QMainWindow>
#include <QDateTime>
#include <QFile>
#include <serialuse.h>
#include <QByteArray>
#include <QDebug>
#include <QTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextCodec>

namespace Ui {
class Logs;
}

class Logs : public QMainWindow
{
    Q_OBJECT

public:
    explicit Logs(QWidget *parent = nullptr);
    ~Logs();

private:
    Ui::Logs *ui;

public slots:
    void SErrors(QString mess);
    void inMess(QByteArray mess);
    void outMess(QByteArray mess);
private slots:
    void on_clearSend_clicked();
    void on_clearRecive_clicked();
};

#endif // LOGS_H
