enum ERenderMode {
  RM_1DTRANS = 0,
  RM_2DTRANS,
  RM_ISOSURFACE,
  RM_CLEARVIEW,
  RM_INVALID
};

enum BBoxMode {
  BBM_NONE = 0,
  BBM_DATASET,
  BBM_BRICKS
};


struct phongColorTripple {
  Core::Math::Vec4ui8 ambient;
  Core::Math::Vec4ui8 diffuse;
  Core::Math::Vec4ui8 specular;
};



// GLOBAL RENDERMODE SETTINGS
virtual void setRenderMode(ERenderMode mode) = 0;
virtual bool supportsRenderMode(ERenderMode mode) = 0;

virtual void setModality(uint64_t modality) const = 0;
virtual void uint64_t getModality() const = 0;
virtual void setTimestep(uint64_t timestep) const = 0;
virtual void uint64_t getTimestep() const = 0;

// 1D TF
virtual void set1DTransferFunction(const TransferFunction1D& tf) = 0;
virtual const TransferFunction1D getDefault1DTransferFunction(uint64_t index) const = 0;
virtual const uint64_t getDefault1DTransferFunctionCount() const = 0;
virtual std::vector<uint64_t> get1DHistogram() const = 0;

// 2D TF
virtual void set2DTransferFunction(const TransferFunction2D& tf) = 0;
virtual const TransferFunction2D getDefaul2DTransferFunction(uint64_t index) const = 0;
virtual const uint64_t getDefault2DTransferFunctionCount() const = 0;
virtual std::vector<uint64_t> get2DHistogram() const = 0;

// ISO-SURFACE FUNCTIONS
virtual void setIsoValue(uint8_t surfaceIndex, float fIsoValue) = 0;
virtual float getIsoValue(uint8_t surfaceIndex) = 0;
virtual void setIsosurfaceColor(uint8_t surfaceIndex, const Core::Math::Vec3ui8& vColor) = 0;
virtual Core::Math::Vec3ui8 getIsosurfaceColor(uint8_t surfaceIndex) const = 0;
virtual Core::Math::Vec2f getRange() const = 0;

// CLEARVIEW FUNCTIONS
virtual void setClearViewPosition(const Core::Math::Vec2f& vNormalizedWindowPos) = 0;
virtual void setClearViewRadius(float f) = 0;
// isovalues and colors are set using the iso functions with surface index 1

// LIGTHING FUNCTIONS
virtual void enableLighting(bool enable) = 0;
virtual void setLightingColors(const phongColorTripple& colors) = 0;
virtual const phongColorTripple getLightingColors() const = 0;
virtual void setLightDirection(const Core::Math::Vec3f& direction) = 0;
virtual Core::Math::Vec3f getLightDirection() const = 0;

// SAMPLE RATE FUNCTIONS
virtual void setSampleRateModifier(float fSampleRateModifier) = 0;
virtual float getSampleRateModifier() = 0;


// render parameters
virtual void setBoundingBoxMode(BBoxMode mode) = 0;
virtual BBoxMode getBoundingBoxMode() const = 0;

virtual void setRendererSpecials(const std::vector<uint64_t>& params) = 0;

virtual void setBackgroundColors(std::pair<Core::Math::Vec3ui8, Core::Math::Vec3ui8> colors) = 0;
virtual std::pair<Core::Math::Vec3ui8, Core::Math::Vec3ui8> getBackgroundColors() = 0;

// CLIPPING
virtual void setClipVolume(Core::Math::Vec3f minValues, Core::Math::Vec3f maxValues) = 0;

// TRANSFORMATION
virtual void setViewParameters(float angle, float znear, float zfar) = 0;

virtual void rotateCamera(Core::Math::Vec3f rotation) = 0;
virtual void moveCamera(Core::Math::Vec3f direction) = 0;

virtual void rotateObject(Core::Math::Vec3f rotation) = 0;
virtual void moveObject(Core::Math::Vec3f direction) = 0;

virtual void resetCamera() = 0;
virtual void resetObject() = 0;






