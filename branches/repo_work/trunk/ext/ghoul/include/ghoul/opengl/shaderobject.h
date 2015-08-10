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

#ifndef __SHADEROBJECT_H__
#define __SHADEROBJECT_H__

#include <ghoul/opengl/ghoul_gl.h>
//#include <ghoul/filesystem/file.h>

#include <string>
#include <map>
#include <functional>

namespace ghoul {
	namespace filesystem {
		class File;
	}
namespace opengl {

/**
 * This class is a wrapper for an OpenGL shader object. It represents a single shader
 * object of the types declared in ShaderObject::ShaderType. All ShaderObject%s must be
 * loaded from a file on disk. The shader will load the contents of the file in the 
 * constructor (if a constructor with a filename was chosen) or in the #setShaderFilename
 * method. If the file changes after it has been loaded, the ShaderObject will not change
 * unless #rebuildFromFile is called, which pulls the changes. Each object can have a name
 * that will be used for a debug label (if available) and for the logging. The type of the
 * ShaderObject is selected in the constructor and cannot be changed afterwards. To use
 * the ShaderObjects, they have to be attached to a ProgramObject first.
 */
class ShaderObject {
public:
    /**
     * An enum of the different types of shaders that can be used in OpenGL. They can be 
     * used interchangeably in native OpenGL calls, too. Compute shaders are only 
     * available if OpenGL 4.3 is available.
     */
    enum ShaderType {
        ShaderTypeVertex = GL_VERTEX_SHADER
        , ShaderTypeTesselationControl = GL_TESS_CONTROL_SHADER
        , ShaderTypeTesselationEvaluation = GL_TESS_EVALUATION_SHADER
        , ShaderTypeGeometry = GL_GEOMETRY_SHADER
        , ShaderTypeFragment = GL_FRAGMENT_SHADER
#ifdef GL_VERSION_4_3
	    , ShaderTypeCompute = GL_COMPUTE_SHADER
#endif
    };


	/**
	* A type definition for a callback function that is called if any of
	* the tracked files is changed.
	*/
	typedef std::function<void(const filesystem::File&)> ShaderObjectCallback;

    /**
     * This constructor creates a shader of the passed type with an empty source string. 
     * Before this can be used, a shader must be loaded with #setShaderFilename and it has
     * to be compiled.
     * \param shaderType The type of shader that this ShaderObject will represent
     */
	ShaderObject(ShaderType shaderType, ShaderObjectCallback changeCallback = nullptr);

    /**
     * This constructor creates a shader of the passed type and loads the shader source 
     * from the provided filename. If the filename is an empty string, no source will be 
     * loaded and the shader remains uninitialized. If the filename is a valid file, its 
     * contents will be used to initialize this ShaderObject.
     * \param shaderType The type of shader that this ShaderObject will represent
     * \param filename The name of the file that will be used to load the source of this 
     * shader
     */
    ShaderObject(ShaderType shaderType, std::string filename, 
		ShaderObjectCallback changeCallback = nullptr);

    /**
     * This constructor creates a shader of the passed type and loads the shader source 
     * from the provided filename. If the filename is an empty string, no source will be 
     * loaded and the shader remains uninitialized. If the filename is a valid file, its 
     * contents will be used to initialize this ShaderObject. The internal name, debug 
     * label and logging category will be set based on the user provided name.
     * \param shaderType The type of shader that this ShaderObject will represent
     * \param filename The name of the file that will be used to load the source of this 
     * shader
     * \param name The human readable name of this ShaderObject
     */
    ShaderObject(ShaderType shaderType,
				std::string filename, std::string name, 
				ShaderObjectCallback changeCallback = nullptr);

    /**
     * A copy constructor that will copy all of the internal state, and the shader source,
     * but it will generate a new OpenGL name for the copied object. In addition, if the 
     * content of the specified file has changed between creating the <code>cpy</code>, 
     * the copied Shader will use the changed file. That means a ShaderObject will not
     * cache the contents of the file inside.
     * \param cpy The original object that will be copied
     */
	ShaderObject(const ShaderObject& cpy);

	/**
	 * A move constructor that will move all of the internal state, and the shader source,
	 * but it will leave the other object in an invalid state
	 */
	ShaderObject(ShaderObject&& rhs);

    /**
     * The destructor will mark the OpenGL name as unused again. Because of the way the
     * OpenGL functions work, the name might still be in use if the ShaderObject is
     * attached to a ProgramObject prior to deleting.
     */
    ~ShaderObject();

    /**
     * The casting operator that enabled this ShaderObject to be used in native OpenGL 
     * functions and it returns the OpenGL name.
     */
    operator GLuint() const;

    /**
     * The assignment operator that will copy all of the internal state, and the shader
     * source, but it will generate a new OpenGL name for the assigned object. In 
     * addition, if the content of the specified file has changed between creating the 
     * <code>rhs</code>, the assigned Shader will use the changed file. I.e., a 
     * ShaderObject will not cache the contents of the file inside.
     * \param rhs The original right hand side that will be used to set this object
     * \return A reference to <code>this</code>
     */
	ShaderObject& operator=(const ShaderObject& rhs);

	/**
	 * A move assignment that will move all of the internal state, and the shader source,
	 * but it will leave the other object in an invalid state
	 */
	ShaderObject& operator=(ShaderObject&& rhs);

    /**
     * Sets the internal name of this ShaderObject that changes the logging category by
     * incorporating the name and (if available) set the object label.
     * \param name The new internal name that will be used for this ShaderObject
     */
    void setName(std::string name);

    /**
     * Returns the internal name of this ShaderObject
     * \return The internal name of this ShaderObject
     */
    const std::string& name() const;

    /**
     * Returns <code>true</code> if this ShaderObject has an internal name assigned to it,
     * <code>false</code> otherwise.
     * \return <code>true</code> if this ShaderObject has an internal name assigned to it,
     * <code>false</code> otherwise.
     */
    bool hasName() const;

    /**
     * Returns the filepath for the shader object file
     * \return The filename
     */
	std::string filename();

    /**
     * (Re)sets the filename this ShaderObject is based on. It will load the contents of
     * the file and uses it as the source text for this ShaderObject. If the file can not
     * be opened or is empty an error message is logged and the old shader will be deleted
     * in the process. The loaded shader will not automatically compiled after loading.
     * The method returns if the loading was successful.
     * \param filename The name of the file that will be used to load this shader
     * \return <code>true</code>, if the load was successful, <code>false</code> otherwise
     */
    bool setShaderFilename(std::string filename);

    /**
     * This method will mark the OpenGL name as unused. Because of the way the OpenGL
     * functions, the name might still be in use if the ShaderObject is attached to a 
     * ProgramObject prior to deleting.
     */
    void deleteShader();

    /**
     * This method will reload the shader source from the file specified earlier. The 
     * OpenGL name will not change by this operation. After loading, the ShaderObject is 
     * not compiled. If the file is no longer available, the previous shader source will 
     * be removed.
     */
    bool rebuildFromFile();

    /**
     * This method will compile the shader source in this ShaderObject and returns the 
     * success of this operation. If the compiling fails, the compiler log will be logged 
     * as an error and this will return <code>false</code>.
     * \return <code>true</code> if the shader source was compiled successfully, <code>
     * false</code> otherwise
     */
    bool compile();

    /**
     * Returns the type of this ShaderObject as a human readable string.
     * \return The type of this ShaderObject as a human readable string
     */
    std::string typeAsString() const;

    /**
     * Returns the ShaderObject <code>type</code> as a human readable string.
     * \return The ShaderObject <code>type</code> as a human readable string.
     */
    static std::string stringForShaderType(ShaderType type);

private:
    /// The OpenGL name of this ShaderObject
    GLuint _id;

    /// The type of this ShaderObject
    ShaderType _type;

    /// The filename that is used to load the source for this ShaderObject;
    std::string _fileName;

    /// The internal name of this ShaderObject; initialized to empty
    std::string _shaderName;

    /// The logger category that will be used for logging of ShaderObject methods
    std::string _loggerCat;

	/// The callback function if any of the tracked files are changed.
	ShaderObjectCallback _onChangeCallback;

	/// Recursively #included files
	std::map<std::string, ghoul::filesystem::File*> _trackedFiles;

	/**
	 * Recursive file loading
	 */
	bool readFile(const std::string& filename, std::string& content, bool track = true);
};

} // namespace opengl
} // namespace ghoul

#endif
