#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>

class Server : public QObject
{
    Q_OBJECT
public:
    Server();
    ~Server();

private slots:
    void newConnection();
    void receivedData();
    void disconnectClient();

private:
    QTcpServer * m_pi;
    QList<QTcpSocket *> clients;
};

#endif // SERVER_H
