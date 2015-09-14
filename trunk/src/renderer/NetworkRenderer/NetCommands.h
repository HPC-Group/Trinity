#ifndef TNG_NetCommands
#define TNG_NetCommands

#include <string>

namespace Tuvok{
namespace Renderer{

namespace Commands{
static const std::string OPENTICKET     = "OPTI";
static const std::string INIT           = "INIT";
static const std::string FRAME          = "FRME";

static const std::string ROT            = "ROTA";
static const std::string MOV            = "MOVE";
static const std::string ZOM            = "ZOOM";

static const std::string SISO           = "SISO";
static const std::string SISOR          = "SISR";
static const std::string GISO           = "GISO";

static const std::string SISOCOLOR      = "SISC";
static const std::string GISOCOLOR      = "GISC";
static const std::string SCOLDS         = "SCDS";

static const std::string SVIEWPARA      = "SVPA";
static const std::string GPROJMAT       = "GPRM";
static const std::string GVIEWMAT       = "GVWM";

static const std::string SBGCOL         = "SBGC";
static const std::string GBGCOL         = "GBGC";

static const std::string SUSELIGHT      = "SULG";
static const std::string SLIGHTPA       = "SLPA";
static const std::string GAMBIENT       = "GAMB";
static const std::string GDIFFUSE       = "GDIF";
static const std::string GSPECULAR      = "GSPEC";

static const std::string SSAMPLERATE    = "SSRM";
static const std::string RESETCAM       = "RCAM";

static const std::string GRES           = "GRES";
static const std::string SRES           = "SRES";
static const std::string SVIEWPORT      = "SVWP";


};

};
};

#endif
