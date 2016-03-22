#pragma once

#include "commands/ISerializable.h"

#include <memory>
#include <string>
#include <vector>

#include "silverbullet/math/Vectors.h"

namespace trinity {

class TransferFunction1D : public SerializableTemplate<TransferFunction1D> {
public:
    TransferFunction1D(size_t iSize = 0);
    TransferFunction1D(const std::string& filename);
    ~TransferFunction1D(void);

    void setStdFunction(float fCenterPoint = 0.5f, float fInvGradient = 0.5f);
    void setStdFunction(float fCenterPoint, float fInvGradient, int iComponent, bool bInvertedStep);
    void set(const std::vector<uint8_t>& data);

    size_t getSize() const { return m_colorData.size() / 4; }
    void resize(size_t iSize);
    void resample(size_t iTargetSize);
    void fillOrTruncate(size_t iTargetSize);

    Core::Math::Vec4ui8 getColor(size_t index) const;
    void setColor(size_t index, Core::Math::Vec4ui8 color);

    bool load(const std::string& filename);
    bool load(const std::string& filename, size_t iTargetSize);
    bool load(std::istream& file);
    bool load(std::istream& file, size_t iTargetSize);
    bool save(std::ostream& file) const;
    bool save(const std::string& filename) const;

    void clear();

    const std::vector<uint8_t>& getRAWData() const { return m_colorData; }

    void computeNonZeroLimits();
    const Core::Math::Vec2ui64& getNonZeroLimits() { return m_vValueBBox; }

    // from ISerializable
    void serialize(ISerialWriter& writer) const override;
    void deserialize(const ISerialReader& reader) override;

private:
    Core::Math::Vec2ui64 m_vValueBBox;
    std::vector<uint8_t> m_colorData;

    float smoothstep(float x) const;
};
}