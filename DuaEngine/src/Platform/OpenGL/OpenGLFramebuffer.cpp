#include "duapch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Dua {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

    void OpenGLFramebuffer::Invalidate() 
    {
        // ����ԭʼ��״̬
        GLint prevFBO;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);

        // �������Դ
        if (m_RendererID) 
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
        }

        // ������FBO
        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        // ��ɫ����
        glGenTextures(1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // ��������ģʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        // ���ģ�帽��������Renderbuffer��
        GLuint rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        // �����Լ��
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) 
        {
            // ������Դ
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteRenderbuffers(1, &rbo);
            m_RendererID = 0;
            std::cout << "Framebuffer is incomplete: "<< status << std::endl;
        }

        // �ָ�ԭʼ��
        glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
    }
    
	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        Clear();
	}

	void OpenGLFramebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

    void OpenGLFramebuffer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // ͸�����
    }

}