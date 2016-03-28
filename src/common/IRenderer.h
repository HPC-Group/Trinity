#pragma once

#include "common/IIO.h"

#include "mocca/base/BidirectionalMap.h"

#include <memory>
#include <vector>

namespace trinity {
class VisStream;
class IRenderer {

public:
    enum class ERenderMode {
        RM_1DTRANS = 0,
        RM_2DTRANS,
        RM_ISOSURFACE,
        RM_CLEARVIEW,
        RM_INVALID
    };

    enum class BBoxMode {
        BBM_NONE = 0,
        BBM_DATASET,
        BBM_BRICKS
    };

    enum class PaintLevel {
        PL_REDRAW = 0,
        PL_RECOMPOSE
    };

    struct PhongColorTriple {
        Core::Math::Vec4ui8 ambient;
        Core::Math::Vec4ui8 diffuse;
        Core::Math::Vec4ui8 specular;
    };

    using RenderModeMapper = mocca::BidirectionalMap<ERenderMode, std::string>;
    static const RenderModeMapper& renderModeMapper();
    
    IRenderer() = default;

    // all renderers need a vis stream
    IRenderer(std::shared_ptr<VisStream> s);

    // "real" renderers, in addition, need an I/O interface
    IRenderer(std::shared_ptr<VisStream> s, std::unique_ptr<IIO> io);

    virtual ~IRenderer(){};

    std::shared_ptr<VisStream> getVisStream();


    // base rendering api begin   
    virtual void setRenderMode(ERenderMode mode) = 0;
    virtual bool supportsRenderMode(ERenderMode mode) = 0;

    virtual void setActiveModality(uint64_t modality) = 0;
    virtual uint64_t getActiveModality() const = 0;
    virtual void setActiveTimestep(uint64_t timestep) = 0;
    virtual uint64_t getActiveTimestep() const = 0;

    virtual uint64_t getModalityCount() const = 0;
    virtual uint64_t getTimestepCount() const = 0;
    
    virtual void setIsoValue(float) = 0;
    virtual void zoomCamera(float f) = 0;

	// postponed init and delete due to opengl specific things
    virtual void initContext() = 0;
	virtual void deleteContext() = 0;

protected:
    std::shared_ptr<VisStream> m_visStream;
    std::unique_ptr<IIO> m_io;
};
}