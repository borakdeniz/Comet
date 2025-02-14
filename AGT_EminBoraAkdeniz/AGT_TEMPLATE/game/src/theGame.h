#pragma once
#include <engine.h>
#include "player.h"
#include "MenuSystem.h"
#include "engine/entities/shapes/octahedron.h"
#include "menusystem.h"
#include <bounding_box/bounding_box.h>
#include "engine/entities/shapes/heightmap.h"
#include "shield_sphere_pickup.h"
#include "platform/opengl/gl_shader.h"
#include "enemy.h"

class pickup;

class theGame : public engine::layer, public player, public enemy
{
public:
	theGame();
	~theGame();

    void on_update(const engine::timestep& time_step) ;
    void on_render() ; 
    void on_event(engine::event& event) ;

private:
	void check_bounce();

	void updateSpotlightPosition();

	void wait(int number_of_seconds);

	// getter and setter for player attributes
	int health = 200;
	int score = 0;
	bool s_invincible = false;

	// in-game values to be modified on_update
	int									timer = 0;
	int									tick = 0;

	// Initialize game objects
	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_ball{};
	engine::ref<engine::game_object>	m_tetrahedron{};
	engine::ref<engine::game_object>	m_octahedron{};

	engine::ref<engine::game_object>	m_ship{};
	engine::ref<engine::game_object>	m_shield_pickup_0{};
	engine::ref<engine::game_object>	m_shield_pickup_1{};
	engine::ref<engine::game_object>	m_shield_pickup_2{};
	engine::ref<engine::game_object>	m_shield_pickup_3{};

	engine::ref<engine::game_object>	m_enemy_ship_0{};
	engine::ref<engine::game_object>	m_enemy_ship_1{};
	engine::ref<engine::game_object>	m_enemy_ship_2{};
	engine::ref<engine::game_object>	m_enemy_ship_3{};
	engine::ref<engine::game_object>	m_enemy_ship_4{};
	engine::ref<engine::game_object>	m_enemy_ship_5{};

	// Initialize player
	player								m_player{};

	// Initialize enemy ai
	enemy 								m_enemy_0{};
	enemy 								m_enemy_1{};
	enemy 								m_enemy_2{};
	enemy 								m_enemy_3{};
	enemy 								m_enemy_4{};
	enemy 								m_enemy_5{};

	// Initialize fog
	bool								m_fog = true;

	engine::ref<engine::material>		m_material{};
	engine::ref<engine::material>		m_lightsource_material{};
	engine::ref<engine::material>		m_ship_material{};
	engine::ref<engine::material>		m_enemy_ship_material{};
	engine::ref<engine::material>		m_mannequin_material{};
	engine::ref<engine::material>		m_tetrahedron_material{};
	engine::ref<engine::material>		m_octahedron_material{};

	// Initialize lighting components
	engine::DirectionalLight            m_directionalLight;
	engine::PointLight m_pointLight;
	uint32_t    num_point_lights = 1;

	engine::SpotLight					m_spotLight;
	uint32_t							num_spot_lights = 1;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;

	// Initialize Menu System
	engine::ref<menusystem>				m_introscreen{};
	engine::ref<menusystem>				m_gameover{};
	engine::ref<menusystem>				m_cross_fade{};

	bool								game_start = false;
	bool								game_over = false;

	// Initialize Bounding boxes and related values
	engine::bounding_box				m_player_box;

	engine::bounding_box				m_shield_pickup_box_0;
	engine::bounding_box				m_shield_pickup_box_1;
	engine::bounding_box				m_shield_pickup_box_2;
	engine::bounding_box				m_shield_pickup_box_3;

	engine::bounding_box				m_enemy_box_0;
	engine::bounding_box				m_enemy_box_1;
	engine::bounding_box				m_enemy_box_2;
	engine::bounding_box				m_enemy_box_3;
	engine::bounding_box				m_enemy_box_4;
	engine::bounding_box				m_enemy_box_5;


	// Initialize HUD values
	engine::ref<engine::text_manager>	m_text_manager{};
	int									m_score = 0;
	std::string							text = "Score:";

    engine::orthographic_camera			m_2d_camera; 
    engine::perspective_camera			m_3d_camera;

	engine::ref<pickup>					m_pickup{};

	engine::ref<shield_sphere_pickup>	m_shield_sphere_pickup{};

};

