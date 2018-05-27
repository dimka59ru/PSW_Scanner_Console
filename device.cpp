#include "device.h"
#include <QDebug>


//Device::Device(QString model)
//{
//    this->model = model;
//}

void Device::setModel(QString model)
{
    this->model = model;
}

void Device::setIp(QByteArray ba) {
//    QString ip = "";
    uchar oneByte = 0;
    for (int i = 0; i < 4; i++) {
        if (i)
            this->ip.append('.');
        oneByte = ba.at(i);
        this->ip.append(QString::number(oneByte));
    }

}

void Device::setMac(QByteArray ba) {
    this->mac = ba.toHex(':').toUpper();
}

//QString Device::getModel()
//{
//    return this->model;
//}

void Device::print()
{
    qDebug().noquote() << "\tModel:" << model;
    qDebug().noquote() << "\tIP:"    << ip;
    qDebug().noquote() << "\tMAC:"   << mac;
    qDebug().noquote() << "\n";
}
