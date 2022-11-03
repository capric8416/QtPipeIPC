// self
#include "pipe_client.h"
#include "../logger/logger.h"


PipeClient::PipeClient()
    : PipeSocket()
{
    LogInfoC("start, this: %p\n", this);
}


PipeClient::~PipeClient()
{
    LogInfoC("stopping, this: %p\n", this);

    Stop();

    LogInfoC("stopped, this: %p\n", this);
}


bool PipeClient::Start(QString name, int msecsTimeout, int retryTimes)
{
    LogInfoC("locking\n");
    std::lock_guard<std::mutex> lock(m_mutex);
    LogInfoC("locked\n");

    m_pSocket = new QLocalSocket(this);
    if (!m_pSocket) {
        LogInfoC("new object error, m_pSocket: %p\n", m_pSocket);
        return false;
    }

    SetName(name);

    // 注册状态及错误信息
    RegisterSignals();

    // 尝试去连接
    int retries = 0;
    while (!m_isCanceling && m_pSocket->state() == QLocalSocket::UnconnectedState && retries++ < retryTimes) {
        m_pSocket->connectToServer(name, QIODevice::ReadWrite);
        std::this_thread::sleep_for(std::chrono::milliseconds(msecsTimeout));
    }

    // 等待连接成功
    retries = 0;
    while (!m_isCanceling && retries++ < retryTimes) {
        if (m_pSocket->waitForConnected(msecsTimeout)) {
            m_isConnected = true;
            break;
        }
    }

    if (m_isConnected) {
        if (m_isCanceling) {
            LogInfoC("cancel connection\n");
            m_pSocket->disconnectFromServer();
            m_isConnected = false;
        }
        else {
            LogInfoC("connect to pipe server, name: %s\n", name.toUtf8().data());
        }
    }

    return m_isConnected;
}


bool PipeClient::Stop()
{
    LogInfoC("locking\n");
    std::lock_guard<std::mutex> lock(m_mutex);
    LogInfoC("locked\n");

    if (m_pSocket != nullptr) {
        LogInfoC("stop client\n");

        m_pSocket->close();
        m_pSocket = nullptr;

        return true;
    }

    return false;
}


bool PipeClient::Disconnect()
{
    LogInfoC("locking\n");
    std::lock_guard<std::mutex> lock(m_mutex);
    LogInfoC("locked\n");

    if (!m_pSocket) {
        LogWarningC("not started, m_pSocket: %p\n", m_pSocket);
        return false;
    }

    if (m_isConnected && m_pSocket) {
        LogWarningC("disconnect, m_pSocket: %p\n", m_pSocket);

        m_pSocket->disconnectFromServer();
        m_isConnected = false;
        m_pSocket = nullptr;

        return true;
    }

    return false;
}
