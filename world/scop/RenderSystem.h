#pragma once

#include "DeferredRendering.h"

class Graphics;

class RenderSystem
{
public:
	RenderSystem(MapUtils* minfo);
	~RenderSystem();
	void setDeffGraphics(DeferredGraphics* defer_graphic);
	void Render(
		Mat const& cam_view,
		Mat const& cam_proj,
		vec3 const& cam_pos
	);

private:
	MapUtils* m_info;
	shared_ptr<DeferredRendering> r_deff;
};

