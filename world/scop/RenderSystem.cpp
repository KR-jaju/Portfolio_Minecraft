#include "pch.h"
#include "RenderSystem.h"

RenderSystem::RenderSystem(MapUtils* minfo) 
	: m_info(minfo)
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::setDeffGraphics(DeferredGraphics* defer_graphic)
{
	this->r_deff = make_shared<DeferredRendering>(
		this->m_info,
		defer_graphic
	);
}

void RenderSystem::Render(
	Mat const& cam_view, 
	Mat const& cam_proj,
	vec3 const& cam_pos
)
{
	this->r_deff->Render(cam_view, cam_proj, cam_pos);
}
