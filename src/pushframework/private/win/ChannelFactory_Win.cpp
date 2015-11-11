/********************************************************************
	File :			ChannelFactory_Win.cpp
	Creation date :	2012/01/29

	License :			Copyright 2010 Ahmed Charfeddine, http://www.pushframework.com

				   Licensed under the Apache License, Version 2.0 (the "License");
				   you may not use this file except in compliance with the License.
				   You may obtain a copy of the License at

					   http://www.apache.org/licenses/LICENSE-2.0

				   Unless required by applicable law or agreed to in writing, software
				   distributed under the License is distributed on an "AS IS" BASIS,
				   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
				   See the License for the specific language governing permissions and
				   limitations under the License.


*********************************************************************/
#include "..\StdAfx.h"
#include "..\ChannelFactory.h"
#include "..\ServerImpl.h"
#include "..\Dispatcher.h"

#ifdef Plateform_Windows
#include "IOQueue_Win.h"
#elif Plateform_Linux
#include "IOQueue_Linux.h"
#endif

namespace PushFramework
{

bool ChannelFactory::SetupPhysicalConnection( SOCKET hSocket, ListenerOptions* pListenerOptions, SOCKADDR_IN address, bool isObserver )
{
    PhysicalConnection* pChannel = new PhysicalConnection(hSocket, address, isObserver, pListenerOptions);

    //Now Associate with IOCP main Handle
    if (!ioQueue.AddSocketContext(hSocket, (DWORD) pChannel))
    {
        delete pChannel;

        //leave socket close to acceptor
        return false;
    }

    AddToConnectedList(pChannel);

    //Unlocking iocp event:
    if(!ioQueue.InitializeSocketContext( (DWORD) pChannel))
    {
        RemoveFromUnattachedList(pChannel);
        delete pChannel;
        //leave socket close to acceptor
        return false;
    }

    return true;
}


}