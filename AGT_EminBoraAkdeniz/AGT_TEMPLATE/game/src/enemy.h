#pragma once
#include <engine.h>
#include "bounding_box/bounding_box.h"

class enemy
{
	enum class state
	{
		idle,
		on_guard,
		chasing,
		crashed
	};
public:
	enemy();
	~enemy();
	// set the parameters for the enemy
	void initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward);
	// update the enemy
	void on_update(const engine::timestep& time_step, const glm::vec3& player_position);

	// methods controlling the enemy’s behaviour in a certain state
	void patrol(const engine::timestep& time_step);
	void face_player(const engine::timestep& time_step, const glm::vec3& player_position);
	void chase_player(const engine::timestep& time_step, const glm::vec3& player_position);
	// game object bound to the enemy
	engine::ref<engine::game_object> object() const { return m_object; }

	void on_render(engine::ref<engine::shader> mesh);
	const bool getRender() { return s_render; }

protected:
	const state getState() { return m_state; }
	void setState_crashed(enemy enemy) { m_state = state::crashed; }

	
private:
	bool s_render = true;
	// enemy's speed
	float m_speed{ 3.f };
	// timer controlling the direction switch and the reset value for this timer
	float m_default_time{ 4.f };
	float m_switch_direction_timer = m_default_time;
	// threshold distances
	float m_detection_radius{ 15.f };
	float m_trigger_radius{ 3.f };
	// game object bound to the enemy
	engine::ref< engine::game_object> m_object;
	//current state of the enemy's state machine
	state m_state = state::idle;
};