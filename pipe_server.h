#pragma once

// project
#include "pipe_socket.h"
#include "../server/common.h"

// qt
#include <QtNetwork/QLocalServer>

// c/c++
#include <mutex>



class PipeServer : public PipeSocket
{
    Q_OBJECT


public:
    PipeServer();
    virtual ~PipeServer();

    bool Start(QString name, int msecsTimeout = 100, int retryTimes = 30);
    bool Stop();

    bool Disconnect();

    bool startTransaction();
    bool commitTransaction();
    bool rollbackTransaction();


private slots:
    void OnNewConnection();


private:
    int m_TimeoutMilliseconds;
    int m_RetryTimes;

    std::mutex m_mutex;
    
    QLocalServer *m_pServer;
};
