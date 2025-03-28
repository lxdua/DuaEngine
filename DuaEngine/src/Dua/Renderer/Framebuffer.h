#pragma once

#include "Dua/Core/Memory.h"

namespace Dua {

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0; 

	};



}