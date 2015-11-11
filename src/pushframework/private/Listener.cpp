/********************************************************************
	File :			Listener.cpp
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
#include "Listener.h"


#ifdef Plateform_Windows
#include "win\ListenerImpl_Win.h"
#else
#ifdef Plateform_Linux
#include "linux\ListenerImpl_Linux.h"
#endif
#endif



namespace PushFramework
{


Listener::Listener()
{
    pImpl = new ListenerImpl(this);
}
Listener::~Listener(void)
{
    delete pImpl;
}

void Listener::setListeningPort( unsigned int uPort )
{
    this->uPort = uPort;
}
ListenerOptions& Listener::getOptions()
{
    return options;
}

bool Listener::startListening()
{
    return pImpl->startListening();
}

void Listener::stopListening()
{
    pImpl->stopListening();
}

}
