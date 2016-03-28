#include "common/IRenderer.h"

#include "VisStream.h"

using namespace trinity;

IRenderer::IRenderer(std::shared_ptr<VisStream> s)
    : m_visStream(s){};

IRenderer::IRenderer(std::shared_ptr<VisStream> s, std::unique_ptr<IIO> io)
    : m_visStream(s)
    , m_io(std::move(io)){};


std::shared_ptr<VisStream> trinity::IRenderer::getVisStream() {
    return m_visStream;
}

const IRenderer::RenderModeMapper& IRenderer::renderModeMapper() {
    static RenderModeMapper mapper;
    if (mapper.empty()) {
        mapper.insert(ERenderMode::RM_1DTRANS, "1DTrans");
        mapper.insert(ERenderMode::RM_2DTRANS, "2DTrans");
        mapper.insert(ERenderMode::RM_ISOSURFACE, "Isosurface");
        mapper.insert(ERenderMode::RM_CLEARVIEW, "Clearview");
        mapper.insert(ERenderMode::RM_INVALID, "Invalid");
    }
    return mapper;
}

void IRenderer::PhongColorTriple::serialize(ISerialWriter& writer) const {
    writer.appendObject("ambient", ambient);
    writer.appendObject("diffuse", diffuse);
    writer.appendObject("specular", specular);
}

void IRenderer::PhongColorTriple::deserialize(const ISerialReader& reader) {
    ambient = reader.getSerializable<Core::Math::Vec4ui8>("ambient");
    diffuse = reader.getSerializable<Core::Math::Vec4ui8>("diffuse");
    specular = reader.getSerializable<Core::Math::Vec4ui8>("specular");
}