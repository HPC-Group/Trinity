//
//  ShaderDescriptor.h
//  TNG
//
//  Created by Andre Waschk on 10/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef __TNG__ShaderDescriptor__
#define __TNG__ShaderDescriptor__

#include <string>
#include <vector>
#include <memory>
#ifdef _WIN32
#include <stdint.h>
#endif
namespace Tuvok{
    namespace Renderer{
        
      /// A common way to identify a shader.
      /// We need to know whether a shader is equivalent.  When we're asked to load a
      /// new shader, we search through a list of cached ones and see if it already
      /// exists.  This allows us to avoid loading it multiple times, and more
      /// importantly compiling it multiple times.
      /// This is made a bit difficult by how we build shaders: they can come from
      /// files or strings.
      class ShaderDescriptor {
      public:
        ShaderDescriptor();
        /// Constructs the descriptor from a list of FILES for each shader type.
        ShaderDescriptor(const std::vector<std::string>& vertex,
                         const std::vector<std::string>& fragment);
        ShaderDescriptor(const ShaderDescriptor&);
        
        /// @param dir list of paths
        /// @return the list of paths which exist.
        static std::vector<std::string> ValidPaths(std::vector<std::string> dir);
        
        /// Takes a list of directories and *two* lists of shaders.  Both lists
        /// must be terminated with a null.  The first is a list of filenames for
        /// the vertex shaders, the second is a list of filenames for fragment
        /// shaders.
        static ShaderDescriptor Create(
                                       std::vector<std::string> directories, ...
                                       );
        
        static ShaderDescriptor Create(
                                       std::vector<std::string> directories,
                                       std::vector<std::pair<uint32_t, std::string>> fragmentDataBindings,
                                       ...
                                       );
        
        /// Adds a global string to the shader that is used for every compilation unit
        void AddDefine(const std::string& define);
        void AddDefines(const std::vector<std::string>& defines);
        
        /// Adds a vertex shader in a string (i.e. not from a filename)
        void AddVertexShaderString(const std::string shader);
        /// Adds a fragment shader in a string (i.e. not from a filename)
        void AddFragmentShaderString(const std::string shader);
        
        /// Two shaders are equal if they utilize the same set of filenames/strings
        /// to compose the shader.
        bool operator ==(const ShaderDescriptor& sd) const;
        
        /// Shader iterator.  When dereferenced, produces a pair of 'program text'
        /// (first) and the source of that program text (second).  The latter is
        /// only intended for diagnostics, and may be empty.
        struct SIterator : public std::iterator<std::input_iterator_tag,
        std::string> {
          SIterator(const SIterator&);
          SIterator& operator++();
          SIterator& operator++(int);
          bool operator==(const SIterator&) const;
          bool operator!=(const SIterator&) const;
          std::pair<std::string, std::string> operator*() const;
        private:
          struct siterinfo;
          std::shared_ptr<siterinfo> si;
          SIterator(struct siterinfo);
          friend class ShaderDescriptor;
        };
        /// Iterate over the list of shaders.
        SIterator begin_vertex() const;
        SIterator end_vertex() const;
        SIterator begin_fragment() const;
        SIterator end_fragment() const;
        
        std::vector<std::pair<uint32_t, std::string>> fragmentDataBindings;
        
        std::string getFragmentSource() const;
        std::string getVertexSource() const;

		uint32_t GetFragmentElementSize();
		uint32_t GetVertexElementSize();
		std::string GetFragmentSource(const uint32_t index);
		std::string GetVertexSource(const uint32_t index);

      private:
        struct sinfo;
        std::shared_ptr<sinfo> si;
      };
    };
};

#endif /* defined(__TNG__ShaderDescriptor__) */
