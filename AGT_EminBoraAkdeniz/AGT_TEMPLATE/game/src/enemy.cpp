#include "pch.h"
#include "enemy.h"

enemy::enemy() {}

enemy::~enemy() {}

void enemy::initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward)
{
	m_object = object;
	m_object->set_forward(forward);
	m_object->set_position(position);
	m_state = state::idle;
}

void enemy::on_update(const engine::timestep& time_step, const glm::vec3& player_position)
{
	float distance_to_player = glm::distance(m_object->position(), player_position);

	// check which state is the enemy in, then execute the matching behaviour
	if (m_state == state::idle)
	{
		patrol(time_step);

		// check whether the condition has been met to switch to the on_guard state
		if (distance_to_player < m_detection_radius)
			m_state = state::on_guard;
	}
	else if (m_state == state::on_guard)
	{
		face_player(time_step, player_position);

		// check whether the condition has been met to switch back to idle state
		if (distance_to_player > m_detection_radius)
			m_state = state::idle;
		// check whether the condition has been met to switch to the chasing state
		else if (distance_to_player < m_trigger_radius)
			m_state = state::chasing;
	}
	else if (m_state == state::chasing)
	{
		m_speed = m_speed * 1.3f;
		chase_player(time_step, player_position);

		// check whether the condition has been met to switch back to idle state
		if (distance_to_player > m_detection_radius)
			m_state = state::idle;

		else if (distance_to_player < 8.f)
		{
			m_state = state::crashed;
		}
	}
	// check whether the condition has been met and set the flag variable for rendering the model
	else if (m_state == state::crashed)
	{
		s_render = false;
	}

}

// move forwards until the timer runs out, then switch direction to move the other way
void enemy::patrol(const engine::timestep& time_step)
{

	m_object->set_position(m_object->position() - m_object->forward() * m_speed * (float)time_step);
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}

// stop and look at the player
void enemy::face_player(const engine::timestep& time_step, const glm::vec3& player_position)
{
	m_object->set_forward(-(player_position - m_object->position()));
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}

// move forwards in the direction of the player
void enemy::chase_player(const engine::timestep& time_step, const glm::vec3& player_position)
{
	m_object->set_forward(player_position - m_object->position());
	m_object->set_position(m_object->position() + m_object->forward() * m_speed * (float)time_step);
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}

void enemy::on_render(engine::ref<engine::shader> mesh)
{
	if (s_render)
	{
		engine::renderer::submit(mesh, m_object);
	}
}

