#ifndef SCANNER_H
#define SCANNER_H
#include <QUdpSocket>
#include <QTimer>
#include <QQueue>
#include <QHash>


class Scanner: public QObject
{
Q_OBJECT
public:
    Scanner();
    void sendRequest();
    int counter;

private:
    QUdpSocket* udpSocket;
    QTimer *timer;
    bool timeout;
    QHash<int, QString> hashDevices;

    void initSocket();    
    QString getIp(QByteArray ba);
    QString getMac(QByteArray ba);
    QQueue<char*> dataQueue;


private slots:
    void readPendingDatagrams();
    void slotTimeout();
};

#endif // SCANNER_H
