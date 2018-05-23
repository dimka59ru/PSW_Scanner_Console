#include "scanner.h"
#include "device.h"
#include <QDebug>


const char  REQUEST_FOR_SEARCH         = 0xe0; // Байт запроса на поиск
const char  RESPONSE_TO_SEARCH_REQUEST = 0xe1; // Байт ответа на поиск
const int   PORT                       = 6123; // Порт для сокета
const int   LENGTH_MESSAGE             = 444;  // Длина дейтаграммы
const int   TIMEOUT                    = 5000; // Время ожидания ответов



Scanner::Scanner()
{
    hashDevices[4] = "PSW-1G4F";
    hashDevices[6] = "PSW-2G6F+";

    this->initSocket(); // Инициализация сокета

    QTimer::singleShot(TIMEOUT, this, SLOT(slotTimeout()));
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
    Device device;

    while (udpSocket->hasPendingDatagrams() && !timeout)
    {
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        if (datagram.front() == RESPONSE_TO_SEARCH_REQUEST)
        {
            counter++;
//            dataQueue.enqueue(datagram.data());

            if (hashDevices.contains(datagram.at(1)))
                device.setModel(hashDevices[datagram.at(1)]);
            else
                device.setModel("Unknown");


            QString ip  = getIp(datagram.mid(2, 4));
            QString mac = getMac(datagram.mid(6, 6));

            qDebug().noquote().nospace() << "Device " << counter << ":";
            qDebug().noquote() << "\tModel:" << device.getModel();
            qDebug().noquote() << "\tIP:" << ip;
            qDebug().noquote() << "\tMAC" << mac;
            qDebug().noquote() << "\n";
        }
    }
}

void Scanner::slotTimeout()
{
    qDebug().noquote() << "Timeout";

    if (counter == 0)
        qDebug().noquote() << "Not found devices";

    qDebug().noquote() << "Found" << counter << "devices";
    timeout = true;
}

QString Scanner::getIp(QByteArray ba) {
    QString ip = "";
    uchar oneByte = 0;
    for (int i = 0; i < 4; i++) {
        if (i)
            ip.append('.');
        oneByte = ba.at(i);
        ip.append(QString::number(oneByte));
    }
    return ip;
}

QString Scanner::getMac(QByteArray ba) {
    QString mac = ba.toHex(':').toUpper();
    return mac;
}

