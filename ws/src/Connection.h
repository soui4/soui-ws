#ifndef WS_CONNECTION_H
#define WS_CONNECTION_H

#include <mutex>
#include <libwebsockets.h>
#include <sstream>
#include <string>
#include <list>
#include <helper/obj-ref-impl.hpp>
#include "ws-i.h"

SNSBEGIN

class Connection : public TObjRefImpl<IConnection> {
  public:
    Connection(lws *socket, IConnGroup *pListener, int id);
    ~Connection();

    int send(const std::string &text, bool bBinary);
    void onRecv(const std::string &message, bool isLastMessage, bool bBinary);
    void sendBuf();

  public:
    int isValid() const override;
    int sendText(const char *text, int nLen = -1) override;
    int sendBinary(const void *data, int nLen) override;
    IConnGroup *getGroup() override
    {
        return m_pListener;
    }
    int getId() const override
    {
        return m_id;
    }

  private:
    int genMsgId()
    {
        m_msgId++;
        if (m_msgId < 0)
            m_msgId = 0;
        return m_msgId;
    }
    int m_id;
    int m_groupId;
    lws *socket;

    std::mutex m_mutex;

    IConnGroup *m_pListener;
    struct MsgData
    {
        std::string buf;
        bool bBinary;
        int msgId;
    };

    std::list<MsgData> sendingBuf;
    int m_msgId;
    std::stringstream receiveStream;
};
SNSEND
#endif // WS_CONNECTION_H
