#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sp = new QSerialPort(this);
    timer = new QTimer(this);
    //timerTemp = new QTimer(this);
    sp->setPortName(ui->comboBox->currentText());
    sp->setBaudRate(115200);
    sp->setParity(QSerialPort::NoParity);
    sp->setDataBits(QSerialPort::Data8);
    sp->setStopBits(QSerialPort::OneStop);
    sp->setFlowControl(QSerialPort::NoFlowControl);
    foreach (QSerialPortInfo i, spi.availablePorts()) {
        ui->comboBox->addItem(i.portName());
    }

    //connect(sp, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(this, SIGNAL(getData()), this, SLOT(writeData()));

    connect(timer,SIGNAL(timeout()),this,SLOT(parseData()));

    //connect(ui->verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(slider(int)));

    //connect(this, SIGNAL(_showData()),this,SLOT(showData()));

    //connect(timerTemp,SIGNAL(timeout()),this, SLOT(tempWrite()));
    connect(ui->verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(servo1(int)));
    connect(ui->verticalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(servo2(int)));
    connect(ui->verticalSlider_5,SIGNAL(valueChanged(int)),this,SLOT(servo3(int)));
    connect(ui->verticalSlider_6,SIGNAL(valueChanged(int)),this,SLOT(servo4(int)));
    connect(ui->verticalSlider_7,SIGNAL(valueChanged(int)),this,SLOT(servo5(int)));
    connect(ui->verticalSlider_8,SIGNAL(valueChanged(int)),this,SLOT(servo6(int)));

    connect(ui->verticalSlider,SIGNAL(sliderReleased()),this,SLOT(pos_serv1()));
    connect(ui->verticalSlider_2,SIGNAL(sliderReleased()),this,SLOT(pos_serv2()));
    connect(ui->verticalSlider_5,SIGNAL(sliderReleased()),this,SLOT(pos_serv3()));
    connect(ui->verticalSlider_6,SIGNAL(sliderReleased()),this,SLOT(pos_serv4()));
    connect(ui->verticalSlider_7,SIGNAL(sliderReleased()),this,SLOT(pos_serv5()));
    connect(ui->verticalSlider_8,SIGNAL(sliderReleased()),this,SLOT(pos_serv6()));
    timer->stop();
    //timerTemp->stop();
    str="";
    ui->label->setText("");
    bl=false;
}

MainWindow::~MainWindow()
{
    file.close();
    delete ui;
}

void MainWindow::writeData()
{
    //str="";


//    QMessageBox ms;
//    ms.setText(arr);
//    ms.exec();

    sp->write(arr);
    parseData();
}

void MainWindow::on_pb_send_clicked()
{
    if(ui->lineEdit->text().length()>0)
    {
        arr=ui->lineEdit->text().toLocal8Bit()+'\r';
        ui->textEdit->append(ui->lineEdit->text());
        emit getData();

        // # 5 P500 # 6 P800 # 4 P1000 # 3 P1200 # 2 P1500
        // # 5 P2500 T1000   (ms)
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->rb_left->isChecked())
    {
        str+=ui->textEdit->toPlainText();
    }
    if(ui->rb_right->isChecked())
    {
        str+=ui->textEdit_2->toPlainText();
    }
    if(!ui->rb_right->isChecked() && !ui->rb_left->isChecked())
    {
        QMessageBox ms;
        ms.setText("Не выбрано");
        ms.exec();
        return;
    }

    ui->label->setText("<b><font color=red>Выполнение</font></b>");

    arr="";

    parseData();
//    if(ui->textEdit->toPlainText().length()>0)
//        foreach (QString s, ui->textEdit->toPlainText().split('\n')) {
//            arr=s.toLocal8Bit();
//            if(arr[arr.length()-1]!='\r')
//                arr+="S5000\r";
//            QMessageBox ms;

//                ms.setText(s);
//                ms.exec();
//            emit getData();
//        }


}

void MainWindow::on_pushButton_2_clicked()
{
    arr="#1P1500T1000#2P1500T1000#3P1500T1000#4P1500T1000#5P9800T1000#6P1500T1000\r";
    ui->verticalSlider->setValue(1500);
    ui->verticalSlider_2->setValue(1500);
    ui->verticalSlider_5->setValue(1500);
    ui->verticalSlider_6->setValue(1500);
    ui->verticalSlider_7->setValue(980);
    ui->verticalSlider_8->setValue(1500);

    bl=true;

    emit getData();
}

void MainWindow::on_pb_connect_clicked()
{
    if(ui->pb_connect->text()=="Disconnect")
    {
        ui->pb_connect->setText("Connect");
        sp->close();
        foreach (QSerialPortInfo i, spi.availablePorts()) {
            ui->comboBox->addItem(i.portName());
        }
    }
    else
    {
        sp->setPortName(ui->comboBox->currentText());
        //QMessageBox ms;
        if(sp->open(QIODevice::ReadWrite))
        {
            //QString str=sp->readAll();

            //QMessageBox ms;

            //ms.setText("YES");
            //ms.exec();

            ui->pb_connect->setText("Disconnect");
    /*
    //        file.setFileName("log.txt");
    //        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    //        {
    //            ms.setText("File not opened");
    //            ms.exec();
    //        }
    //        out.setDevice(&file);
    //        timerTemp->start(30000);

    //        arr="q";
            //emit getData();

            //timer->start(2000);
            //showData("q");

    //        ui->textEdit->append(str);
    //        str="";
    */
        }
        else {
            //ms.setText("NO");
            //ms.exec();
            foreach (QSerialPortInfo i, spi.availablePorts()) {
                ui->comboBox->addItem(i.portName());
            }
        }
    }

}
void MainWindow::servo1(int value)
{

    arr="#1P"+QString::number(value).toLocal8Bit()+"T1000\r";
    emit getData();
}
void MainWindow::servo2(int value)
{

    arr="#2P"+QString::number(value).toLocal8Bit()+"T1000\r";
    emit getData();
}
void MainWindow::servo3(int value)
{
    arr="#3P"+QString::number(value).toLocal8Bit()+"T1000\r";
    emit getData();
}
void MainWindow::servo4(int value)
{
    arr="#4P"+QString::number(value).toLocal8Bit()+"T1000\r";
    emit getData();
}
void MainWindow::servo5(int value)
{
    arr="#5P"+QString::number(value).toLocal8Bit()+"T1000\r";
    emit getData();
}
void MainWindow::servo6(int value)
{
    arr="#6P"+QString::number(value).toLocal8Bit()+"T1000\r";
    emit getData();
}

void MainWindow::parseData()
{
    if(bl)
    {
        bl=false;
        return;
    }
    timer->stop();

    disconnect(ui->verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(servo1(int)));
    disconnect(ui->verticalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(servo2(int)));
    disconnect(ui->verticalSlider_5,SIGNAL(valueChanged(int)),this,SLOT(servo3(int)));
    disconnect(ui->verticalSlider_6,SIGNAL(valueChanged(int)),this,SLOT(servo4(int)));
    disconnect(ui->verticalSlider_7,SIGNAL(valueChanged(int)),this,SLOT(servo5(int)));
    disconnect(ui->verticalSlider_8,SIGNAL(valueChanged(int)),this,SLOT(servo6(int)));

    if(arr!="")
    {
        foreach (QString s, QString::fromLocal8Bit(arr).trimmed().split('#')) {
            int serv=0;
            if(s!="")
            {
                serv=s[0].digitValue();
                s.remove(0,2);
                if(s.indexOf('T')!=-1)
                    s=s.left(s.indexOf('T'));
                switch (serv) {
                case 1:
                    ui->sb_serv1->setValue(s.toInt());
                    break;
                case 2:
                    ui->sb_serv2->setValue(s.toInt());
                    break;
                case 3:
                    ui->sb_serv3->setValue(s.toInt());
                    break;
                case 4:
                    ui->sb_serv4->setValue(s.toInt());
                    break;
                case 5:
                    ui->sb_serv5->setValue(s.toInt());
                    break;
                case 6:
                    ui->sb_serv6->setValue(s.toInt());
                    break;
                }
            }
        }
    }

    str=str.trimmed();
    if(!str.isEmpty())
    {
        QString temp_str;

        int index=str.indexOf('\n');
        if(index==-1)
        {
            temp_str=str;
            str="";
        }
        else {
            temp_str=str.left(index);
            str.remove(0,str.indexOf('\n'));
        }
        if(temp_str[1]=='D')
        {
            temp_str.remove(0,2);
            timer->start(temp_str.toInt());
        }
        else {
            arr=temp_str.toLocal8Bit();
            if(arr[arr.length()-1]=='\n'||arr[arr.length()-1]=='\r')
                arr[arr.length()-1]='\r';
            else {
                arr+='\r';
            }
                //arr+="S5000\r";

            emit getData();
        }
    }
    else
    {
        if(ui->label->text()=="<b><font color=red>Выполнение</font></b>")
            ui->label->setText("<b><font color=green>Готово</font></b>");

        connect(ui->verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(servo1(int)));
        connect(ui->verticalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(servo2(int)));
        connect(ui->verticalSlider_5,SIGNAL(valueChanged(int)),this,SLOT(servo3(int)));
        connect(ui->verticalSlider_6,SIGNAL(valueChanged(int)),this,SLOT(servo4(int)));
        connect(ui->verticalSlider_7,SIGNAL(valueChanged(int)),this,SLOT(servo5(int)));
        connect(ui->verticalSlider_8,SIGNAL(valueChanged(int)),this,SLOT(servo6(int)));

        if(ui->cb_repeat->isChecked())
        {
//            bl=true;
            on_pushButton_2_clicked();
//            QMessageBox ms;
//            ms.setText("GOOD");
//            ms.exec();
            str="#D1500\n";
            on_pushButton_clicked();
        }
    }

}

void MainWindow::pos_serv1()
{
    str="#1P"+QString::number(ui->sb_serv1->value())+"T1000";
    ui->textEdit_5->append(str);
    ui->textEdit_5->append("#D1000");
}
void MainWindow::pos_serv2()
{
    str="#2P"+QString::number(ui->sb_serv2->value())+"T1000";
    ui->textEdit_5->append(str);
    ui->textEdit_5->append("#D1000");
}
void MainWindow::pos_serv3()
{
    str="#3P"+QString::number(ui->sb_serv3->value())+"T1000";
    ui->textEdit_5->append(str);
    ui->textEdit_5->append("#D1000");
}
void MainWindow::pos_serv4()
{
    str="#4P"+QString::number(ui->sb_serv4->value())+"T1000";
    ui->textEdit_5->append(str);
    ui->textEdit_5->append("#D1000");
}
void MainWindow::pos_serv5()
{
    str="#5P"+QString::number(ui->sb_serv5->value())+"T1000";
    ui->textEdit_5->append(str);
    ui->textEdit_5->append("#D1000");
}
void MainWindow::pos_serv6()
{
    str="#6P"+QString::number(ui->sb_serv6->value())+"T1000";
    ui->textEdit_5->append(str);
    ui->textEdit_5->append("#D1000");
}

void MainWindow::on_pb_open_clicked()
{

    QString file_name = QFileDialog::getOpenFileName(this,"Открыть",QDir::currentPath());
    if(file_name.isEmpty())
        return;
    else
    {
        if(file.isOpen())
            file.close();
        file.setFileName(file_name);
        if(!file.open(QIODevice::ReadWrite))
        {
            QMessageBox ms;
            ms.setText("File not opened");
            ms.exec();
        }
        out.setDevice(&file);

        if(!ui->textEdit_2->toPlainText().isEmpty())
        {
            if(QMessageBox::question(this, "Удалить", "Удалить?")==QMessageBox::Yes)
            {
                ui->textEdit_2->clear();
                ui->textEdit_2->setText(out.readAll());
            }
        }
        else
            ui->textEdit_2->setText(out.readAll());
    }
}
