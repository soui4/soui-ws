#include <ws-i.h>
#include "wsclient.h"
#include "wsServer.h"

SNSBEGIN

class Websocket : public TObjRefImpl<IWebsocket> {
  public:
    IWsClient *CreateWsClient(IConnListener *pListener) override
    {
        return new WsClient(pListener);
    }
    IWsServer *CreateWsServer(ISvrListener *pListener) override
    {
        return new WsServer(pListener);
    }
};

SOUI_COM_C BOOL SOUI_COM_API SCreateInstance(IObjRef **ppRet)
{
    *ppRet = new Websocket();
    return TRUE;
}

SNSEND

SOUI_COM_C BOOL SOUI_COM_API Ws_CreateInstance(IObjRef **ppRet)
{
    return SOUI::SCreateInstance(ppRet);
}
