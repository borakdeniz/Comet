#include "pch.h"
#include "menusystem.h"
#include "quad.h"

menusystem::menusystem(const std::string& path, float max_time, float width, float height)
{
	m_max_time = max_time;
	m_texture = engine::texture_2d::create(path, true);

	m_transparency = 0.0f;

	m_quad = quad::create(glm::vec2(width, height));
	s_active = false;
}

menusystem::~menusystem()
{}

void menusystem::on_update(const engine::timestep& time_step)
{
	if (!s_active)
		return;

	m_timer += (float)time_step;
}

void menusystem::cross_fade(const engine::timestep& time_step)
{
	if (!s_active)
		return;

	m_timer += (float)time_step;

	m_transparency = 0.8f - 0.8f * m_timer / m_max_time;

	if (m_timer > m_max_time)
		s_active = false;
}


void menusystem::on_render(engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;

	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(0.f, 0.f, 0.1f));

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", m_transparency);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.f);
}

void menusystem::activate()
{
	s_active = true;
	m_transparency = 1.0f;
	m_timer = 0.0f;
}

void menusystem::stop()
{
	s_active = false;
}

engine::ref<menusystem> menusystem::create(const std::string& path, float max_time, float width, float height)
{
	return std::make_shared<menusystem>(path, max_time, width, height);
}
