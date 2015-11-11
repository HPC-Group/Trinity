#pragma once

#include <thread>
#include <queue>
#include <condition_variable>

#include <base/Error.h>
#include <base/BytePacket.h>
#include <net/TCPNetworkAddress.h>
#include <net/LoopbackNetworkService.h>
#include <net/LoopbackConnectionListener.h>
#include <net/TCPNetworkService.h>

//COMPRESSION
#include <lz4/lz4.h>
#include <lz4/lz4hc.h>

#include <renderer/RenderManager.h>
#include <renderer/IRenderer.h>

using namespace mocca;
using namespace mocca::net;
using namespace Tuvok::Renderer;

class EchoService : public Service
{
public:
	EchoService(void);
	~EchoService(void);
	void handle( LogicalConnection* pClient, IncomingPacket* pRequest );
	void rotateCamera(string x, string y, string z);
	void rotateCameraThread();

private:
	thread m_roateThread;
	queue<Core::Math::Vec3f> m_rotateQueue;
	mutex m_rotateMutex;
	condition_variable m_rotateCV;
};

