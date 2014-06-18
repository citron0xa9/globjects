#pragma once

#include <string>
#include <vector>

#include <glbinding/types.h>

#include <glow/glow_api.h>

#include <glowbase/Changeable.h>
#include <glowbase/ChangeListener.h>
#include <glowbase/ref_ptr.h>
#include <glowbase/AbstractStringSource.h>

#include <glow/Object.h>

namespace glow 
{

class Program;

/** \brief Encapsulates OpenGL shaders.
    
    A shader can be constructed using an AbstractStringSource.
    A shader can be attached to a program using
    Program::attach(). A Shader subclasses either ChangeListener and Changeable
    to react to changing shader sources and to propagate this change to 
    ChangeListeners.

    \see  http://www.opengl.org/wiki/Shader

    \see Program
    \see ShaderSource
    \see ChangeListener
    \see Changeable
 */
class GLOW_API Shader : public Object, protected ChangeListener, public Changeable
{
    friend class Program;

public:
    static Shader * fromString(const gl::GLenum type, const std::string & sourceString);
    static Shader * fromFile(const gl::GLenum type, const std::string & filename);

public:
    Shader(const gl::GLenum type);
    Shader(const gl::GLenum type, AbstractStringSource * source);
    Shader(const gl::GLenum type, AbstractStringSource * source, const std::vector<std::string> & includePaths);

    virtual void accept(ObjectVisitor& visitor) override;

	gl::GLenum type() const;

    void setSource(AbstractStringSource * source);
	void setSource(const std::string & source);
    const AbstractStringSource* source() const;
    void updateSource();
    void setIncludePaths(const std::vector<std::string> & includePaths);

    bool compile() const;
	bool isCompiled() const;
    void invalidate();

    gl::GLint get(gl::GLenum pname) const;
    std::string getSource() const;
    bool checkCompileStatus() const;
	std::string infoLog() const;

    std::string typeString() const;

    static std::string typeString(gl::GLenum type);

protected:
    virtual ~Shader();

    virtual void notifyChanged(const Changeable * changebale) override;
protected:
    static gl::GLuint create(gl::GLenum type);
    static void setSource(const Shader & shader, const std::string & source);

    std::string shaderString() const;

protected:
	gl::GLenum m_type;
    ref_ptr<AbstractStringSource> m_source;
    std::vector<std::string> m_includePaths;

    mutable bool m_compiled;
    mutable bool m_compilationFailed;

public:
    static bool forceFallbackIncludeProcessor;
};

} // namespace glow
