// self
#include "request.h"

// project
#include "../logger/logger.h"



void Request::Int32Serialization(int32_t size, char *bytes)
{
	// size 序列化成 byte array
	char pBufferSize[sizeof(size)];
	std::copy(static_cast<const char *>(static_cast<const void *>(&size)),
		static_cast<const char *>(static_cast<const void *>(&size)) + sizeof(size),
		bytes);
}


// bool Request::Send(PipeSocket &sock, QByteArray &commonHeader, QByteArray &content)
// {
//     // size 序列化成 byte array
//     int32_t nbytesCommonHeader = commonHeader.size();
//     char pBufferSize[sizeof(nbytesCommonHeader)];
//     Int32Serialization(nbytesCommonHeader, pBufferSize);

//     // 写 size
//     qint64 nbytesWrittenSize = sock.Write(pBufferSize, sizeof(nbytesCommonHeader), false);
//     if (nbytesWrittenSize < sizeof(nbytesCommonHeader)) {
//         TRACE("nbytesWrittenSize < sizeof(nbytesCommonHeader), %jd, %d\n", nbytesWrittenSize, sizeof(nbytesCommonHeader));
//         return false;
//     }

//     // 写 common header
//     qint64 nbytesWrittenCommonHeader = sock.Write(commonHeader, false);
//     if (nbytesWrittenCommonHeader < commonHeader.size()) {
//         TRACE("nbytesWrittenCommonHeader < commonHeader.size(), %jd, %d\n", nbytesWrittenCommonHeader, commonHeader.size());
//         return false;
//     }

//     // 写 content
//     qint64 nbytesWrittenContent = sock.Write(content, true);
//     if (nbytesWrittenContent < content.size()) {
//         TRACE("nbytesWrittenContent < content.size(), %jd, %d\n", nbytesWrittenContent, content.size());
//         return false;
//     }

//     return true;
// }


bool Request::Send(PipeSocket &sock, QByteArray &commonHeader, QByteArray &extendHeader)
{
	// size 序列化成 byte array
	int32_t nbytesCommonHeader = commonHeader.size();
	char pBufferSize[sizeof(nbytesCommonHeader)];
	Int32Serialization(nbytesCommonHeader, pBufferSize);

	// 写 size
	qint64 nbytesWrittenSize = sock.Write(pBufferSize, sizeof(nbytesCommonHeader), false);
	if (nbytesWrittenSize < sizeof(nbytesCommonHeader)) {
		LogWarning("nbytesWrittenSize < sizeof(nbytesCommonHeader), %jd, %d\n", nbytesWrittenSize, sizeof(nbytesCommonHeader));
		return false;
	}

	// 写 common header
	qint64 nbytesWrittenCommonHeader = sock.Write(commonHeader, false);
	if (nbytesWrittenCommonHeader < commonHeader.size()) {
		LogWarning("nbytesWrittenCommonHeader < commonHeader.size(), %jd, %d\n", nbytesWrittenCommonHeader, commonHeader.size());
		return false;
	}

	// 写 extend header
	qint64 nbytesWrittenVideoHeader = sock.Write(extendHeader, true);
	if (nbytesWrittenVideoHeader < extendHeader.size()) {
		LogWarning("nbytesWrittenVideoHeader < extendHeader.size(), %jd, %d\n", nbytesWrittenVideoHeader, extendHeader.size());
		return false;
	}

	return true;
}


bool Request::Send(PipeSocket &sock, QByteArray &commonHeader, QByteArray &extendHeader, QByteArray &content)
{
	// size 序列化成 byte array
	int32_t nbytesCommonHeader = commonHeader.size();
	char pBufferSize[sizeof(nbytesCommonHeader)];
	Int32Serialization(nbytesCommonHeader, pBufferSize);

	// 写 size
	qint64 nbytesWrittenSize = sock.Write(pBufferSize, sizeof(nbytesCommonHeader), false);
	if (nbytesWrittenSize < sizeof(nbytesCommonHeader)) {
		LogWarning("nbytesWrittenSize < sizeof(nbytesCommonHeader), %jd, %d\n", nbytesWrittenSize, sizeof(nbytesCommonHeader));
		return false;
	}

	// 写 common header
	qint64 nbytesWrittenCommonHeader = sock.Write(commonHeader, false);
	if (nbytesWrittenCommonHeader < commonHeader.size()) {
		LogWarning("nbytesWrittenCommonHeader < commonHeader.size(), %jd, %d\n", nbytesWrittenCommonHeader, commonHeader.size());
		return false;
	}

	// 写 extend header
	qint64 nbytesWrittenVideoHeader = sock.Write(extendHeader, false);
	if (nbytesWrittenVideoHeader < extendHeader.size()) {
		LogWarning("nbytesWrittenVideoHeader < extendHeader.size(), %jd, %d\n", nbytesWrittenVideoHeader, extendHeader.size());
		return false;
	}

	// 写 content
	qint64 nbytesWrittenContent = sock.Write(content, true);
	if (nbytesWrittenContent < content.size()) {
		LogWarning("nbytesWrittenContent < content.size(), %jd, %d\n", nbytesWrittenContent, content.size());
		return false;
	}

	return true;
}


bool Request::Send(PipeSocket &sock, QByteArray &commonHeader, QByteArray &extendHeader, char *content, int32_t nbytes)
{
	// size 序列化成 byte array
	int32_t nbytesCommonHeader = commonHeader.size();
	char pBufferSize[sizeof(nbytesCommonHeader)];
	Int32Serialization(nbytesCommonHeader, pBufferSize);

	// 写 size
	qint64 nbytesWrittenSize = sock.Write(pBufferSize, sizeof(nbytesCommonHeader), false);
	if (nbytesWrittenSize < sizeof(nbytesCommonHeader)) {
		LogWarning("nbytesWrittenSize < sizeof(nbytesCommonHeader), %jd, %d\n", nbytesWrittenSize, sizeof(nbytesCommonHeader));
		return false;
	}

	// 写 common header
	qint64 nbytesWrittenCommonHeader = sock.Write(commonHeader, false);
	if (nbytesWrittenCommonHeader < commonHeader.size()) {
		LogWarning("nbytesWrittenCommonHeader < commonHeader.size(), %jd, %d\n", nbytesWrittenCommonHeader, commonHeader.size());
		return false;
	}

	// 写 extend header
	qint64 nbytesWrittenVideoHeader = sock.Write(extendHeader, false);
	if (nbytesWrittenVideoHeader < extendHeader.size()) {
		LogWarning("nbytesWrittenVideoHeader < extendHeader.size(), %jd, %d\n", nbytesWrittenVideoHeader, extendHeader.size());
		return false;
	}

	// 写 content
	qint64 nbytesWrittenContent = sock.Write(content, nbytes, true);
	if (nbytesWrittenContent < nbytes) {
		LogWarning("nbytesWrittenContent < content.size(), %jd, %d\n", nbytesWrittenContent, nbytes);
		return false;
	}

	return true;
}


bool VideoRequest::Send(
	PipeSocket &sock, char *content, uint32_t nbytes,
	enum message::VideoHead_Codec codec, enum message::VideoHead_FrameType type,
	uint32_t sequence, uint32_t width, uint32_t height, uint64_t dts, uint64_t pts
)
{
	// 扩展消息头
	message::VideoHead videoHeader;
	videoHeader.set_next_size(nbytes);
	videoHeader.set_partial(false);  // 如果这是一个完整的I/P/B帧，将此字段设置为false
	videoHeader.set_codec(codec);  // 可选：编码类型
	videoHeader.set_type(type);  // 可选： 帧类型
	videoHeader.set_sequence(sequence);  // 可选：帧序号
	videoHeader.set_width(width);  // 可选：图像宽
	videoHeader.set_height(height);  // 可选：图像高
	videoHeader.set_dts(dts);  // 可选：送解码时间
	videoHeader.set_pts(pts);  // 可选： 送显示时间

	// video header size
	int32_t nbytesVideoHeader = videoHeader.ByteSize();

	// 基础消息头
	message::CommonHead commonheader;
	// 设置 消息类型
	commonheader.set_type(message::CommonHead_Type_Video);
	// 设置 是否有扩展头部
	commonheader.set_extend(true);
	// 设置 紧接下来的块大小 (如果没有扩展头部，那这里就是正文大小；如果有扩展头部，那就是扩展头部大小，扩展头部里面再设置正文大小)
	commonheader.set_next_size(nbytesVideoHeader);

	// common header size
	int32_t nbytesCommonHeader = commonheader.ByteSize();

	// common header 序列化成 byte array
	QByteArray pBufferCommonHeader(nbytesCommonHeader, 0);
	if (!commonheader.SerializeToArray(pBufferCommonHeader.data(), nbytesCommonHeader)) {
		LogWarning("serilize common header fail\n");
		return false;
	}

	// video header 序列化成 byte array
	QByteArray pBufferVideoHeader(nbytesVideoHeader, 0);
	if (!videoHeader.SerializeToArray(pBufferVideoHeader.data(), nbytesVideoHeader)) {
		LogWarning("serilize video header fail\n");
		return false;
	}

	// content 无需序列化

	if (!Request::Send(sock, pBufferCommonHeader, pBufferVideoHeader, content, nbytes)) {
		LogWarning("send video fail\n");
		return false;
	}

	return true;
}


bool EventCloseRequest::Send(PipeSocket &sock)
{
	// 扩展的事件消息头
	message::EventHead eventHeader;
	eventHeader.set_next_size(0);  // 没有正文
	eventHeader.set_type(message::EventHead_Type_Close);  // 通知播放器关闭

	// event header size
	int32_t nbytesEventHeader = eventHeader.ByteSize();

	// 基础消息头
	message::CommonHead commonHeader;
	commonHeader.set_type(message::CommonHead_Type_Event);
	commonHeader.set_extend(true);
	commonHeader.set_next_size(eventHeader.ByteSize());

	// common header 序列化成 byte array
	QByteArray pBufferCommonHeader(commonHeader.ByteSize(), 0);
	if (!commonHeader.SerializeToArray(pBufferCommonHeader.data(), commonHeader.ByteSize())) {
		LogWarning("serilize common header fail\n");
		return false;
	}

	// event header 序列化成 byte array
	QByteArray pBufferEventHeader(nbytesEventHeader, 0);
	if (!eventHeader.SerializeToArray(pBufferEventHeader.data(), nbytesEventHeader)) {
		LogWarning("serilize event header fail\n");
		return false;
	}

	// 没有正文

	// 发送
	return Request::Send(sock, pBufferCommonHeader, pBufferEventHeader);
}
