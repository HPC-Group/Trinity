/********************************************************************
	File :			BroadcastStreamerManager.cpp
	Creation date :	2012/01/31

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
#include "BroadcastStreamerManager.h"
#include "BroadcastStreamer.h"

namespace PushFramework
{

BroadcastStreamerManager broadcastStreamerManager;

BroadcastStreamerManager::BroadcastStreamerManager(void)
{
}

BroadcastStreamerManager::~BroadcastStreamerManager(void)
{
}

void BroadcastStreamerManager::Start()
{
    int nStreamers = 1;
    for (int i=0; i<nStreamers; i++)
    {
        BroadcastStreamer* pStreamer = new BroadcastStreamer();
        streamersList.push_back( pStreamer);
        pStreamer->Start();
    }
}

void BroadcastStreamerManager::Stop()
{
    streamersListT::iterator it = streamersList.begin();
    while (it!=streamersList.end())
    {
        BroadcastStreamer* pStreamer = *it;
        pStreamer->Stop();
        delete pStreamer;
        it = streamersList.erase(it);
    }
}

void BroadcastStreamerManager::AddClient( LogicalConnection* pLogicalConnection )
{
    BroadcastStreamer* pLessBusy = NULL;

    streamersListT::iterator it = streamersList.begin();
    while (it!=streamersList.end())
    {
        if (pLessBusy)
        {
            if (pLessBusy->GetItemsCount() > (*it)->GetItemsCount())
            {
                pLessBusy = (*it);
            }
        }
        else
            pLessBusy = (*it);
        it++;
    }

    if (pLessBusy)
    {
        pLessBusy->AddItem(pLogicalConnection);
    }
}

void BroadcastStreamerManager::Reshuffle()
{
    streamersListT::iterator it = streamersList.begin();
    while (it!=streamersList.end())
    {
        BroadcastStreamer* pStreamer = *it;
        pStreamer->Reshuffle();
        it++;
    }
}

}
