#ifndef DEVICE_H
#define DEVICE_H
#include <QString>

class Device
{
public:
//    Device(QString model);
    void setModel(QString model);
    void setIp(QByteArray ba);
    void setMac(QByteArray ba);

//    QString getModel();
//    QString getIP();

    void print();

    static int sum;

private:
    QString model;
    QString ip;
    QString mac;



};

#endif // DEVICE_H
