#ifndef DEVICE_H
#define DEVICE_H
#include <QString>

class Device
{
public:
    Device();
    void setModel(QString model);
    QString getModel();

private:
    QString model;
    QString ip;
    QString mac;
};

#endif // DEVICE_H
