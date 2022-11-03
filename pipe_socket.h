#pragma once

// qt
#include <QtCore/QByteArray>
#include <QtNetwork/QLocalSocket>



class PipeSocket : public QObject
{
    Q_OBJECT


public:
    PipeSocket();
    virtual ~PipeSocket();

    void SetName(QString name);

    void RegisterSignals();

    void Cancel();

    bool IsConnected();
    bool IsCanceling();

    qint64 Write(QByteArray &buffer, bool flush = true);
    qint64 Write(const char *buffer, qint64 nbytes, bool flush = true);

    qint32 ReadInt32();
    QByteArray ReadBytes(quint64 nbytes);
    //qint64 ReadBytes(char *buffer, quint64 nbytes);

    QLocalSocket *GetSocket();


public slots:
    void OnConnected();
    void OnDisconnected();
    void OnError(QLocalSocket::LocalSocketError socketError);
    void OnStateChanged(QLocalSocket::LocalSocketState socketState);




protected:
    bool m_isCanceling;
    bool m_isConnected;
    QString m_strName;
    QLocalSocket *m_pSocket;
};
