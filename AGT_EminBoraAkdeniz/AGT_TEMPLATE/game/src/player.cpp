#include "player.h"
#include "pch.h"
#include "glm/gtx/rotate_vector.hpp"
#include "engine/core/input.h"
#include "engine/key_codes.h"

player::player() : m_timer(0.0f), m_speed(8.f)
{
	;
}

player::~player()
{
}

void player::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, -1.f));
	m_object->set_position(glm::vec3(0.f, 0.5, 10.f));
	//m_object->animated_mesh()->set_default_animation(0);

}

void player::on_update(const engine::timestep& time_step)
{
	// acceleration added
	m_speed += m_acceleration * time_step / 2;
	m_object->set_position(m_object->position() += m_object->forward() * m_speed * (float)time_step);
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));

	// limit horizontal movement to set boundaries
	limitMovement();
	if (engine::input::key_pressed(engine::key_codes::KEY_A))			// strafe left
		strafe(-1.f);
	else if (engine::input::key_pressed(engine::key_codes::KEY_D))		// strafe right
		strafe(1.f);

}


void player::update_camera(engine::perspective_camera& camera)
{
	//set up the camera boom and the camera position
	float A = 1.5f;
	float B = 4.f;
	float C = 8.f;

	glm::vec3 cam_pos = m_object->position() - glm::normalize(m_object->forward()) * B;
	cam_pos.y += A;

	glm::vec3 cam_look_at = m_object->position() + glm::normalize(m_object->forward()) * C;
	cam_look_at.y = 0.f;
	camera.set_view_matrix(cam_pos, cam_look_at);
}

void player::limitMovement()
{
	float minHorizontal = -10.f, maxHorizontal = 10.f;
	glm::vec3 currentPosition = m_object->position();
	currentPosition.x = glm::clamp(currentPosition.x, minHorizontal, maxHorizontal);
	m_object->set_position(currentPosition);
}

void player::strafe(float direction)
{
	m_object->set_position(m_object->position() += m_object->right() * m_speed * 0.01f * direction);
}

glm::vec3 player::getPosition() const
{
	return m_object->position();
}