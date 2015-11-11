/********************************************************************
	File :			BroadcastQueueInfo.cpp
	Creation date :	2012/02/01

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
#include "BroadcastQueueInfo.h"

namespace PushFramework
{


BroadcastQueueInfo::BroadcastQueueInfo( BroadcastQueue* pChannel, unsigned int uPacketQuota )
{
    this->pChannel = pChannel;
    this->uPacketQuota = uPacketQuota;
    pNext = NULL;
}

BroadcastQueueInfo::~BroadcastQueueInfo( void )
{
    delete pNext;
}


BroadcastQueueGroupInfo::BroadcastQueueGroupInfo( unsigned int uPriority )
{
    this->uPriority = uPriority;
    this->pItemList = NULL;
    pNextGroup = NULL;
}

BroadcastQueueGroupInfo::~BroadcastQueueGroupInfo()
{
    delete pItemList;
    delete pNextGroup;
}

void BroadcastQueueGroupInfo::insertChannelInfo( BroadcastQueueInfo* pChannelInfo )
{
    pChannelInfo->pParentGroup = this;

    //
    if (pItemList == NULL)
    {
        pItemList = pChannelInfo;
        return;
    }

    BroadcastQueueInfo* pBefore = NULL;
    BroadcastQueueInfo* pAfter = pItemList;

    while(pAfter!=NULL)
    {
        if (pChannelInfo->uPacketQuota >= pAfter->uPacketQuota)
        {
            break;
        }
        else
        {
            pBefore = pAfter;
            pAfter = pAfter->pNext;
        }
    }

    if (pAfter == NULL)
    {
        pBefore->pNext = pChannelInfo;
    }
    else
    {
        pChannelInfo->pNext = pAfter;

        if (pBefore)
            pBefore->pNext = pChannelInfo;
        else
            pItemList = pChannelInfo;
    }
}

}
