#include "scanner.h"
#include <QDebug>
#include <QNetworkDatagram>
#include <typeinfo>

const char REQUEST_FOR_SEARCH          = 0xe0; // Байт запроса на поиск
const int  PORT                        = 6123; // Порт для сокета
const int  LENGTH_MESSAGE              = 444;  // Длина дейтаграммы



Scanner::Scanner()
{
    reading = false;
    this->initSocket(); // Инициализация сокета
    this->sendRequest(); // Отправка в сеть запроса на поиск
}


void Scanner::initSocket(){

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(PORT);

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));

    QTimer::singleShot(2000, this, SLOT(slotTimeout()));

}

void Scanner::sendRequest()
{
    QByteArray datagram;
    datagram.fill(0, LENGTH_MESSAGE);
    datagram[0] = REQUEST_FOR_SEARCH;

    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, PORT);
    reading = true;

    qDebug() << "The datagram was sent";
}


void Scanner::readPendingDatagrams()
{
    QByteArray datagram;
    QHostAddress senderAddress;
    quint16 senderPort;

    while (udpSocket->hasPendingDatagrams() && reading) {
        //        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
        qDebug() << "Received a response from " << QHostAddress(senderAddress.toIPv4Address()).toString();

        QString ip  = getIp(datagram.mid(2, 4));
        QString mac = getMac(datagram.mid(6,6));

        qDebug() << "\t" << ip;
        qDebug() << "\t" << mac;
        qDebug() << "\n";
    }


}

void Scanner::slotTimeout()
{
    qDebug() << "Timeout";
    reading = false;
}

QString Scanner::getIp(QByteArray ba) {
    QString ip;
    for (int i = 0; i < 4; i++) {
        if (i)
            ip.append('.');
        ip.append(QString::number((uchar)ba.at(i)));
    }
    return ip;
}

QString Scanner::getMac(QByteArray ba) {
    QString mac = ba.toHex(':').toUpper();
    return mac;
}
