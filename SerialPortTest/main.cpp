#include "mainwindow.h"
#include <QApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <locale.h>
#include <cmath>
#include <cstdlib>

QSerialPort     SPort1;
QByteArray      SArray1;
int counter = 0;
void UartConfig(void);

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    UartConfig();
    QString filename = "Data.txt";
    QTime TimeObj;
    QFile DataFile(filename);
    /* Remove File if it already exists */
    if (DataFile.exists()) DataFile.remove();
    if (DataFile.open(QIODevice::ReadWrite)){
        QObject::connect(&SPort1, &QSerialPort::readyRead,[&]{
        /* This is called when readyRead() is emitted */
            QTextStream stream(&DataFile);
            qDebug() << "New data available: " << SPort1.bytesAvailable();
            QByteArray datas = SPort1.readAll();
            stream<<datas;
            counter++;
            if(counter==10000)    DataFile.close();
        });
    }

    QObject::connect(&SPort1, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>
    (&QSerialPort::error),[&](QSerialPort::SerialPortError error){
        /* This is called when a serial communication error occurs */
        qDebug() << "An error occured: " << error;
        a.quit();
    });
    return a.exec();
}

void UartConfig(void){
    SPort1.setPortName("ttyUSB0");
    SPort1.setBaudRate(QSerialPort::Baud9600);
    SPort1.setDataBits(QSerialPort::Data8);
    SPort1.setParity(QSerialPort::NoParity);
    SPort1.setStopBits(QSerialPort::OneStop);
    SPort1.setFlowControl(QSerialPort::NoFlowControl);
    if(!SPort1.open(QIODevice::ReadOnly))
        qDebug() << SPort1.errorString();
    /*SPort1.write("Connection Established! \n");*/
}
void UartDeConfig(void){
    SPort1.close();
}
