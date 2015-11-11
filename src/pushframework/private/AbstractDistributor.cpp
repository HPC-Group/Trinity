/********************************************************************
	File :			AbstractDistributor.cpp
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
#include "AbstractDistributor.h"


namespace PushFramework
{
AbstractDistributor::AbstractDistributor( void )
{
    stop = false;
    itemsCount = 0;
    InitializeCriticalSection(&csInsertedItemsList);

    lastQueueChange = 0;
    lastAddedItemTime = 0;
    lastListChange = time(NULL);
    lastAddedItemsSyncTime = time(NULL);

}



AbstractDistributor::~AbstractDistributor( void )
{
    DeleteCriticalSection(&csInsertedItemsList);
}

void AbstractDistributor::Reshuffle()
{
    AtomicExchange(lastQueueChange, time(NULL) );
}


void AbstractDistributor::AddItem( ItemPtr pItem )
{
    EnterCriticalSection(&csInsertedItemsList);
    insertedItemsList.push_back(pItem);
    lastAddedItemTime = time(NULL);
    itemsCount++;
    LeaveCriticalSection(&csInsertedItemsList);
}


void AbstractDistributor::DoWork()
{
    while (!stop)
    {
        if (CheckMergeNewInsertedItems() || CheckMergeInactiveItems() )
        {
            ProcessActiveList();
        }
        else
        {
            Sleep(1000);
        }
    }
}

bool AbstractDistributor::CheckMergeNewInsertedItems()
{
    bool ret = false;
    if (lastAddedItemTime > lastAddedItemsSyncTime)
    {
        EnterCriticalSection(&csInsertedItemsList);

        cout << "into CheckMergeNewInsertedItems" << endl;

        itemListT::iterator it = insertedItemsList.begin();
        while (it!=insertedItemsList.end())
        {
            cout << "indeed found an element" << endl;
            ret = true;
            activeList.push_back(*it);
            it++;
        }
        insertedItemsList.clear();
        lastAddedItemsSyncTime = time(NULL);
        LeaveCriticalSection(&csInsertedItemsList);
    }
    return ret;
}


bool AbstractDistributor::CheckMergeInactiveItems()
{
    if (lastQueueChange < lastListChange)
    {
        return false;
    }
    bool ret = false;
    itemListT::iterator it = inactiveList.begin();
    while (it!=inactiveList.end())
    {
        activeList.push_back(*it);
        it ++;
        ret = true;
    }
    inactiveList.clear();
    return ret;
}


void AbstractDistributor::ProcessActiveList()
{
    itemListT::iterator it = activeList.begin();

    while(it != activeList.end())
    {
        ItemPtr pItem = *it;

        ProcessResult result = ProcessItem(pItem);

        if(result == DeleteItem)
        {
            it = activeList.erase(it);
            itemsCount--;
        }
        if (result == PauseProcessing)
        {
            inactiveList.push_back(pItem);
            it = activeList.erase(it);
            lastListChange =time(NULL);
        }
        if (result == ContinueProcessing)
        {
            it++;
        }
        //
        if (it == activeList.end())
        {
            CheckMergeNewInsertedItems();
            CheckMergeInactiveItems();
            it = activeList.begin();
        }
    }
}

int AbstractDistributor::GetItemsCount()
{
    return itemsCount;
}

void AbstractDistributor::Start()
{
#ifdef Plateform_Windows
    DWORD  nThreadID;

    hThread = CreateThread(NULL,// Security
                                     0,						// Stack size - use default
                                     threadProc,     		// Thread fn entry point
                                     (void*) this,			// Param for thread
                                     0,						// Init flag
                                     &nThreadID);			// Thread address
#else
#ifdef Plateform_Linux
    pthread_create(&hThread, NULL, threadProc, (void*) this);
#endif
#endif
}

void AbstractDistributor::Stop()
{
    stop = true;
#ifdef Plateform_Windows
    WaitForSingleObject(hThread, INFINITE);
#else
#ifdef Plateform_Linux
    pthread_join(hThread, NULL);
#endif
#endif
}

#ifdef Plateform_Windows
DWORD WINAPI AbstractDistributor::threadProc(LPVOID lpVoid)
#else
#ifdef Plateform_Linux
void* AbstractDistributor::threadProc(void* lpVoid)
#endif
#endif
{
    AbstractDistributor* pMe = (AbstractDistributor*) lpVoid;
    pMe->DoWork();
#ifdef Plateform_Windows
    return 0;
#endif
}


}
