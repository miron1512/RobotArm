#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void writeData();

    void on_pb_send_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pb_connect_clicked();

    void parseData();

    void servo1(int);
    void servo2(int);
    void servo3(int);
    void servo4(int);
    void servo5(int);
    void servo6(int);

    void pos_serv1();
    void pos_serv2();
    void pos_serv3();
    void pos_serv4();
    void pos_serv5();
    void pos_serv6();

    void on_pb_open_clicked();

signals:
    void getData();

private:
    Ui::MainWindow *ui;
    QSerialPort *sp;
    QSerialPortInfo spi;
    QString str;
    QTimer *timer;
    QByteArray arr;
    int serv1,serv2,serv3,serv4,serv5,serv6,prev_servo;
    QFile file;
    QTextStream out;
    bool bl;

    //QTimer *timerTemp;
    //QFile file;
    //QTextStream out;

    void position();

};

#endif // MAINWINDOW_H
