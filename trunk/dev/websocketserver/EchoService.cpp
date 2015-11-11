#include "StdAfx.h"
#include "EchoService.h"

#include "WebsocketClient.h"
#include "WebsocketDataMessage.h"

#include <sstream>
#include <iomanip>

#include <assert.h>
#include <stdio.h>
#include <string>

#include <chrono>
#include <iostream>
#include <fstream>


#include <lz4/lz4.h>
#include <lz4/lz4hc.h>

#include "rapidjson-0.11\rapidjson\include\rapidjson\document.h"


using namespace Tuvok::Renderer;
using namespace Tuvok;
using namespace Core::Math;
using namespace ghoul::logging;
using namespace std;

using std::cout;
using std::endl;
using std::shared_ptr;

static float f = 0.01;
static float zoom = 1.5f;

std::shared_ptr<IRenderer> renderer = NULL;

EchoService::EchoService(void)
{
	// (4)replace the getInstance call with :connect to processing Manager at known IP
	IRenderManager& manager = RenderManager::getInstance();

	// (7) connect to processing process(given in connection) ??

	//(8) send LOAD_DATA json based msg {"params":{"id": walnut}??
	renderer = manager.createRenderer("localhost", 1234, Visibility::hidden, Vec2ui(640, 480), "walnut.uvf", "none");

	m_roateThread = thread(&EchoService::rotateCameraThread, this);
}


EchoService::~EchoService(void)
{
}

static vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}



static string uint8_vector_to_hex_string(const vector<uint8_t>& v) {
	stringstream ss;
	ss << std::hex << std::setfill('0');
	vector<uint8_t>::const_iterator it;

	/*for (it = v.begin(); it != v.end(); it++) {
	ss << "0x" << std::setw(2) << static_cast<unsigned>(*it);
	}*/

	for (it = v.begin(); it != v.end(); it++) {
		ss << *it;
	}

	//std::cout << ss.str() << std::endl;

	return ss.str();
}




void EchoService::rotateCameraThread()
{
	while (true)
	{
		unique_lock<mutex> lck{ m_rotateMutex };
		//m_rotateCV.wait(lck);
		while (m_rotateQueue.empty()) {
			m_rotateCV.wait(lck);
		}

		auto vec = m_rotateQueue.front();
		m_rotateQueue.pop();
		lck.unlock();

		if (renderer != NULL)
			renderer->RotateCamera(vec);
	}
}

void EchoService::handle(LogicalConnection* pClient, IncomingPacket* pRequest)
{
	WebsocketDataMessage& request = (WebsocketDataMessage&)(*pRequest);
	WebsocketClient& client = (WebsocketClient&)(*pClient);
	char * json = new char[request.GetArg1().length() + 1];
	std::strcpy(json, request.GetArg1().c_str());

	rapidjson::Document msg;
	msg.Parse<0>(json);
	delete json;
	string method = msg["method"].GetString();


		if (method == "ROTATE_CAMERA") {

			int x = msg["params"]["x"].GetInt();
			int y = msg["params"]["y"].GetInt();
			int z = msg["params"]["z"].GetInt();

			Core::Math::Vec3f v1 = Core::Math::Vec3f(-y * 5, x * 5, z);

			unique_lock<mutex> lck{ m_rotateMutex };
			if (m_rotateQueue.empty())
				m_rotateQueue.push(v1);
			m_rotateCV.notify_one();
			
	}

	/*
	if (request.GetArg1().find("ROTATE_CAMERA") != std::string::npos) {


		vector<string> rotationString = split(request.GetArg1(), ':');
		vector<string> rotationVector = split(rotationString[1], ',');

		Core::Math::Vec3f v1 = Core::Math::Vec3f(-stof(rotationVector[1])*5, stof(rotationVector[0])*5, stof(rotationVector[2]));

		unique_lock<mutex> lck{ m_rotateMutex };
		if (m_rotateQueue.empty())
			m_rotateQueue.push(v1);
		m_rotateCV.notify_one();
	}*/

	if (method == "GET_IMAGE"){
		if (renderer != NULL)
		{
			FrameData frame = renderer->ReadFrameBuffer();

			if (frame._data.size() > 0)
			{
				WebsocketDataMessage response(EchoCommunication);
				response.SetArguments(uint8_vector_to_hex_string(frame._data));
				SendResult result = client.PushPacket(&response, true);
			}
		}
	}

	/*WebsocketDataMessage response(EchoCommunication);
	response.SetArguments(request.GetArg1());
	SendResult result = client.PushPacket(&response, false);*/
}
