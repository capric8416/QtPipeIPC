// self
#include "pipe_socket.h"

// project
#include "../logger/logger.h"


// c/c++
#include <thread>



PipeSocket::PipeSocket()
    : m_pSocket(nullptr)
    , m_isCanceling(false)
    , m_isConnected(false)
{
    LogInfo("start, this: %p\n", this);
}


PipeSocket::~PipeSocket()
{
}


void PipeSocket::SetName(QString name)
{
    m_strName = name;
}


void PipeSocket::RegisterSignals()
{
    if (!m_pSocket) {
        LogWarning("m_pSocket: %p", m_pSocket);
        return;
    }

    connect(m_pSocket, SIGNAL(connected()), this, SLOT(OnConnected()));
    connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
    connect(m_pSocket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(OnError(QLocalSocket::LocalSocketError)));
    connect(m_pSocket, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this, SLOT(OnStateChanged(QLocalSocket::LocalSocketState)));
}


void PipeSocket::Cancel()
{
    m_isCanceling = true;
}


bool PipeSocket::IsConnected()
{
    return m_isConnected;
}


bool PipeSocket::IsCanceling()
{
    return m_isCanceling;
}


qint64 PipeSocket::Write(QByteArray &buffer, bool flush)
{
    return this->Write(buffer.constData(), buffer.size(), flush);
}


qint64 PipeSocket::Write(const char *buffer, qint64 nbytes, bool flush)
{
    if (buffer == nullptr || nbytes < 0) {
        LogErrorC("invalid params, buffer: %p, nbytes: %jd\n", buffer, nbytes);
        return -1;
    }

    if (!m_pSocket || !m_isConnected) {
        LogErrorC("not connected, m_pSocket: %p, m_isConnected: %d\n", m_pSocket, m_isConnected);
        return -1;
    }

    qint64 nbytesWritten = m_pSocket->write(buffer, nbytes);
    if (flush) {
        m_pSocket->flush();
    }

    return nbytesWritten;
}


qint32 PipeSocket::ReadInt32()
{
    if (!m_pSocket || !m_isConnected) {
        LogErrorC("not connected, m_pSocket: %p, m_isConnected: %d\n", m_pSocket, m_isConnected);
        return -1;
    }

    QByteArray buffer = ReadBytes(sizeof(qint32));
    if (buffer.size() != sizeof(qint32)) {
        LogErrorC("unexpected size, nbytes: %d\n", buffer.size());
        return -1;
    }

    qint32 size = 0;
    std::memcpy(&size, buffer.constData(), sizeof(qint32));
    if (size <= 0) {
        LogErrorC("unexpected value, size: %d", size);
        return -1;
    }

    return size;
}


QByteArray PipeSocket::ReadBytes(quint64 nbytes)
{
    if (!m_pSocket || !m_isConnected) {
        LogErrorC("not connected, m_pSocket: %p, m_isConnected: %d\n", m_pSocket, m_isConnected);
        return nullptr;
    }

    return m_pSocket->read(nbytes);
}


//qint64 PipeSocket::ReadBytes(char *buffer, quint64 nbytes)
//{
//    if (!m_pSocket || !m_isConnected) {
//        TRACE("not connected, m_pSocket: %p, m_isConnected: %d\n", m_pSocket, m_isConnected);
//        return -1;
//    }
//
//    //if (!WaitBytes(nbytes)) {
//    //    TRACE("canceled, nbytes: %jd, bytesAvailable: %jd, m_isCanceling: %d\n", nbytes, m_pSocket->bytesAvailable(), m_isCanceling);
//    //    return -1;
//    //}
//
//    return m_pSocket->read(buffer, nbytes);
//}


QLocalSocket *PipeSocket::GetSocket()
{
    return m_pSocket;
}


void PipeSocket::OnConnected()
{
    LogInfo("this: %p, name: %s\n", this, m_strName.toUtf8().data());

    m_isConnected = true;
}


void PipeSocket::OnDisconnected()
{
    LogInfo("this: %p, name: %s\n", this, m_strName.toUtf8().data());

    m_isConnected = false;
}


void PipeSocket::OnError(QLocalSocket::LocalSocketError socketError)
{
    LogInfo("this: %p, name: %s, socketError: %d, msg: %s\n", this, m_strName.toUtf8().data(), socketError, m_pSocket->errorString().toUtf8().data());
}


void PipeSocket::OnStateChanged(QLocalSocket::LocalSocketState socketState)
{
    LogInfo("this: %p, name: %s, socketState: %d\n", this, m_strName.toUtf8().data(), socketState);
}
