#include "server.h"
#include <iostream>
#include <QDebug>

Server::Server()
{
//    QLabel * serverState = new QLabel;

    m_pi = new QTcpServer(this);

    if (!m_pi->listen(QHostAddress::Any, 14396)) // Démarrage du serveur sur toutes les IP disponibles et sur le port 50885
    {
        // Si le serveur n'a pas été démarré correctement
        qInfo() << "Le serveur n'a pas pu être démarré. Raison :" << m_pi->errorString();
    }
    else {
        // Si le serveur a été démarré correctement
        qInfo() << "Le serveur a été démarré sur le port <strong>" + QString::number(m_pi->serverPort()) + "\nDes clients peuvent maintenant se connecter.";
        connect(m_pi, SIGNAL(newConnection()), this, SLOT(newConnection()));
    }
}

Server::~Server()
{}

void Server::newConnection()
{
    qInfo() << "Someone has connected to the server!" << endl;
    QTcpSocket *const newSocket = m_pi->nextPendingConnection();
    clients.push_back(newSocket);
    QString addr = newSocket->peerAddress().toString();

    connect(newSocket, SIGNAL(readyRead()), this, SLOT(receivedData()));
    connect(newSocket, SIGNAL(disconnected()), this, SLOT(disconnectClient()));
}

void Server::receivedData()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == nullptr) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    // Si tout va bien, on continue : on récupère le message
    QDataStream in(socket);
    quint16 code;

    if (socket->bytesAvailable() < static_cast<int>(sizeof(quint16)))  // On n'a pas reçu la taille du  message en entier
        return;

    in >> code;  // Si on a reçu la taille du message en entier, on la récupère
    if (code == 1)  // on
    {
        qDebug() << "The led has been switched on" << endl;
    }
    else {  // code = 0 = off
        qDebug() << "The led has been switched off" << endl;
    }
}

void Server::disconnectClient()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == nullptr)  // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
    {
        return;
    }

    clients.removeOne(socket);

    socket->deleteLater();
}
