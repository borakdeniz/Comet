
#include "theGame.h"

#include "pickup.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include "engine/entities/game_object.h"



theGame::theGame()
	:m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f),
	m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())

{
	// Hide the mouse and lock it inside the window
	//engine::input::anchor_mouse(true);
	engine::application::window().hide_mouse_cursor();

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/submit_button_click.wav", engine::sound_type::event, "space_click");		// Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/crash.wav", engine::sound_type::event, "crash");							// Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/bounce.wav", engine::sound_type::spatialised, "bounce");					// Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/pick_up.wav", engine::sound_type::event, "pickup");						// Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/DST-Roadway.mp3", engine::sound_type::track, "theme");					// Royalty free music from http://www.nosoapradio.us/
	m_audio_manager->load_sound("assets/audio/DST-StardustMemory.mp3", engine::sound_type::track, "intro");				// Royalty free music from http://www.nosoapradio.us/
	m_audio_manager->play("intro");

	// Initialise the shaders, materials and lights
	auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	// Directional light
	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.25f;
	m_directionalLight.DiffuseIntensity = 0.6f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

	// Spot light
	m_spotLight.Color = glm::vec3(7.0f, 3.0f, 10.0f);
	m_spotLight.AmbientIntensity = 1.f;
	m_spotLight.DiffuseIntensity = 0.6f;
	m_spotLight.Position = glm::vec3(0.f, 5.f, 0.f);
	m_spotLight.Direction = glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f));
	m_spotLight.Cutoff = 0.5f;
	m_spotLight.Attenuation.Constant = 1.0f;
	m_spotLight.Attenuation.Linear = 0.1f;
	m_spotLight.Attenuation.Exp = 0.01f;

	// Point Light
	m_pointLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_pointLight.AmbientIntensity = 0.2f;
	m_pointLight.DiffuseIntensity = 0.6f;
	m_pointLight.Position = glm::vec3(0.f, 1.f, 0.f);

	// Initialise fog component
	m_fog = false;
	glm::vec3 m_fog_colour = glm::vec3(0.1f);
	int m_fog_factor_type = 0.00001f;

	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gMatSpecularIntensity", 0.1f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("transparency", 0.1f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gMatSpecularIntensity", 0.1f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gSpecularPower", 10.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection", glm::ortho(0.f, (float)engine::application::window().width(), 0.f, (float)engine::application::window().height()));

	// Order and meaning of the material parameters
	// Diffuse color (RGB)
	// Specular color (RGB)
	// Ambient color (RGB)
	// Shininess

	m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	m_lightsource_material = engine::material::create(1.f, m_pointLight.Color, m_pointLight.Color, m_pointLight.Color, 1.0f);

	m_ship_material = engine::material::create(1.0f, glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	m_enemy_ship_material = engine::material::create(1.0f, glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	m_tetrahedron_material = engine::material::create(32.0f,
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		0.3f);

	m_octahedron_material = engine::material::create(32.0f,
		glm::vec3(0.0f, 0.5f, 1.0f),	// Diffuse color (RGB)
		glm::vec3(0.0f, 0.5f, 1.0f),	// Specular color (RGB)
		glm::vec3(0.5f, 0.5f, 0.5f),	// Ambient color (RGB)
		0.3f);							// Shininess

	// Skybox texture from http://www.vwall.it/wp-content/plugins/canvasio3dpro/inc/resource/cubeMaps/
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/NewSkyBox/game_top.jpg", true),
		  engine::texture_2d::create("assets/textures/NewSkyBox/game_top.jpg", true),
		  engine::texture_2d::create("assets/textures/NewSkyBox/game_top.jpg", true),
		  engine::texture_2d::create("assets/textures/NewSkyBox/game_top.jpg", true),
		  engine::texture_2d::create("assets/textures/NewSkyBox/game_top.jpg", true),
		  engine::texture_2d::create("assets/textures/NewSkyBox/game_top.jpg", true)
		});


	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/NewSkyBox/game_bottom.jpg", true) };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(100.f, 0.5f, 600.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.friction = 0.0f;
	terrain_props.bounding_shape = glm::vec3(100.f, 0.5f, 500.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	// Load the Ship model. Create a Ship object. Set its properties
	// taken from https://free3d.com/3d-model/justice-league-flying-vehicle-14220.html
	engine::ref <engine::model> ship_model = engine::model::create("assets/models/static/justigue league flying vehicle.obj");
	std::vector<engine::ref<engine::texture_2d>> ship_texture = { engine::texture_2d::create("assets/textures/Maps/zqw1b.jpg", false) };
	engine::game_object_properties ship_props;
	ship_props.meshes = ship_model->meshes();
	ship_props.textures = ship_texture;
	float ship_scale = 0.008;
	ship_props.position = { -5.f,0.5f, 0.f };
	ship_props.scale = glm::vec3(ship_scale);
	ship_props.bounding_shape = ship_model->size() / 2.f * ship_scale;
	m_ship = engine::game_object::create(ship_props);
	m_player.initialise(m_ship);

	// set collision box for player
	m_player_box.set_box(ship_props.bounding_shape.x * 1.8,
		ship_props.bounding_shape.y * 5,
		ship_props.bounding_shape.z * 1.5,
		ship_props.position);


	//-----------------------------------------------DEFINE ENEMY SHIPS------------------------------------------//
	// Load the Enemy Ship model. Create a Enemy Ship object. Set its properties
	// taken from https://free3d.com/3d-model/kameri-explorer-57154.html
	engine::ref <engine::model> enemy_ship_model = engine::model::create("assets/models/static/kameri explorer flying.obj");
	std::vector<engine::ref<engine::texture_2d>> enemy_ship_texture = { engine::texture_2d::create("assets/textures/Maps/dcvx1d.jpg", false) };
	engine::game_object_properties enemy_ship_props;
	enemy_ship_props.meshes = enemy_ship_model->meshes();
	enemy_ship_props.textures = enemy_ship_texture;
	float enemy_ship_scale = 0.015;
	enemy_ship_props.position = { -5.f,0.5f, -50.f };
	enemy_ship_props.scale = glm::vec3(enemy_ship_scale);
	enemy_ship_props.bounding_shape = enemy_ship_model->size() / 2.f * enemy_ship_scale;
	m_enemy_ship_0 = engine::game_object::create(enemy_ship_props);

	// set collision box for enemy
	m_enemy_box_0.set_box(enemy_ship_props.bounding_shape.x * 1.8,
		enemy_ship_props.bounding_shape.y * 5,
		enemy_ship_props.bounding_shape.z * 1.5,
		enemy_ship_props.position);

	m_enemy_0.initialise(m_enemy_ship_0, enemy_ship_props.position, glm::vec3(0.f, 0.f, -1.f));

	enemy_ship_props.position = { 5.f,0.5f, -150.f };
	m_enemy_ship_1 = engine::game_object::create(enemy_ship_props);
	// set collision box for enemy
	m_enemy_box_1.set_box(enemy_ship_props.bounding_shape.x * 1.8,
		enemy_ship_props.bounding_shape.y * 5,
		enemy_ship_props.bounding_shape.z * 1.5,
		enemy_ship_props.position);

	m_enemy_1.initialise(m_enemy_ship_1, enemy_ship_props.position, glm::vec3(0.f, 0.f, -1.f));

	enemy_ship_props.position = { -5.f,0.5f, -250.f };
	m_enemy_ship_2 = engine::game_object::create(enemy_ship_props);
	// set collision box for enemy
	m_enemy_box_2.set_box(enemy_ship_props.bounding_shape.x * 1.8,
		enemy_ship_props.bounding_shape.y * 5,
		enemy_ship_props.bounding_shape.z * 1.5,
		enemy_ship_props.position);

	m_enemy_2.initialise(m_enemy_ship_2, enemy_ship_props.position, glm::vec3(0.f, 0.f, -1.f));

	enemy_ship_props.position = { 5.f,0.5f, -350.f };
	m_enemy_ship_3 = engine::game_object::create(enemy_ship_props);
	// set collision box for enemy
	m_enemy_box_3.set_box(enemy_ship_props.bounding_shape.x * 1.8,
		enemy_ship_props.bounding_shape.y * 5,
		enemy_ship_props.bounding_shape.z * 1.5,
		enemy_ship_props.position);

	m_enemy_3.initialise(m_enemy_ship_3, enemy_ship_props.position, glm::vec3(0.f, 0.f, -1.f));

	enemy_ship_props.position = { -5.f,0.5f, -450.f };
	m_enemy_ship_4 = engine::game_object::create(enemy_ship_props);
	// set collision box for enemy
	m_enemy_box_4.set_box(enemy_ship_props.bounding_shape.x * 1.8,
		enemy_ship_props.bounding_shape.y * 5,
		enemy_ship_props.bounding_shape.z * 1.5,
		enemy_ship_props.position);

	m_enemy_4.initialise(m_enemy_ship_4, enemy_ship_props.position, glm::vec3(0.f, 0.f, -1.f));

	enemy_ship_props.position = { 5.f,0.5f, -550.f };
	m_enemy_ship_5 = engine::game_object::create(enemy_ship_props);
	// set collision box for enemy
	m_enemy_box_5.set_box(enemy_ship_props.bounding_shape.x * 1.8,
		enemy_ship_props.bounding_shape.y * 5,
		enemy_ship_props.bounding_shape.z * 1.5,
		enemy_ship_props.position);

	m_enemy_5.initialise(m_enemy_ship_5, enemy_ship_props.position, glm::vec3(0.f, 0.f, -1.f));

	//-------------------------------------END OF ENEMY SHIPS-------------------------------------//

	//-------------------------------------DEFINE PICK UPS----------------------------------------//
	//Load the star model. Create a star object. Set its properties
	engine::ref <engine::model> shield_pickup_model = engine::model::create("assets/models/static/star.3ds");
	engine::game_object_properties shield_pickup_props;
	shield_pickup_props.meshes = shield_pickup_model->meshes();
	shield_pickup_props.textures = shield_pickup_model->textures();
	float star_scale = 1.f / glm::max(shield_pickup_model->size().x, glm::max(shield_pickup_model->size().y, shield_pickup_model->size().z));
	shield_pickup_props.position = { 0.f, 1.f, -100.f };
	shield_pickup_props.bounding_shape = shield_pickup_model->size() / 2.f;
	shield_pickup_props.scale = glm::vec3(star_scale);
	m_shield_pickup_0 = engine::game_object::create(shield_pickup_props);

	m_shield_pickup_box_0.set_box(shield_pickup_props.bounding_shape.x* star_scale, 
		shield_pickup_props.bounding_shape.y
		* star_scale, shield_pickup_props.bounding_shape.z* star_scale, 
		shield_pickup_props.position);

	shield_pickup_props.position = { 0.f, 1.f, -200.f };
	m_shield_pickup_1 = engine::game_object::create(shield_pickup_props);

	m_shield_pickup_box_1.set_box(shield_pickup_props.bounding_shape.x* star_scale,
		shield_pickup_props.bounding_shape.y
		* star_scale, shield_pickup_props.bounding_shape.z* star_scale,
		shield_pickup_props.position);

	shield_pickup_props.position = { 0.f, 1.f, -300.f };
	m_shield_pickup_2 = engine::game_object::create(shield_pickup_props);

	m_shield_pickup_box_2.set_box(shield_pickup_props.bounding_shape.x* star_scale,
		shield_pickup_props.bounding_shape.y
		* star_scale, shield_pickup_props.bounding_shape.z* star_scale,
		shield_pickup_props.position);

	shield_pickup_props.position = { 0.f, 1.f, -400.f };
	m_shield_pickup_3 = engine::game_object::create(shield_pickup_props);

	m_shield_pickup_box_3.set_box(shield_pickup_props.bounding_shape.x* star_scale,
		shield_pickup_props.bounding_shape.y
		* star_scale, shield_pickup_props.bounding_shape.z* star_scale,
		shield_pickup_props.position);

	//-------------------------------------END OF PICK UPS----------------------------------------//

	// Sphere
	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(10, 20, 0.5f);
	engine::game_object_properties sphere_props;
	sphere_props.position = { -15.f, 2.5f, -20.f };
	sphere_props.meshes = { sphere_shape->mesh() };
	sphere_props.mass = 0.000001f;
	std::vector<engine::ref<engine::texture_2d>> sphere_texture = { engine::texture_2d::create("assets/textures/astroid.bmp", false) };
	sphere_props.textures = sphere_texture;
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.5f);
	sphere_props.restitution = 0.92f;
	m_ball = engine::game_object::create(sphere_props);

	// Tetrahedron
	std::vector<glm::vec3> tetrahedron_vertices;
	tetrahedron_vertices.push_back(glm::vec3(0.f, 10.f, 0.f));		//0
	tetrahedron_vertices.push_back(glm::vec3(0.f, 0.f, 10.f));		//1
	tetrahedron_vertices.push_back(glm::vec3(-10.f, 0.f, -10.f));	//2
	tetrahedron_vertices.push_back(glm::vec3(10.f, 0.f, -10.f));	//3

	engine::ref<engine::tetrahedron> tetrahedron_shape = engine::tetrahedron::create(tetrahedron_vertices);
	engine::game_object_properties tetrahedron_props;
	tetrahedron_props.position = { 20.f, 0.f, -10.f };
	tetrahedron_props.meshes = { tetrahedron_shape->mesh() };
	std::vector<engine::ref<engine::texture_2d>> tetrahedron_textures = { engine::texture_2d::create("assets/textures/NewSkyBox/game_bottom.jpg", false) };
	tetrahedron_props.textures = tetrahedron_textures;
	tetrahedron_props.scale = glm::vec3(1.f);
	m_tetrahedron = engine::game_object::create(tetrahedron_props);

	// Octahedron
	std::vector<glm::vec3> octahedron_vertices;
	octahedron_vertices.push_back(glm::vec3(0.f, 10.f, 0.f));		//0
	octahedron_vertices.push_back(glm::vec3(-10.f, 0.f, -10.f));	//1
	octahedron_vertices.push_back(glm::vec3(10.f, 0.f, -10.f));		//2
	octahedron_vertices.push_back(glm::vec3(10.f, 0.f, 10.f));		//3
	octahedron_vertices.push_back(glm::vec3(-10.f, 0.f, 10.f));		//4
	octahedron_vertices.push_back(glm::vec3(0.f, -10.f, 0.f));		//5

	engine::ref<engine::octahedron> octahedron_shape = engine::octahedron::create(octahedron_vertices);
	engine::game_object_properties octahedron_props;
	octahedron_props.position = { 20.f, 10.f, -10.f };
	octahedron_props.meshes = { octahedron_shape->mesh() };
	std::vector<engine::ref<engine::texture_2d>> octahedron_textures = { engine::texture_2d::create("assets/textures/PurpleSlime.bmp", false) };
	octahedron_props.textures = octahedron_textures;
	octahedron_props.scale = glm::vec3(0.2);
	m_octahedron = engine::game_object::create(octahedron_props);

	m_game_objects.push_back(m_terrain);
	m_game_objects.push_back(m_ball);
	//m_game_objects.push_back(m_cow);
	//m_game_objects.push_back(m_tree);
	//m_game_objects.push_back(m_pickup);
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();

	// Adding a pick up
	engine::ref<engine::cuboid> pickup_shape = engine::cuboid::create(glm::vec3(0.5f), false);
	engine::ref<engine::texture_2d> pickup_texture = { engine::texture_2d::create("assets/textures/asteroid_start.bmp", false) };
	engine::game_object_properties pickup_props;
	pickup_props.position = { 5.f, 1.f, 0.f };
	pickup_props.meshes = { pickup_shape->mesh() };
	pickup_props.textures = { pickup_texture };
	m_pickup = pickup::create(pickup_props);
	m_pickup->init();

	// Initialize intro and outro screens base template https://image3.uhdpaper.com/wallpaper-hd/digital-art-retrowave-spaceship-sci-fi-uhdpaper.com-hd-80.jpg
	m_introscreen = menusystem::create("assets/textures/comet-start.jpg", 2.0f, 1.6f, 0.9f);

	m_gameover = menusystem::create("assets/textures/comet-gameover.jpg", 2.0f, 1.6f, 0.9f);

	m_cross_fade = menusystem::create("assets/textures/Red.bmp", 2.0f, 1.6f, 0.9f);

	m_shield_sphere_pickup = shield_sphere_pickup::create(glm::vec3(1.f, 0.f, 1.0f), glm::vec3(0.f, 0.f, 1.f), true, 0.25f, 3.0f);

	m_introscreen->activate();
}

theGame::~theGame() {}

void theGame::on_update(const engine::timestep& time_step)
{	

	if (game_start && !game_over)
	{
		tick = tick + 1;

		m_introscreen->stop();

		m_cross_fade->cross_fade(time_step);

		m_player.update_camera(m_3d_camera);

		m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

		// allocate the position of the sphere real time
		m_shield_sphere_pickup->on_update(time_step);

		m_shield_sphere_pickup->set_position(m_player.getPosition());

		// make spotlight follow ship 
		updateSpotlightPosition();

		m_player.on_update(time_step);

		m_audio_manager->update_with_camera(m_3d_camera);

		m_pickup->update(m_3d_camera.position(), time_step);

		//Set up player ship
		m_player.on_update(time_step);

		m_player.update_camera(m_3d_camera);

		// Set up AI ships
		m_enemy_0.on_update(time_step, m_player.object()->position());

		m_enemy_0.patrol(time_step);

		m_enemy_1.on_update(time_step, m_player.object()->position());

		m_enemy_1.patrol(time_step);

		m_enemy_2.on_update(time_step, m_player.object()->position());

		m_enemy_2.patrol(time_step);

		m_enemy_3.on_update(time_step, m_player.object()->position());

		m_enemy_3.patrol(time_step);

		m_enemy_4.on_update(time_step, m_player.object()->position());

		m_enemy_4.patrol(time_step);

		m_enemy_5.on_update(time_step, m_player.object()->position());

		m_enemy_5.patrol(time_step);

		// Player collision box
		m_player_box.on_update(m_player.object()->position());

		// Enemy collision boxes
		m_enemy_box_0.on_update(m_enemy_0.object()->position());
		m_enemy_box_1.on_update(m_enemy_1.object()->position());
		m_enemy_box_2.on_update(m_enemy_2.object()->position());
		m_enemy_box_3.on_update(m_enemy_3.object()->position());
		m_enemy_box_4.on_update(m_enemy_4.object()->position());
		m_enemy_box_5.on_update(m_enemy_5.object()->position());

		if (m_shield_pickup_box_0.collision(m_player_box))
		{
			m_fog = false;
			m_score += 1000;
			m_shield_sphere_pickup->activate(1.f, m_player.getPosition());
			s_invincible = true;
			m_audio_manager->play("pickup");
		}
		if (m_shield_pickup_box_1.collision(m_player_box))
		{
			m_fog = false;
			m_score += 1000;
			m_shield_sphere_pickup->activate(1.f, m_player.getPosition());
			s_invincible = true;
			m_audio_manager->play("pickup");
		}
		if (m_shield_pickup_box_2.collision(m_player_box))
		{
			m_fog = false;
			m_score += 1000;
			m_shield_sphere_pickup->activate(1.f, m_player.getPosition());
			s_invincible = true;
			m_audio_manager->play("pickup");
		}
		if (m_shield_pickup_box_3.collision(m_player_box))
		{
			m_fog = false;
			m_score += 1000;
			m_shield_sphere_pickup->activate(1.f, m_player.getPosition());
			s_invincible = true;
			m_audio_manager->play("pickup");
		}
		// check collisions, play a crash sound on contact and decrease health
		if (m_enemy_box_0.collision(m_player_box))
		{
			if (!s_invincible)
			{
				//m_player.object()->set_position(pos);
				health -= 10;
				m_cross_fade->activate();
				m_audio_manager->play("crash");
				enemy::setState_crashed(m_enemy_0);
			}
			else
			{
				score += 1000;
			}
		}

		if (m_enemy_box_1.collision(m_player_box))
		{
			if (!m_shield_sphere_pickup->get_s_active())
			{
				s_invincible = false;
				if (!s_invincible)
				{

					//m_player.object()->set_position(pos);
					health -= 10;
					m_cross_fade->activate();
					m_audio_manager->play("crash");
					enemy::setState_crashed(m_enemy_1);
				}
			}
			else
			{
				score += 1000;
			}
		}

		if (m_enemy_box_2.collision(m_player_box))
		{
			if (!m_shield_sphere_pickup->get_s_active())
			{
				s_invincible = false;
				if (!s_invincible)
				{
					s_invincible = false;
					//m_player.object()->set_position(pos);
					health -= 10;
					m_cross_fade->activate();
					m_audio_manager->play("crash");
					enemy::setState_crashed(m_enemy_2);
				}
			}
			else
			{
				score += 1000;
			}
		}

		if (m_enemy_box_3.collision(m_player_box))
		{
			if (!m_shield_sphere_pickup->get_s_active())
			{
				s_invincible = false;
				if (!s_invincible)
				{
					s_invincible = false;
					//m_player.object()->set_position(pos);
					health -= 10;
					m_cross_fade->activate();
					m_audio_manager->play("crash");
					enemy::setState_crashed(m_enemy_3);
				}
			}
			else
			{
				score += 1000;
			}
		}

		if (m_enemy_box_4.collision(m_player_box))
		{
			if (!m_shield_sphere_pickup->get_s_active())
			{
				s_invincible = false;
				if (!s_invincible)
				{
					s_invincible = false;
					//m_player.object()->set_position(pos);
					health -= 10;
					m_cross_fade->activate();
					m_audio_manager->play("crash");
					enemy::setState_crashed(m_enemy_4);
				}
			}
			else
			{
				score += 1000;
			}
		}

		if (m_enemy_box_5.collision(m_player_box))
		{
			if (!m_shield_sphere_pickup->get_s_active())
			{
				s_invincible = false;
				if (!s_invincible)
				{
					s_invincible = false;
					//m_player.object()->set_position(pos);
					health -= 10;
					m_cross_fade->activate();
					m_audio_manager->play("crash");
					enemy::setState_crashed(m_enemy_5);
				}
			}
			else
			{
				score += 1000;
			}

		}

		// track the time how long player survived
		if (tick == 60) 
		{
			tick = 0;
			timer = timer + 1;
			score = score + (timer * 5);
		}

		if (health < 100 && !m_shield_sphere_pickup->get_s_active())
		{
			m_fog = true;
		}
		if (health <= 0 || m_player.object()->position().z < -600)
		{
			game_over = true;
			m_gameover->activate();
		}
	}

}

void theGame::on_render()
{
	engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
	engine::render_command::clear();

	// Set up  shader. (renders textures and materials)
	const auto mesh_shader = engine::renderer::shaders_library()->get("mesh");

	engine::renderer::begin_scene(m_3d_camera, mesh_shader);

	// Set up some of the scene's parameters in the shader
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_on", m_fog);

	// Position the skybox centred on the player and render it
	glm::mat4 skybox_tranform(1.0f);
	skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(mesh_shader, m_skybox, skybox_tranform);

	engine::renderer::submit(mesh_shader, m_terrain);

	//glm::mat4 ship_transform(1.0f);
	//ship_transform = glm::translate(ship_transform, m_ship->position());
	//ship_transform = glm::scale(ship_transform, m_ship->scale());
	//engine::renderer::submit(mesh_shader, ship_transform, m_ship);

	// Submit all meshes to the shader
	if (m_pickup->active())
	{
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
		m_pickup->textures().at(0)->bind();
		m_pickup->set_position(glm::vec3(-2.5f, 1.5f, 7.f));
		glm::mat4 pickup_transform(1.0f);
		pickup_transform = glm::translate(pickup_transform, m_pickup->position());
		pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
		pickup_transform = glm::scale(pickup_transform, glm::vec3(1.f));
		engine::renderer::submit(mesh_shader, m_pickup->meshes().at(0), pickup_transform);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
	}
	if (m_pickup->active())
	{
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
		m_pickup->textures().at(0)->bind();
		m_pickup->set_position(glm::vec3(2.5f, 1.5f, 7.f));
		glm::mat4 pickup_transform(1.0f);
		pickup_transform = glm::translate(pickup_transform, m_pickup->position());
		pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
		pickup_transform = glm::scale(pickup_transform, glm::vec3(1.f));
		engine::renderer::submit(mesh_shader, m_pickup->meshes().at(0), pickup_transform);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
	}

	m_ship_material->submit(mesh_shader);
	engine::renderer::submit(mesh_shader, m_player.object());

	m_material->submit(mesh_shader);

	m_enemy_ship_material->submit(mesh_shader);

	// setting the start point up by using sphere and cuboid
	for (int i = 0; i < 5; i++)
	{
		glm::mat4 ball_transform1(1.0f);
		ball_transform1 = glm::translate(ball_transform1, glm::vec3(-2.5f - i * 2, 1.5f, 7.f));
		ball_transform1 = glm::rotate(ball_transform1, 200.f, glm::vec3(1, 0, 0));
		ball_transform1 = glm::scale(ball_transform1, m_ball->scale());
		engine::renderer::submit(mesh_shader, ball_transform1, m_ball);

		glm::mat4 ball_transform2(1.0f);
		ball_transform2 = glm::translate(ball_transform2, glm::vec3(2.5f + i * 2, 1.5f, 7.f));
		ball_transform2 = glm::rotate(ball_transform2, 200.f, glm::vec3(1, 0, 0));
		ball_transform2 = glm::scale(ball_transform2, m_ball->scale());
		engine::renderer::submit(mesh_shader, ball_transform2, m_ball);

	}

	// setting up the scene with the pyramids (tetrahedrons) and octahedrons
	m_tetrahedron_material->submit(mesh_shader);
	m_octahedron_material->submit(mesh_shader);
	for (int i = 0; i < 60; i++)
	{
		glm::mat4 tetrahedron_transform(1.0f);
		tetrahedron_transform = glm::translate(tetrahedron_transform, glm::vec3(20.f, 0.f, 0.f - (i * 10)));
		tetrahedron_transform = glm::rotate(tetrahedron_transform, 200.f, glm::vec3(1, 0, 0));
		tetrahedron_transform = glm::scale(tetrahedron_transform, m_tetrahedron->scale());
		engine::renderer::submit(mesh_shader, tetrahedron_transform, m_tetrahedron);

		glm::mat4 octahedron_transform(1.0f);
		octahedron_transform = glm::translate(octahedron_transform, glm::vec3(20.1f, 11.f, 4.5f - (i * 10)));
		octahedron_transform = glm::rotate(octahedron_transform, 150.f, glm::vec3(1, 0, 0));
		octahedron_transform = glm::scale(octahedron_transform, m_octahedron->scale());
		engine::renderer::submit(mesh_shader, octahedron_transform, m_octahedron);
	}
	for (int i = 0; i < 60; i++)
	{
		glm::mat4 tetrahedron_transform(1.0f);
		tetrahedron_transform = glm::translate(tetrahedron_transform, glm::vec3(-20.f, 0.f, 0.f - (i * 10)));
		tetrahedron_transform = glm::rotate(tetrahedron_transform, 200.f, glm::vec3(1, 0, 0));
		tetrahedron_transform = glm::scale(tetrahedron_transform, m_tetrahedron->scale());
		engine::renderer::submit(mesh_shader, tetrahedron_transform, m_tetrahedron);

		glm::mat4 octahedron_transform(1.0f);
		octahedron_transform = glm::translate(octahedron_transform, glm::vec3(-20.1f, 11.f, 4.5f - (i * 10)));
		octahedron_transform = glm::rotate(octahedron_transform, 200.f, glm::vec3(1, 0, 0));
		octahedron_transform = glm::scale(octahedron_transform, m_octahedron->scale());
		engine::renderer::submit(mesh_shader, octahedron_transform, m_octahedron);
	}

	engine::renderer::end_scene();

	// Submit material, set global shader uniforms and lighting meshes
	engine::renderer::begin_scene(m_3d_camera, mesh_shader);

	m_material->submit(mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_on", m_fog);

	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
	m_spotLight.submit(mesh_shader, 0);

	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", false);
	m_lightsource_material->submit(mesh_shader);
	engine::renderer::submit(mesh_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.f), m_spotLight.Position));
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", true);

	engine::renderer::end_scene();

	engine::renderer::begin_scene(m_3d_camera, mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_on", m_fog);

	glm::mat4 aniTransform = glm::mat4(1.0f);

	engine::renderer::submit(mesh_shader, m_ship);

	// adjust rotation and submit pickups
	glm::mat4 shield_pickup_transform(1.0f);
	shield_pickup_transform = glm::translate(shield_pickup_transform, m_shield_pickup_0->position() - glm::vec3(m_shield_pickup_0-> scale().x, 0.f, m_shield_pickup_0->scale().z)* m_shield_pickup_0->scale().x);
	shield_pickup_transform = glm::rotate(shield_pickup_transform, m_shield_pickup_0->rotation_amount(), m_shield_pickup_0-> rotation_axis());
	shield_pickup_transform = glm::scale(shield_pickup_transform, m_shield_pickup_0->scale());

	engine::renderer::submit(mesh_shader, m_shield_pickup_0);
	engine::renderer::submit(mesh_shader, m_shield_pickup_1);
	engine::renderer::submit(mesh_shader, m_shield_pickup_2);
	engine::renderer::submit(mesh_shader, m_shield_pickup_3);

	// render enemy ships
	m_enemy_0.on_render(mesh_shader);
	m_enemy_1.on_render(mesh_shader);
	m_enemy_2.on_render(mesh_shader);
	m_enemy_3.on_render(mesh_shader);
	m_enemy_4.on_render(mesh_shader);
	m_enemy_5.on_render(mesh_shader);
	//engine::renderer::submit(mesh_shader, m_enemy_ship);

	m_shield_sphere_pickup->on_render(mesh_shader);

	engine::renderer::end_scene();


	// Render text
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	if (!game_over && game_start) {
		m_text_manager->render_text(text_shader, "Health: " + std::to_string(health), 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Score: " + std::to_string(score), 145.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Time Survived: " + std::to_string(timer) + " Seconds", 300.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
	}

	// Render intro screen 
	engine::renderer::begin_scene(m_2d_camera, mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", false);
	m_introscreen->on_render(mesh_shader);
	m_cross_fade->on_render(mesh_shader);
	if (game_over)
	{
		m_gameover->on_render(mesh_shader);
	}
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", true);
	engine::renderer::end_scene();

}

void theGame::on_event(engine::event& event)
{
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);
		if (e.key_code() == engine::key_codes::KEY_SPACE)
		{
			if (game_start == false)
			{
				m_audio_manager->stop("intro");
				m_audio_manager->play("space_click");

				//wait(2);
				m_audio_manager->play("theme");
				//m_audio_manager->loop("theme", true);
				game_start = true;
			}
		}
		if (e.key_code() == engine::key_codes::KEY_TAB)
		{
			engine::render_command::toggle_wireframe();
		}
		

	}
}


void theGame::wait(int number_of_seconds)
{
	std::this_thread::sleep_for(std::chrono::seconds(number_of_seconds));
}

void theGame::check_bounce()
{
	if (m_prev_sphere_y_vel < 0.1f && m_ball->velocity().y > 0.1f)
		//m_audio_manager->play("bounce");
		m_audio_manager->play_spatialised_sound("bounce", m_3d_camera.position(), glm::vec3(m_ball->position().x, 0.f, m_ball->position().z));
	m_prev_sphere_y_vel = m_game_objects.at(1)->velocity().y;
}

void theGame::updateSpotlightPosition()
{
	// Update the position of the spotlight to follow the ship
	m_spotLight.Position = m_ship->position() + glm::vec3(0.0f, 10.0f, 20.0f);
}



