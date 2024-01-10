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

class SvrConnection : public TObjRefImpl<ISvrConnection> {
  public:
    SvrConnection(lws *socket, IConnGroup* pGroup, int id);
    ~SvrConnection();

    int send(const std::string &text, bool bBinary);
    void onRecv(const std::string &message, bool isLastMessage, bool bBinary);
    void sendBuf();

  public:
    int isValid() const override;
    int sendText(const char *text, int nLen = -1) override;
    int sendBinary(const void *data, int nLen) override;
    int getGroupId() const override
    {
        return m_pGroup->getId();
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
    lws *socket;

    std::mutex m_mutex;

    IConnGroup *m_pGroup;
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
