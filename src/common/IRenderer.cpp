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

const IRenderer::BBoxModeMapper& IRenderer::bboxModeMapper() {
    static BBoxModeMapper mapper;
    if (mapper.empty()) {
        mapper.insert(BBoxMode::BBM_NONE, "None");
        mapper.insert(BBoxMode::BBM_DATASET, "Dataset");
        mapper.insert(BBoxMode::BBM_BRICKS, "Bricks");
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

bool IRenderer::PhongColorTriple::equals(const PhongColorTriple& other) const {
    return ambient == other.ambient && diffuse == other.diffuse && specular == other.specular;
}

std::string IRenderer::PhongColorTriple::toString() const {
    std::stringstream stream;
    stream << "ambient: " << ambient << "; diffuse: " << diffuse << "; specular: " << specular;
    return stream.str();
}

bool trinity::operator==(const IRenderer::PhongColorTriple& lhs, const IRenderer::PhongColorTriple& rhs) {
    return lhs.equals(rhs);
}

std::ostream& trinity::operator<<(std::ostream& os, const IRenderer::PhongColorTriple& obj) {
    return os << obj.toString();
}