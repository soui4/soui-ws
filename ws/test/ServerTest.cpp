#include <iostream>
#include <chrono>
#include <map>
#include <ws-i.h>
#include <helper/obj-ref-impl.hpp>

using namespace SOUI;

class ConnGroup : public TObjRefImpl<IConnGroup> {
    int m_id;
    std::map<int, IConnection *> m_conns;
    int m_autoId;
    IWsServer* m_pSvr;
  public:
    ConnGroup(int id, IWsServer* pSvr)
        : m_id(id)
        , m_autoId(100000)
        , m_pSvr(pSvr)
    {
    }

  public:
    virtual int getId() const
    {
        return m_id;
    }
    void onConnError(IConnection *pConn, const char *errStr)
    {
    }
    bool onConnected(IConnection *pConn)
    {
        std::cout << "New Connection!" << std::endl;
        if (m_conns.find(pConn->getId()) != m_conns.end())
            return false;
        m_conns[pConn->getId()] = pConn;
        
        char msg[20];
        sprintf(msg, "group size, %d", m_conns.size());
        boardcast(msg, -1, false);
        return true;
    }

    void onDisconnect(IConnection *pConn)
    {
        std::cout << "onDisconnect!" << std::endl;
        m_conns.erase(pConn->getId());

        char msg[20];
        sprintf(msg, "group size, %d", m_conns.size());
        boardcast(msg, -1, false);
    }
    void onDataSent(IConnection *pConn, int nMsgId)
    {
    }
    void onDataRecv(IConnection *pConn, const void *data, int len, bool bBinary)
    {
        std::cout << "Received: " << std::string((const char *)data, len).c_str() << std::endl;
        pConn->sendText("Echo4567890123");
    }

    void boardcast(const void *data, int nLen, bool bBinary) override
    {
        for (auto &it : m_conns)
        {
            if (bBinary)
                it.second->sendBinary(data, nLen);
            else
                it.second->sendText((const char *)data, nLen);
        }
    }
    bool isEmpty() const override
    {
        return m_conns.empty();
    }

    int generateConnId() override
    {
        return ++m_autoId;
    }
};

class SvrListener : public TObjRefImpl<ISvrListener> {
  public:
    // 通过 ISvrListener 继承
    virtual IConnGroup *onNewGroup(int id, IWsServer* pSvr) override
    {
        return new ConnGroup(id, pSvr);
    }
    virtual void onDelGroup(IConnGroup *pGroup) override
    {
        pGroup->Release();
    }

    bool parseConnId(const char *uriPath, const char *uriArgs, int *groupId, int *connId) override
    {
        sscanf(uriArgs, "roomid=%d&userid=%d", groupId, connId);
        return true;
    }
};
using namespace std::chrono_literals;

int main()
{
    {
        SvrListener svrListener;
        IWebsocket *pWs = NULL;
        IWsServer *pSvr = NULL;
        Ws_CreateInstance((IObjRef **)&pWs);
        pSvr = pWs->CreateWsServer(&svrListener);
        int nPort = 4043;
        std::cout << "Started on port " << nPort << std::endl;
        SvrOption optoin = { true, ".\\server.crt", ".\\server.key" };
        pSvr->start(nPort, "websocket", optoin);
        bool bFinish = pSvr->wait(-1);
        std::cout << "server wait ret:" << bFinish << std::endl;
        pSvr->Release();
        pWs->Release();
    }
    std::cout << "server quit" << std::endl;

    system("pause");
}
