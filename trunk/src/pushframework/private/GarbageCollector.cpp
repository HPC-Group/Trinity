/********************************************************************
	File :			GarbageCollector.cpp
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
#include "StdAfx.h"
#include "GarbageCollector.h"

#include "../include/LogicalConnection.h"
#include "PhysicalConnection.h"
#include "ScopedLock.h"


namespace PushFramework
{


GarbageCollector garbageCollector;

GarbageCollector::GarbageCollector(void)
{
    InitializeCriticalSection(&cs);
}

GarbageCollector::~GarbageCollector(void)
{
    DeleteCriticalSection(&cs);
}

void GarbageCollector::Activate()
{
    ScopedLock lock(cs);

    for (waitingListT::iterator it = waitingList.begin();
            it!= waitingList.end();
        )
    {
        LogicalConnection* pLogicalConnection = *it;
        if (pLogicalConnection->CanDelete())
        {
            delete pLogicalConnection;
            it =  waitingList.erase(it);
        }
        else
            it++;
    }
}

void GarbageCollector::AddDisposableClient( LogicalConnection* pLogicalConnection )
{
    ScopedLock lock(cs);
    waitingList.push_back(pLogicalConnection);
}

}

