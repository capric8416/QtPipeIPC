// self
#include "pipe_server.h"

// project
#include "../logger/logger.h"


PipeServer::PipeServer()
    : PipeSocket()
    , m_pServer(nullptr)
    , m_TimeoutMilliseconds(0)
    , m_RetryTimes(0)
{
    INFO("start, this: %p\n", this);
}


PipeServer::~PipeServer()
{
    INFO("stopping, this: %p\n", this);

    Stop();

    INFO("stopped, this: %p\n", this);
}


bool PipeServer::Start(QString name, int msecsTimeout, int retryTimes)
{
    INFO("locking\n");
    std::lock_guard<std::mutex> lock(m_mutex);
    INFO("locked\n");

    m_TimeoutMilliseconds = msecsTimeout;
    m_RetryTimes = retryTimes;

    if (m_pServer != nullptr) {
        WARNING("no need to start, m_pServer: %p\n", m_pServer);
        return true;
    }

    m_pServer = new QLocalServer(this);
    if (!m_pServer) {
        FATAL("new object error, m_pServer: %p\n", m_pServer);
        return false;
    }

    //m_pServer->setMaxPendingConnections(1);

    connect(m_pServer, SIGNAL(newConnection()), this, SLOT(OnNewConnection()));

    if (!m_pServer->listen(name)) {
        FATAL("listen error, name: %p\n", name.toUtf8().data());
        return false;
    }

    SetName(name);

    INFO("start pipe server, name: %s\n", name.toUtf8().data());

    return true;
}


bool PipeServer::Stop()
{
    INFO("locking\n");
    std::lock_guard<std::mutex> lock(m_mutex);
    INFO("locked\n");

    if (m_pServer != nullptr) {
        INFO("stop server\n");

        m_pServer->close();
        m_pServer = nullptr;

        return true;
    }

    return false;
}


bool PipeServer::Disconnect()
{
    INFO("locking\n");
    std::lock_guard<std::mutex> lock(m_mutex);
    INFO("locked\n");

    if (!m_pServer) {
        WARNING("not started, m_pServer: %p\n", m_pServer);
        return false;
    }

    if (m_isConnected && m_pSocket) {
        INFO("disconnect, m_pSocket: %p\n", m_pSocket);

        m_pSocket->disconnectFromServer();
        m_isConnected = false;
        m_pSocket = nullptr;

        return true;
    }

    return false;
}


bool PipeServer::startTransaction()
{
    if (!m_pServer || !m_pSocket || !m_isConnected) {
        WARNING("not prepared, m_pServer: %p, m_pSocket: %p, m_isConnected: %d\n", m_pServer, m_pSocket, m_isConnected);
        return false;
    }

    m_pSocket->startTransaction();

    return m_pSocket->isTransactionStarted();
}


bool PipeServer::commitTransaction()
{
    if (!m_pServer || !m_pSocket || !m_isConnected) {
        WARNING("not prepared, m_pServer: %p, m_pSocket: %p, m_isConnected: %d\n", m_pServer, m_pSocket, m_isConnected);
        return false;
    }

    m_pSocket->commitTransaction();

    return true;
}


bool PipeServer::rollbackTransaction()
{
    if (!m_pServer || !m_pSocket || !m_isConnected) {
        WARNING("not prepared, m_pServer: %p, m_pSocket: %p, m_isConnected: %d\n", m_pServer, m_pSocket, m_isConnected);
        return false;
    }

    m_pSocket->rollbackTransaction();

    return true;
}


void PipeServer::OnNewConnection()
{
    INFO("locking\n");
    std::lock_guard<std::mutex> lock(m_mutex);
    INFO("locked\n");

    if (m_pSocket) {
        WARNING("limit to 1 client connection\n");
        return;
    }

    m_pSocket = m_pServer->nextPendingConnection();
    if (!m_pSocket) {
        CRITICAL("accept connection fail\n");
        return;
    }

    // 注册状态及错误信息
    RegisterSignals();

    m_isConnected = true;

    INFO("cannected, m_pSocket: %p\n", m_pSocket);
   
    //int retryTimes = m_RetryTimes;
    //while (!m_isCanceling && retryTimes-- > 0) {
    //    if (m_pSocket->waitForReadyRead(m_TimeoutMilliseconds)) {
    //        m_isConnected = true;
    //        break;
    //    }
    //}

    //if (m_isConnected) {
    //    if (m_isCanceling) {
    //        TRACE("cancel connection\n");

    //        m_pSocket->disconnectFromServer();
    //        m_isConnected = false;
    //        m_pSocket = nullptr;
    //    }
    //    else {
    //        TRACE("cannected, m_pSocket: %p\n", m_pSocket);
    //    }
    //}
    //else if (!m_isCanceling) {
    //    TRACE("connect fail, error: \n", m_pSocket->errorString().toUtf8().data());
    //}
}
