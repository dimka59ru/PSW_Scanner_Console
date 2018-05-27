#include "scanner.h"
#include "device.h"
#include <QDebug>
#include <QTextStream>
#include <QCoreApplication>

const char  REQUEST_FOR_SEARCH         = 0xe0; // Байт запроса на поиск
const char  RESPONSE_TO_SEARCH_REQUEST = 0xe1; // Байт ответа на поиск
const int   PORT                       = 6123; // Порт для сокета
const int   LENGTH_MESSAGE             = 444;  // Длина дейтаграммы
const int   TIMEOUT                    = 5000; // Время ожидания ответов

int Device::sum = 0;

Scanner::Scanner()
{
    hashDevices[4] = "PSW-1G4F";
    hashDevices[6] = "PSW-2G6F+";

    this->initSocket(); // Инициализация сокета

    QTimer::singleShot(TIMEOUT, this, SLOT(slotTimeout()));
    //    QTimer *timer = new QTimer(this);
    //    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
    //    timer->start();

    timeout = true;

    counter = 0;
}


void Scanner::initSocket(){

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(PORT);

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));

}

void Scanner::sendRequest()
{
    QByteArray datagram;
    datagram.fill(0, LENGTH_MESSAGE);
    datagram[0] = REQUEST_FOR_SEARCH;

    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, PORT);
    timeout = false;

    qDebug().noquote() << "The datagram was sent\n";
    qDebug().noquote().nospace() << "Found devices:";

}


void Scanner::readPendingDatagrams()
{
    QByteArray datagram;

    while (udpSocket->hasPendingDatagrams() && !timeout)
    {
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        if (datagram[0] == RESPONSE_TO_SEARCH_REQUEST)
        {
            Device device;
            device.sum++;
            //            counter++;
            //            dataQueue.enqueue(datagram.data());

            if (hashDevices.contains(datagram.at(1)))
                device.setModel(hashDevices[datagram.at(1)]);
            else
                device.setModel("Unknown");

            device.setIp(datagram.mid(2, 4));
            device.setMac(datagram.mid(6, 6));
            //            QString ip  = getIp(datagram.mid(2, 4));
            //            QString mac = getMac(datagram.mid(6, 6));

            device.print();
        }
    }
}

void Scanner::slotTimeout()
{
    qDebug().noquote() << "Timeout";
    qDebug().noquote() << "Number of devices:" << Device::sum;
    timeout = true;

    qDebug().noquote() << "Search again? (Y\\N)";
    QTextStream s(stdin);
    QString value = s.readLine();
    if (value == "Y" || value == "y")
    {
        Device::sum = 0;
        timeout = false;
        sendRequest();
        QTimer::singleShot(TIMEOUT, this, SLOT(slotTimeout()));
    }
    else
    {
        qApp->quit();
    }

}

//QString Scanner::getIp(QByteArray ba) {
//    QString ip = "";
//    uchar oneByte = 0;
//    for (int i = 0; i < 4; i++) {
//        if (i)
//            ip.append('.');
//        oneByte = ba.at(i);
//        ip.append(QString::number(oneByte));
//    }
//    return ip;
//}

//QString Scanner::getMac(QByteArray ba) {
//    QString mac = ba.toHex(':').toUpper();
//    return mac;
//}

