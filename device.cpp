#include "device.h"


Device::Device()
{

}

void Device::setModel(QString model)
{
    this->model = model;
}

QString Device::getModel()
{
    return this->model;
}
