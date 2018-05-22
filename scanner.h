#ifndef SCANNER_H
#define SCANNER_H
#include <QUdpSocket>
#include <QTimer>

class Scanner: public QObject
{
Q_OBJECT
public:
    Scanner();
    void sendRequest();


private:
    QUdpSocket* udpSocket;
    QTimer *timer;
    bool reading;

    void initSocket();    
    QString getIp(QByteArray ba);
    QString getMac(QByteArray ba);

private slots:
    void readPendingDatagrams();
    void slotTimeout();
};

#endif // SCANNER_H
