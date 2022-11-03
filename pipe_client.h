#pragma once

// project
#include "pipe_socket.h"
#include "../client/common.h"

// c/c++
#include <mutex>



class PipeClient : public PipeSocket
{
    Q_OBJECT


public:
    PipeClient();
    virtual ~PipeClient();

    bool Start(QString name, int msecsTimeout = 100, int retryTimes = 30);
    bool Stop();

    bool Disconnect();


private:
    std::mutex m_mutex;
};
