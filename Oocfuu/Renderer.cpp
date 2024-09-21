#include "Renderer.h"
#include <gl/glut.h>

Renderer::Renderer()
	: m_clearColor(1.0f)
{
}

Renderer::~Renderer()
{
}

void Renderer::draw()
{
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);

	glDisable(GL_BLEND);
}
