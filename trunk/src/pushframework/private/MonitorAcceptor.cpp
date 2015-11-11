/********************************************************************
	File :			MonitorAcceptor.cpp
	Creation date :	2010/6/27

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
#include "StdAfx.h"
#include "MonitorAcceptor.h"
#include "XMLProtocol.h"

#include "ServerImpl.h"
#include "ChannelFactory.h"

namespace PushFramework
{

MonitorAcceptor monitorAcceptor;

MonitorAcceptor::MonitorAcceptor()
{
    pProtocol = new XMLProtocol();

    options.isTcpNoDelay = true;
    options.interfaceAddress = NULL;
    options.listeningBackLog = 10;
    options.pProtocol = pProtocol;
    options.synAttackPrevention = false;
    options.uReadBufferSize = 8192;
    options.uSendBufferSize = 8192;
    options.uIntermediateReceiveBufferSize = 8192*2;
    options.uIntermediateSendBufferSize = 8192*3;
}

MonitorAcceptor::~MonitorAcceptor(void)
{
    delete pProtocol;
}

bool MonitorAcceptor::handleAcceptedSocket( SOCKET clientSocket, SOCKADDR_IN address )
{
    return channelFactory.SetupDashboardConnection(clientSocket, address, this);
}

Protocol* MonitorAcceptor::getProtocol()
{
    return pProtocol;
}

}
