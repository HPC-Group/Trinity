/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012-2014                                                               *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <ghoul/opengl/ghoul_gl.h>
#include <ghoul/glm.h>
#include <glm/gtx/std_based_type.hpp>
#include <string> 

namespace ghoul {
namespace opengl {

/**
 * This class is a wrapper for an OpenGL texture. It wraps the OpenGL method for
 * generating (<code>glGenTextures</code>), binding (<code>glBindTexture</code>, #bind),
 * enabling (<code>glEnable</code>, #enable) and others. A new texture can be created
 * either from an image file using the TextureManager or #loadTexture function or using a
 * pointer to the data. The data layout must be as specified by the
 * <code>glTexImageXD</code> functions (see OpenGL reference). After creation a Texture
 * must be uploaded (<code>uploadTexture</code>, #uploadTexture) before it can be used in
 * the graphics pipeline. See http://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml
 * for detailed information on the different arguments
 */
class Texture {
public:
    /**
     * This enum specifies the allowed formats for the Texture%s. These are directly
     * mapped to the appropriate OpenGL constants. See
     * http://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml for more information.
     */
    enum Format {
        Red = GL_RED, ///< GL_RED
        RG = GL_RG, ///< GL_RG
        RGB = GL_RGB, ///< GL_RGB
        BGR = GL_BGR, ///< GL_BGR
        RGBA = GL_RGBA, ///< GL_RGBA
        BGRA = GL_BGRA, ///< GL_BGRA
        DepthComponent = GL_DEPTH_COMPONENT ///< GL_DEPTH_COMPONENT
    };

    /**
     * This enum specifies the filtering method this texture will use to interpolate
     * between two texel. The values for this enum correspond directly to OpenGL settings.
     * See the OpenGL specification for details.
     */
    enum class FilterMode {
        Nearest, ///< GL_NEAREST
        Linear, ///< GL_LINEAR
        AnisotropicMipMap
    };

    /**
     * This enum specifies the wrapping mode this texture will use at the edges of the
     * texture. The values for this enum correspond directly to OpenGL settings. See the
     * OpenGL specification for details.
     */
    enum class WrappingMode {
        Repeat = GL_REPEAT, ///< GL_REPEAT
        Clamp = GL_CLAMP, ///< GL_CLAMP
        ClampToEdge = GL_CLAMP_TO_EDGE, ///< GL_CLAMP_TO_EDGE
        ClampToBorder = GL_CLAMP_TO_BORDER, ///< GL_CLAMP_TO_BORDER
        MirroredRepeat = GL_MIRRORED_REPEAT ///< GL_MIRRORED_REPEAT
    };

    /**
     * This constructor will create storage internally to fit the amount of data that is
     * necessary for the <code>dimensions * bytesPerPixel</code> (which is in turn
     * dependent on the dataType). The Texture can be 1D, 2D, or 3D depending on how many
     * components are equal to <code>1</code>.
     * \param dimensions The dimensions of the texture. A 3D texture will be created 
     * if all components are bigger than <code>1</code>, a 2D texture will be created if
     * the <code>z</code> component is equal to <code>1</code>, while a 1D texture is
     * created if the <code>y</code> and <code>z</code> component is equal to
     * <code>1</code>
     * \param format Specifies the format of the data
     * \param internalFormat The internal format for the texture. See
     * http://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml Tables 1,2,  and 3 for
     * concrete values. In addition, the S3TC_DXT formats can be used to support 
     * hardware compression. See http://www.opengl.org/wiki/Image_Format#S3TC.2FDXT for
     * more information
     * \param dataType The data type of the pixel data. See
     * http://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml for a list of possible
     * values
     * \param filter The Texture::FilterMode that will be used to interpolate between texels
     * \param wrapping The Texture::WrappingMode that will be used to generate values on
     * the border of the texture
     */
    Texture(glm::size3_t dimensions, Format format = Format::RGBA,
            GLint internalFormat = GL_RGBA, GLenum dataType = GL_UNSIGNED_BYTE,
            FilterMode filter = FilterMode::Linear,
            WrappingMode wrapping = WrappingMode::Repeat);

    /**
     * This constructor will generate a Texture out of the passed data. The data should 
     * contain enough bytes to fill <code>dimensions * bytesPerPixel</code> (which is in
     * turn dependent on the dataType) pixel. The Texture can be 1D, 2D, or 3D depending
     * on how many components are equal to <code>1</code>.
     * \param data The data from which to generate the Texture. The data must be in a 
     * linear format and (in 2D and 3D cases) aligned so that it can be accessed using the
     * following equations: <code>(y * dimensions.x) + x</code> in the 2D case and 
     * <code>(z * dimensions.x * dimensions.y) + (y * dimensions.x) + x</code> in the 3D 
     * case. This Texture will take ownership of the data array and will delete it once 
     * this object is destroyed
     * \param dimensions The dimensions of the texture. A 3D texture will be created 
     * if all components are bigger than <code>1</code>, a 2D texture will be created if
     * the <code>z</code> component is equal to <code>1</code>, while a 1D texture is
     * created if the <code>y</code> and <code>z</code> component is equal to
     * <code>1</code>
     * \param format Specifies the format of the data
     * \param internalFormat The internal format for the texture. See
     * http://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml Tables 1,2, and 3 for
     * concrete values. In addition, the S3TC_DXT formats can be used to support 
     * hardware compression. See http://www.opengl.org/wiki/Image_Format#S3TC.2FDXT for
     * more information
     * \param dataType The data type of the pixel data. See
     * http://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml for a list of possible
     * values
     * \param filter The Texture::FilterMode that will be used to interpolate between
     * texels
     * \param wrapping The Texture::WrappingMode that will be used to generate values on
     * the border of the texture
     */
    Texture(void* data, glm::size3_t dimensions, Format format = Format::RGBA,
            GLint internalFormat = GL_RGBA, GLenum dataType = GL_UNSIGNED_BYTE,
            FilterMode filter = FilterMode::Linear,
            WrappingMode wrapping = WrappingMode::Repeat);

    /**
     * Unloads the Texture from GPU memory and destroys the id. The destructor will also 
     * remove the data associated with this texture.
     */
    ~Texture();

    /// Enables this texture type by calling <code>glEnable</code>
    void enable();
    /// Disables this texture type by calling <code>glDisable</code>
    void disable();

    /// Binds this texture to the texture type by calling <code>glBindTexture</code>
    void bind();

    /// Returns the OpenGL name of this texture.
    operator GLuint() const;

    /**
     * Returns an (optional) name for this Texture. The name is not used internally and is
     * solely for external purposes. One possible use is the filename from which the 
     * texture was loaded.
     * \return The name for this texture
     */
    const std::string& getName() const;

    /**
     * Sets an (optional) name for this texture. The name is not used internally and is 
     * solely for external purposes. One possible use is the filename from which the 
     * texture was loaded.
     * \param name The name for this texture
     */
    void setName(std::string name);

    /**
     * Returns the type for this texture.
     * \returns The type for this texture. This value can either be
     * <code>GL_TEXTURE_1D</code>, <code>GL_TEXTURE_2D</code> or
     * <code>GL_TEXTURE_3D</code> depending on the dimension of the stored texture.
     */
    GLenum type() const;

    /**
     * Overrides the type of the texture that was automatically determined.
     * \param type The new type for the texture. This value can either be
     * <code>GL_TEXTURE_1D</code>, <code>GL_TEXTURE_2D</code> or
     * <code>GL_TEXTURE_3D</code>.
     */
    void setType(GLenum type);

    /**
     * Returns the dimensions of this texture. If the texture is a 2D texture, the
     * <code>z</code> component will be equal to <code>1</code> and if the texture is a 1D
     * texture, the <code>y</code> and <code>z</code> components will be equal to
     * <code>1</code>.
     * \return The dimensions of this texture.
     */
    const glm::size3_t& dimensions() const;

    /**
     * Sets new dimensions for this texture. The texture is not automatically updated or 
     * uploaded to the graphics card. Solely the stored dimensions are changed.
     * \param dimensions The new dimensions for this texture
     */
    void setDimensions(glm::size3_t dimensions);

    /// Returns the width of the stored texture
    size_t width() const;

    /// Returns the height of the stored texture or 1 if it is a 1D texture
    size_t height() const;

    /// Returns the depth of the stored texture or 1 if it is a 1D or 2D texture
    size_t depth() const;

    /**
     * Returns the format for this texture. 
     * \returns The format for this texture.
     */
    Format format() const;

    /**
     * Sets the format for this texture. This only updates the internal state and doesn't 
     * affect the texture on the graphics card. Call <code>uploadTexture</code> to apply 
     * changes.
     * \param format The new format for this texture.
     */
    void setFormat(Format format);

    /** 
     * Returns the internal format for this texture. See
     * http://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml for more information and
     * the possible return values.
     */
    GLint internalFormat() const;

    /**
     * Sets the new internal format for this texture. This only updates the internal state
     * and doesn't affect the texture on the graphics card. Call #uploadTexture to apply
     * changes. See http://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml for more
     * information and possible parameter values.
     * \param internalFormat The new internal format. See
     * http://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml Tables 1, 2, and 3 for
     * possible values.
     */
    void setInternalFormat(GLint internalFormat);

    /**
     * Returns the Texture::FilterMode used by this texture.
     * \return The Texture::FilterMode used by this texture
     */
    FilterMode filter() const;

    /**
     * Sets a new Texture::FilterMode for this Texture. The new Texture::FilterMode is
     * applied immediately. If the filter is FilterMode::AnisotropicMipMap, the texture
     * has to be uploaded before calling this method. Otherwise, the result is undefined.
     * \param filter The new Texture::FilterMode for this Texture
     */
    void setFilter(FilterMode filter);

    /**
     * Returns the storage data type for this Texture. For a complete list of available 
     * return values see http://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml for
     * more information.
     * \return The storage data type
     */
    GLenum dataType() const;

    /**
     * Sets a new storage data type for this Texture. For a complete list of available 
     * parameters see http://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml. The new
     * data type is only stored internally and not updated immediately, although it will
     * update the number of* bytes per pixel (see bytesPerPixel method).
     * \param dataType The new storage data type
     */
    void setDataType(GLenum dataType);

    /**
     * Returns the number of channels that are stored in this texture. If the format of
     * the Texture is not in the list of formats found at
     * http://www.opengl.org/sdk/docs/man/xhtml/glTexImage1D.xml, an assertion will be
     * triggered.
     * \return The number of channels that are stored in this texture
     */
    size_t numberOfChannels() const;

    /*
     * Returns the number of bytes each pixel stores.
     * \return The number of bytes each pixel stores
     */
    GLubyte bytesPerPixel() const;

    /**
     * Returns the stored data of the texture. If the memory is deleted, a new
     * (sufficiently) large memory block have been created or a segmentation fault might
     * occur.
     * \return The stored data of the texture
     */
    const void* pixelData() const;

    /**
     * Returns the size the pixel data should have according to the dimensionality and the
     * bytes per pixel. <code>dimensions.x * dimensions.y * dimensions.z * bpp</code>. The 
     * real size of the data can be different if it was set manually.
     * \return The size of the pixel data according to the dimensionality and the bytes
     * per pixel
     */
    size_t expectedPixelDataSize() const;

    /**
     * Sets new data for the texture to use. If the dimensions are not updated and the new 
     * data has a different size, undefined behavior will occur. This Texture will take 
     * ownership of the data array.
     * \param pixels The pointer to the new data array that should be used.
     * \param takeOwnership Should this Texture take ownership of the data and delete
     * it?
     */
    void setPixelData(void* pixels, bool takeOwnership = true);

    /**
     * Returns <code>true</code> if the OpenGL texture is resident in the GPU memory.
     * \return <code>true</code> if the OpenGL texture is resident
     */
    bool isResident() const;

    /** Sets a new Texture::WrappingMode for this Texture. The new mode is applied
     * immediately.
     * \param wrapping The new Texture::WrappingMode for this Texture
     */
    void setWrapping(WrappingMode wrapping);

    /**
     * Returns the currently used Texture::WrappingMode for this texture.
     * \return The currently used Texture::WrappingMode for this texture
     */
    WrappingMode wrapping() const;

    /**
     * Sets the maximum number of MipMap levels to use. This is only valid when the
     * FilterMode::AnisotropicMipMap is selected. Defaults to 8 levels.
     * \param mipMapLevel The MipMap level that should be used in this texture.
     */
    void setMipMapLevel(int mipMapLevel);

    /**
     * Sets the maximum anisotropy level that should be used. This is only valid when the
     * FilterMode::AnisotropicMipMap is selected. On default, the maximum anisotropy
     * supported by the graphics card is used.
     * \param anisotropyLevel The anisotropy level that should be used
     */
    void setAnisotropyLevel(float anisotropyLevel);

    /**
     * Binds and uploads the texture to graphics memory. The Texture has to be of type
     * <code>GL_TEXTURE_1D</code>, <code>GL_TEXTURE_2D</code>, or
     * <code>GL_TEXTURE_3D</code>. The type will be determined automatically based on the
     * provided dimensions
     */
    void uploadTexture();

    /**
     * Downloads the texture from graphics memory and makes it available using the 
     * #pixelData method.
     */
    void downloadTexture();

    /**
     * Changes (=grants/revokes) ownership of the stored data. Changing this will not 
     * change the underlying data.
     * \param hasOwnership <code>true</code> if this Texture should own the data in
     * <code>_pixels</code>
     */
    void setDataOwnership(bool hasOwnership);

    /**
     * Returns if this Texture owns its contained data.
     * \return <code>true</code> if this Texture owns its contained data
     */
    bool dataOwnership() const;

    /**
     * Accesses the <code>i</code>-th texel in the data array. Returns a reference to it 
     * so that it can be changed by the caller. The Texture must be a one dimensional 
     * Texture or an assertion will be triggered. Only Textures with a data type of the 
     * following list can be used to fetch textures: <code>GL_UNSIGNED_BYTE</code>,
     * <code>GL_BYTE</code>, <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to
     * use this function on another type will lead to undefined behavior in the return
     * value.
     * \tparam T The type of the data that is returned. The size of T must be equal to the
     * bytes per pixel stored in the Texture or an assertion will be triggered
     * \param x The position of the texel that should be fetched. This value must be with
     * in the width of the Texture or an assertion will be triggered
     * \return The texel at the specified position casted to the requested type T
     */
    template <class T>
    T& texel(size_t x);

    /**
     * Accesses the <code>i</code>-th texel in the data array. The Texture must be a one
     * dimensional Texture or an assertion will be triggered. Only Textures with a data 
     * type of the following list can be used to fetch textures:
     * <code>GL_UNSIGNED_BYTE</code>, <code>GL_BYTE</code>,
     * <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to
     * use this function on another type will lead to undefined behavior in the return 
     * value.
     * \tparam T The type of the data that is returned. The size of T must be equal to the
     * bytes per pixel stored in the Texture or an assertion will be triggered
     * \param x The position of the texel that should be fetched. This value must be with 
     * in the width of the Texture or an assertion will be triggered
     * \return The texel at the specified position casted to the requested type T
     */
    template <class T>
    const T& texel(size_t x) const;

    /**
     * Accesses the texel at <code>(x,y)</code> from the data array. Returns a reference 
     * to it so that it can be changed by the caller. The Texture must be a 2D dimensional
     * Texture or an assertion will be triggered. Only Textures with a data type of the 
     * following list can be used to fetch textures: <code>GL_UNSIGNED_BYTE</code>,
     * <code>GL_BYTE</code>, <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to
     * use this function on another type will lead to undefined behavior in the return
     * value.
     * \tparam T The type of the data that is returned. The size of T must be equal to the 
     * bytes per pixel stored in the Texture or an assertion will be triggered
     * \param x The coordinate on the <code>width</code> axis. The value must be smaller 
     * than the width of the Texture or an assertion will be triggered
     * \param y The coordinate on the <code>height</code> axis. The value must be smaller
     * than the height of the Texture or an assertion will be triggered
     * \return The texel at the specified position casted to the requested type T
     */
    template <class T>
    T& texel(size_t x, size_t y);

    /**
     * Accesses the texel at <code>(x,y)</code> from the data array. The Texture must be a
     * 2D dimensional Texture or an assertion will be triggered. Only Textures with a data 
     * type of the following list can be used to fetch textures:
     * <code>GL_UNSIGNED_BYTE</code>, <code>GL_BYTE</code>,
     * <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to
     * use this function on another type will lead to undefined behavior in the return 
     * value.
     * \tparam T The type of the data that is returned. The size of T must be equal to the
     * bytes per pixel stored in the Texture or an assertion will be triggered
     * \param x The coordinate on the <code>width</code> axis. The value must be smaller
     * than the width of the Texture or an assertion will be triggered
     * \param y The coordinate on the <code>height</code> axis. The value must be smaller
     * than the height of the Texture or an assertion will be triggered
     * \return The texel at the specified position casted to the requested type T
     */
    template<class T>
    const T& texel(size_t x, size_t y) const;

    /**
     * Accesses the texel at <code>position</code> from the data array. Returns a 
     * reference to it so that it can be changed by the caller. The Texture must be a 2D 
     * dimensional Texture or an assertion will be triggered. Only Textures with a data 
     * type of the following list can be used to fetch textures:
     * <code>GL_UNSIGNED_BYTE</code>, <code>GL_BYTE</code>,
     * <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to
     * use this function on another type will lead to undefined behavior in the return 
     * value.
     * \tparam T The type of the data that is returned. The size of T must be equal to the
     * bytes per pixel stored in the Texture or an assertion will be triggered
     * \param position The coordinate of the texel. <code>position.x</code> must be 
     * smaller than the width of the Texture, while <code>position.y</code> must be 
     * smaller than the height
     * \return The texel at the specified position casted to the requested type T
     */
    template <class T>
    T& texel(const glm::size2_t& position);

    /**
     * Accesses the texel at <code>position</code> from the data array. The Texture must 
     * be a 2D dimensional Texture or an assertion will be triggered. Only Textures with a
     * data type of the following list can be used to fetch textures:
     * <code>GL_UNSIGNED_BYTE</code>, <code>GL_BYTE</code>,
     * <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to 
     * use this function on another type will lead to undefined behavior in the return 
     * value.
     * \tparam T The type of the data that is returned. The size of T must be equal to the
     * bytes per pixel stored in the Texture or an assertion will be triggered
     * \param position The coordinate of the texel. <code>position.x</code> must be 
     * smaller than the width of the Texture, while <code>position.y</code> must be
     * smaller than the height
     * \return The texel at the specified position casted to the requested type T
     */
    template<class T>
    const T& texel(const glm::size2_t& position) const;

    /**
     * Accesses the texel at <code>(x,y,z)</code> from the data array. Returns a reference
     * to it so that it can be changed by the caller. The Texture must be a 3D dimensional 
     * Texture or an assertion will be triggered. Only Textures with a data type of the 
     * following list can be used to fetch textures: <code>GL_UNSIGNED_BYTE</code>,
     * <code>GL_BYTE</code>, <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to
     * use this function on another type will lead to undefined behavior in the return
     * value.
     * \tparam T The type of the data that is returned. The size of T must be equal to the
     * bytes per pixel stored in the Texture or an assertion will be triggered
     * \param x The coordinate on the <code>width</code> axis. The value must be smaller 
     * than the width of the Texture or an assertion will be triggered
     * \param y The coordinate on the <code>height</code> axis. The value must be smaller 
     * than the height of the Texture or an assertion will be triggered
     * \param z The coordinate on the <code>depth</code> axis. The value must be smaller 
     * than the depth of the Texture or an assertion will be triggered
     * \return The texel at the specified position casted to the requested type T
     */
    template <class T>
    T& texel(size_t x, size_t y, size_t z);

    /**
     * Accesses the texel at <code>(x,y,z)</code> from the data array. The Texture must be
     * a 3D dimensional Texture or an assertion will be triggered. Only Textures with a
     * data type of the following list can be used to fetch textures:
     * <code>GL_UNSIGNED_BYTE</code>, <code>GL_BYTE</code>,
     * <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to
     * use this function on another type will lead to undefined behavior in the return 
     * value.
     * \tparam T The type of the data that is returned. The size of T must be equal to the
     * bytes per pixel stored in the Texture or an assertion will be triggered
     * \param x The coordinate on the <code>width</code> axis. The value must be smaller 
     * than the width of the Texture or an assertion will be triggered
     * \param y The coordinate on the <code>height</code> axis. The value must be smaller
     * than the height of the Texture or an assertion will be triggered
     * \param z The coordinate on the <code>depth</code> axis. The value must be smaller 
     * than the depth of the Texture or an assertion will be triggered
     * \return The texel at the specified position casted to the requested type T
     */
    template<class T>
    const T& texel(size_t x, size_t y, size_t z) const;

    /**
     * Accesses the texel at <code>position</code> from the data array. Returns a 
     * reference to it so that it can be changed by the caller. The Texture must be a 3D 
     * dimensional Texture or an assertion will be triggered. Only Textures with a data 
     * type of the following list can be used to fetch textures:
     * <code>GL_UNSIGNED_BYTE</code>, <code>GL_BYTE</code>,
     * <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to
     * use this function on another type will lead to undefined behavior in the return 
     * value.
     * \tparam T The type of the data that is returned. The size of T must be equal to the
     * bytes per pixel stored in the Texture or an assertion will be triggered
     * \param position The coordinate of the texel. <code>position.x</code> must be 
     * smaller than the width of the Texture, while <code>position.y</code> must be 
     * smaller than the height and<code>position.z</code> must be smaller than the depth
     * \return The texel at the specified position casted to the requested type T
     */
    template <class T>
    T& texel(const glm::size3_t& position);

    /**
     * Accesses the texel at <code>position</code> from the data array. The Texture must 
     * be a 3D dimensional Texture or an assertion will be triggered. Only Textures with a 
     * data type of the following list can be used to fetch textures:
     * <code>GL_UNSIGNED_BYTE</code>, <code>GL_BYTE</code>,
     * <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to
     * use this function on another type will lead to undefined behavior in the return 
     * value.
     * \tparam T The type of the data that is returned. The size of T must be equal to the
     * bytes per pixel stored in the Texture or an assertion will be triggered
     * \param position The coordinate of the texel. <code>position.x</code> must be 
     * smaller than the width of the Texture, while <code>position.y</code> must be 
     * smaller than the height and <code>position.z</code> must be smaller than the depth
     * \return The texel at the specified position casted to the requested type T
     */
    template<class T>
    const T& texel(const glm::size3_t& position) const;

    /**
     * Returns the texel at the specified position as a <code>float</code> vector.
     * Independent of the underlying type, each component of the vector is in the range of
     * <code>[0,1]</code>. The Texture must be 1 dimensional or an assertion will be
     * triggered. Only Texture%s with a data type of the following list can be used to
     * fetch textures: <code>GL_UNSIGNED_BYTE</code>, <code>GL_BYTE</code>,
     * <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code> . Trying
     * to use this function on another type will lead to undefined behavior in the return 
     * value.
     * \param x The coordinate on the <code>width</code> axis. The value must be smaller 
     * than the width of the Texture or an assertion will be triggered
     * \return The texel at the specified position as a vector with each component in
     * <code>[0,1]</code>
     */
    glm::vec4 texelAsFloat(size_t x) const;

    /**
     * Returns the texel at the specified position as a <code>float</code> vector.
     * Independent of the underlying type, each component of the vector is in the range of
     * <code>[0,1]</code>. The Texture must be 2 dimensional or an assertion will be
     * triggered. Only Texture%s with a data type of the following list can be used to
     * fetch textures: <code>GL_UNSIGNED_BYTE</code>, <code>GL_BYTE</code>,
     * <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to 
     * use this function on another type will lead to undefined behavior in the return 
     * value.
     * \param x The coordinate on the <code>width</code> axis. The value must be smaller 
     * than the width of the Texture or an assertion will be triggered
     * \param y The coordinate on the <code>height</code> axis. The value must be smaller
     * than the height of the Texture or an assertion will be triggered
     * \return The texel at the specified position as a vector with each component in
     * <code>[0,1]</code>
     */
    glm::vec4 texelAsFloat(size_t x, size_t y) const;

    /**
     * Returns the texel at the specified position as a <code>float</code> vector.
     * Independent of the underlying type, each component of the vector is in the range of
     * <code>[0,1]</code>. The Texture must be 2 dimensional or an assertion will be
     * triggered. Only Textures with a data type of the following list can be used to
     * fetch textures: <code>GL_UNSIGNED_BYTE</code>, <code>GL_BYTE</code>,
     * <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to
     * use this function on another type will lead to undefined behavior in the return 
     * value.
     * \param pos The coordinate of the texel. <code>position.x</code> must be
     * smaller than the width of the Texture, while <code>position.y</code> must be 
     * smaller than the height
     * \return The texel at the specified position as a vector with each component in
     * <code>[0,1]</code>
     */
    glm::vec4 texelAsFloat(const glm::size2_t& pos) const;

    /**
     * Returns the texel at the specified position as a <code>float</code> vector.
     * Independent of the underlying type, each component of the vector is in the range of
     * <code>[0,1]</code>. The Texture must be 3 dimensional or an assertion will be
     * triggered. Only Textures with a data type of the following list can be used to
     * fetch textures: <code>GL_UNSIGNED_BYTE</code>, <code>GL_BYTE</code>,
     * <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to 
     * use this function on another type will lead to undefined behavior in the return 
     * value.
     * \param x The coordinate on the <code>width</code> axis. The value must be smaller
     * than the width of the Texture or an assertion will be triggered
     * \param y The coordinate on the <code>height</code> axis. The value must be smaller 
     * than the height of the Texture or an assertion will be triggered
     * \param z The coordinate on the <code>depth</code> axis. The value must be smaller 
     * than the depth of the Texture or an assertion will be triggered
     * \return The texel at the specified position as a vector with each component in
     * <code>[0,1]</code>
     */
    glm::vec4 texelAsFloat(size_t x, size_t y, size_t z) const;

    /**
     * Returns the texel at the specified position as a <code>float</code> vector.
     * Independent of the underlying type, each component of the vector is in the range of
     * <code>[0,1]</code>. The Texture must be 3 dimensional or an assertion will be
     * triggered. Only Texture%s with a data type of the following list can be used to
     * fetch textures: <code>GL_UNSIGNED_BYTE</code>, <code>GL_BYTE</code>,
     * <code>GL_UNSIGNED_SHORT</code>, <code>GL_SHORT</code>,
     * <code>GL_UNSIGNED_INT</code>, <code>GL_INT</code>, <code>GL_FLOAT</code>. Trying to
     * use this function on another type will lead to undefined behavior in the return 
     * value.
     * \param pos The coordinate of the texel. <code>position.x</code> must be 
     * smaller than the width of the Texture, while <code>position.y</code> must be 
     * smaller than the height and <code>position.z</code> must be smaller than the depth
     * \return The texel at the specified position as a vector with each component in
     * <code>[0,1]</code>
     */
    glm::vec4 texelAsFloat(const glm::size3_t& pos) const;

protected:
    /**
     * Initializes the Texture by determining the Texture type, the bytes per pixel, 
     * generating an OpenGL name, allocating memory (depending on the passed parameter)
     * and applying the filter and wrapping modes
     */
    void initialize(bool allocateData);

    /// Allocates new memory according to the dimensions and the bytes per pixel
    void allocateMemory();

    /// Frees the stored memory
    void destroyMemory();

    /// Generates a new OpenGL name and stores it in the _id variable
    void generateId();

    /**
     * Bind the Texture and apply the changes to the OpenGL state according to the current
     * filter. If the FilterMode is FilterMode::AnisotropicMipMap, the Texture has to be 
     * uploaded before this method is called.
     */
    void applyFilter();

    /**
     * Bind the Texture and apply the changes to the OpenGL state according to the current
     * wrapping mode.
     */
    void applyWrapping();

    /**
     * Calculates the bytes each pixel needs to store its content. This is dependent on 
     * the number of channels as well as the data type this texture has. If an unknown 
     * data type is stored, an assertion will be triggered.
     */
    void calculateBytesPerPixel();

    /// Determines from the dimensions whether this is a 1D, 2D, or 3D texture
    void determineTextureType();

private:
    /**
     * Stores the dimensions of the texture, <code>y</code> and <code>z</code> may be
     * <code>1</code> if the Texture is 1D or 2D
     */
    glm::size3_t _dimensions;
    Format _format;
    GLint _internalFormat;
    GLenum _dataType;
    FilterMode _filter;
    WrappingMode _wrapping;
    GLuint _id;
    GLenum _type;
    GLubyte _bpp;
    int _mipMapLevel;
    float _anisotropyLevel;
    std::string _name;

    bool _hasOwnershipOfData;
    void* _pixels;
};

} // namespace opengl
} // namespace logging

#include "texture.inl"

#endif
