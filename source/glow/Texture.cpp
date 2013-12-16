
#include <algorithm>

#include <glow/Error.h>
#include <glow/logging.h>
#include <glow/ObjectVisitor.h>

#include <glow/Texture.h>

namespace glow
{

Texture::Texture(GLenum  target)
: Object(genTexture())
, m_target(target)
{
}

Texture::Texture(GLuint id, GLenum  target, bool ownsGLObject)
: Object(id, ownsGLObject)
, m_target(target)
{
}

Texture::~Texture()
{
	if (ownsGLObject())
	{
		glDeleteTextures(1, &m_id);
		CheckGLError();
	}
}

void Texture::bind() const
{
    glBindTexture(m_target, m_id);
	CheckGLError();
}

void Texture::unbind() const
{
    glBindTexture(m_target, 0);
	CheckGLError();
}

void Texture::unbind(const GLenum target)
{
    glBindTexture(target, 0);
    CheckGLError();
}

void Texture::bindActive(const GLenum texture) const
{
    glActiveTexture(texture);
    glBindTexture(m_target, m_id);
    CheckGLError();
}

void Texture::unbindActive(const GLenum texture) const
{
    glActiveTexture(texture);
    glBindTexture(m_target, 0);
    CheckGLError();
}

GLenum Texture::target() const
{
    return m_target;
}

void Texture::setParameter(GLenum name, GLint value)
{
	bind();

    glTexParameteri(m_target, name, value);
	CheckGLError();
}

void Texture::setParameter(GLenum name, GLfloat value)
{
	bind();

    glTexParameterf(m_target, name, value);
	CheckGLError();
}

GLint Texture::getParameter(GLenum pname)
{
	bind();

	GLint value = 0;

    glGetTexParameteriv(m_target, pname, &value);
	CheckGLError();

	return value;
}

GLint Texture::getLevelParameter(GLint level, GLenum pname)
{
	bind();

	GLint value = 0;

    glGetTexLevelParameteriv(m_target, level, pname, &value);
	CheckGLError();

	return value;
}

void Texture::image2D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
	bind();

    glTexImage2D(m_target, level, internalFormat, width, height, border, format, type, data);
	CheckGLError();
}

void Texture::image3D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
    bind();

    glTexImage3D(m_target, level, internalFormat, width, height, depth, border, format, type, data);
    CheckGLError();
}


void Texture::storage2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
{
	bind();

    glTexStorage2D(m_target, levels, internalFormat, width, height);
	CheckGLError();
}


void Texture::bindImageTexture(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
{
	bind();

	glBindImageTexture(unit, m_id, level, layered, layer, access, format);
	CheckGLError();
}

void Texture::generateMipmap()
{
	bind();

    glGenerateMipmap(m_target);
	CheckGLError();
}

GLuint Texture::genTexture()
{
	GLuint id = 0;

	glGenTextures(1, &id);
	CheckGLError();

	return id;
}

void Texture::accept(ObjectVisitor& visitor)
{
	visitor.visitTexture(this);
}

TextureHandle Texture::textureHandle() const
{
    TextureHandle result = glGetTextureHandleARB(m_id);
	CheckGLError();
	return result;
}

GLboolean Texture::isResident() const
{
    bool result = glIsTextureHandleResidentARB(textureHandle()) == GL_TRUE;
	CheckGLError();

	return result;
}

TextureHandle Texture::makeResident()
{
    TextureHandle handle = textureHandle();

    glMakeTextureHandleResidentARB(handle);
	CheckGLError();

	return handle;
}

void Texture::makeNonResident()
{
    glMakeTextureHandleNonResidentARB(textureHandle());
	CheckGLError();
}

} // namespace glow
