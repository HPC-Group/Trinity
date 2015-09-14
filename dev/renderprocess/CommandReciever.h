#ifndef COMMANDRECIEVER
#define COMMANDRECIEVER


#include <base/Error.h>
#include <base/BytePacket.h>
#include <net/TCPNetworkAddress.h>
#include <net/LoopbackNetworkService.h>
#include <net/LoopbackConnectionListener.h>
#include <net/TCPNetworkService.h>
#include <net/Error.h>

#include <renderer/IRenderer.h>

#include <thread>


using namespace mocca;
using namespace mocca::net;
using namespace Tuvok::Renderer;


class CommandReciever{
public:
    CommandReciever(uint32_t port = 42420, std::shared_ptr<IRenderer> renderer = nullptr);
    virtual ~CommandReciever();

    void setRenderer(std::shared_ptr<IRenderer> renderer);

    void endCommandReciever();

    void run();

    void startThread();
	void joinThread();

protected:
    void handleMsg(std::string& msg);

//remote commands
private:
    void SetFirstPersonMode(bool mode);
    void RotateCamera(Core::Math::Vec3f rotation);                                              //
    void MoveCamera(Core::Math::Vec3f direction);                                               //
    void SetCameraZoom(float f);
    void ZoomCamera(float f);

    void SetIsoValue(float fIsoValue);                                                          //
	void SetIsoValueRelative(float fIsovalue);
	void GetIsoValue();                                                                        //
	void SetIsosurfaceColor(Core::Math::Vec3f vColor);
	void GetIsosurfaceColor();
	void SetColorDataset(bool isColor);

	void SetViewParameters(float angle, float znear, float zfar);
	void GetProjectionMatrix();
	void GetViewMatrix();

	void SetBackgroundColors(Core::Math::Vec3f vTopColor,Core::Math::Vec3f vBottomColor);
	void GetBackgroundColor(const uint8_t index);

	void SetUseLighting(bool bUseLighting);
	void SetLightingParameters(Core::Math::Vec4f cAmbient,
			Core::Math::Vec4f cDiffuse,
			Core::Math::Vec4f cSpecular);
	void GetAmbientColor();
	void GetDiffuseColor();
	void GetSpecularColor();
    void SetSampleRateModifier(float fSampleRateModifier);
	void ResetCamera();

	void GetSize();
    void Resize(Core::Math::Vec2ui vWinSize);

    void SetViewPort(Core::Math::Vec2ui lower_left, Core::Math::Vec2ui upper_right,
                                 bool decrease_screen_res = false);

private:
    std::unique_ptr<IConnectionListener>    _listener;
    std::unique_ptr<AbstractConnection>     _connection;

    std::shared_ptr<IRenderer>              _renderer;

    uint32_t                                _port;

    bool                                    _keepAlive;

    std::unique_ptr<std::thread>            _thread;
};


#endif // COMMANDRECIEVER
