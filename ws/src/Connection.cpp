#include "Connection.h"
#include "wsServer.h"
#include <vector>

SNSBEGIN

SvrConnection::SvrConnection(lws *socket, IConnGroup* pGroup, int id)
    : socket{ socket }
    , m_msgId(0)
    , m_id(id)
    , m_pGroup(pGroup)
{
}

SvrConnection::~SvrConnection()
{
}

int SvrConnection::send(const std::string &text, bool bBinary)
{
    if (!this->socket)
    {
        return -1;
    }
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    if (sendingBuf.empty())
        lws_callback_on_writable(socket);
    MsgData msgData = { text, bBinary, genMsgId() };
    sendingBuf.push_back(msgData);
    return msgData.msgId;
}

int SvrConnection::isValid() const
{
    return !!this->socket;
}

void SvrConnection::sendBuf()
{
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    while (!sendingBuf.empty())
    {
        MsgData msgData = sendingBuf.front();
        sendingBuf.pop_front();
        std::vector<unsigned char> buf;
        buf.resize(msgData.buf.length() + LWS_PRE);
        memcpy(buf.data() + LWS_PRE, msgData.buf.data(), msgData.buf.length());
        if (int n = lws_write(socket, buf.data() + LWS_PRE, msgData.buf.length(), msgData.bBinary ? LWS_WRITE_BINARY : LWS_WRITE_TEXT) < 0)
        {
            lwsl_err("%s: WRITEABLE: %d\n", __func__, n);
            // todo:close the connect
            break;
        }
        if (m_pGroup)
        {
            m_pGroup->onDataSent(this, msgData.msgId);
        }
        if (lws_send_pipe_choked(socket))
        {
            lws_callback_on_writable(socket);
            break;
        }
    };
}

void SvrConnection::onRecv(const std::string &message, bool isLastMessage, bool bBinary)
{
    this->receiveStream << message;
    if (isLastMessage)
    {
        if (m_pGroup)
        {
            m_pGroup->onDataRecv(this, message.c_str(), message.length(), bBinary);
        }
        this->receiveStream.str(std::string{});
    }
}

int SvrConnection::sendText(const char *text, int nLen)
{
    if (nLen < 0)
        nLen = strlen(text);
    return send(std::string(text, nLen), false);
}
int SvrConnection::sendBinary(const void *data, int nLen)
{
    return send(std::string((const char *)data, nLen), true);
}
SNSEND