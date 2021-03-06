#ifndef _RENDERABLE_VERTEX
#define _RENDERABLE_VERTEX

#pragma once

#include <assert.h>
#include "Render\RenderManager.h"

class CEffectTechnique;

class CRenderableVertexs
{
	public:
		virtual bool Render(CRenderManager *RenderManager, CEffectTechnique *EffectTechnique, void *Parameters)
		{
			 assert(!"This method mustn't be called");
			 return false;
		}
		virtual bool RenderIndexed(CRenderManager *RenderManager, CEffectTechnique * EffectTechnique, void *Parameters,
			unsigned int IndexCount=-1, unsigned int StartIndexLocation=0, unsigned int BaseVertexLocation=0)
		{
			 assert(!"This method mustn't be called");
			 return false;
		}
};

#endif