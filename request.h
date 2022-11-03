// project
#include "pipe_socket.h"
#include "../proto/message.pb.h"


class Request
{
public:
    static void Int32Serialization(int32_t size, char *bytes);
    static bool Send(PipeSocket &sock, QByteArray &commonHeader, QByteArray &extendHeader);
    static bool Send(PipeSocket &sock, QByteArray &commonHeader, QByteArray &extendHeader, QByteArray &content);
    static bool Send(PipeSocket &sock, QByteArray &commonHeader, QByteArray &extendHeader, char *content, int32_t nbytes);
};


class VideoRequest : public Request
{
public:
    static bool Send(
        PipeSocket &sock, char *content, uint32_t nbytes,
        enum message::VideoHead_Codec codec = message::VideoHead_Codec_NoMansLand1,
        enum message::VideoHead_FrameType type = message::VideoHead_FrameType_NoMansLand,
        uint32_t sequence = 0, uint32_t width = 0, uint32_t height = 0, uint64_t dts = 0, uint64_t pts = 0
    );
};


class EventCloseRequest : public Request
{
public:
    static bool Send(PipeSocket &sock);
};
