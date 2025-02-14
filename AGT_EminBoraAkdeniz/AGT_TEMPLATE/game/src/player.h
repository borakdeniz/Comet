#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"


class player
{
public:
	player();
	~player();

	void initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step);

	engine::ref<engine::game_object> object() const { return m_object; }

	void update_camera(engine::perspective_camera& camera);

	void limitMovement();
	void strafe(float direction);
	glm::vec3 getPosition() const;


private:
	// somewhat game physics 
	float m_speed{ 8.f };
	float m_timer;
	float m_acceleration{ 0.5f };

	engine::ref< engine::game_object> m_object;
};
