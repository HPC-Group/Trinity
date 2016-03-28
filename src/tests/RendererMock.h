#include "gmock/gmock.h"

#include "common/IRenderer.h"

class RendererMock : public trinity::IRenderer {
public:
    MOCK_METHOD1(setRenderMode, void(IRenderer::ERenderMode));
    MOCK_METHOD1(supportsRenderMode, bool(IRenderer::ERenderMode));
    MOCK_METHOD1(setIsoValue, void(float));
    MOCK_METHOD1(zoomCamera, void(float));
    MOCK_METHOD0(initContext, void());
    MOCK_METHOD0(deleteContext, void());
    MOCK_METHOD1(setActiveModality, void(uint64_t));
    MOCK_CONST_METHOD0(getActiveModality, uint64_t());
    MOCK_METHOD1(setActiveTimestep, void(uint64_t));
    MOCK_CONST_METHOD0(getActiveTimestep, uint64_t());
    MOCK_CONST_METHOD0(getModalityCount, uint64_t());
    MOCK_CONST_METHOD0(getTimestepCount, uint64_t());
    MOCK_METHOD1(set1DTransferFunction, void(const TransferFunction1D&));
    MOCK_CONST_METHOD1(getDefault1DTransferFunction, TransferFunction1D(uint64_t));
    MOCK_CONST_METHOD0(getDefault1DTransferFunctionCount, uint64_t());
    MOCK_CONST_METHOD0(get1DHistogram, std::vector<uint64_t>());
};