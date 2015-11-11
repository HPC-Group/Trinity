
#include "StdAfx.h"
#include "ChannelFactory.h"
#include "ServerImpl.h"
#include "Dispatcher.h"


#ifdef Plateform_Windows
#include "IOQueue_Win.h"
#else
#ifdef Plateform_Linux
#include "IOQueue_Linux.h"
#endif
#endif

namespace PushFramework
{

bool ChannelFactory::SetupPhysicalConnection( SOCKET hSocket, ListenerOptions* pListenerOptions, SOCKADDR_IN address, bool isObserver )
{
    PhysicalConnection* pChannel = new PhysicalConnection(hSocket, address, isObserver, pListenerOptions);

    AddToConnectedList(pChannel);

    //Now Associate with IOCP main Handle
    if (!ioQueue.AddSocketContext(hSocket, pChannel))
    {
        RemoveFromUnattachedList(pChannel);
        delete pChannel;
        //leave socket close to acceptor
        return false;
    }

    dispatcher.OnInitializeReady(pChannel);
    return true;
}
}
