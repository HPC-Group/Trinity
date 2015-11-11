/********************************************************************
	File :			LogicalConnection.cpp
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
#include "../include/LogicalConnection.h"

#include "PhysicalConnection.h"



namespace PushFramework
{


LogicalConnection::LogicalConnection(void)
{
    dtConnectTime = time(NULL);
    nRefrenceCounter = 0;
}

LogicalConnection::~LogicalConnection(void)
{
    delete pPhysicalConnection;
}

SendResult LogicalConnection::PushPacket( OutgoingPacket* pPacket, bool binary)
{
	pPhysicalConnection->SetBinaryFlag(binary);
    return pPhysicalConnection->PushPacket(pPacket);
}

PushFramework::SendResult LogicalConnection::TryPushPacket( OutgoingPacket* pPacket )
{
    return pPhysicalConnection->TryPushPacket(pPacket);
}

bool LogicalConnection::getChannelInfos( std::string& ip, unsigned int& port )
{
    ip = pPhysicalConnection->getPeerIP();
    port = pPhysicalConnection->getPeerPort();
    return true;
}

double LogicalConnection::getVisitDuration()
{
    time_t current = time(NULL);
    return difftime(current, dtConnectTime);
}

bool LogicalConnection::CanDelete()
{
    return nRefrenceCounter==0 && pPhysicalConnection->CheckIfUnusedByIOWorkers();
}

void LogicalConnection::IncrementUsage()
{
    AtomicIncrement(nRefrenceCounter);
}

void LogicalConnection::DecrementUsage()
{
    AtomicDecrement(nRefrenceCounter);
}

void LogicalConnection::Return()
{
    DecrementUsage();
}



}

