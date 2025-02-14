#pragma once

#include "engine.h"

// alpha sphere modified according to the needs
class shield_sphere_pickup
{
public:
    shield_sphere_pickup(glm::vec3 start_colour, glm::vec3 end_colour, bool zap, float zap_level, float max_time);
    ~shield_sphere_pickup();

    void on_update(const engine::timestep& time_step);
    void on_render(engine::ref<engine::shader> shader);
    void activate(float radius, glm::vec3 position);

    static engine::ref<shield_sphere_pickup> create(glm::vec3 start_colour, glm::vec3 end_colour, bool zap, float zap_level, float max_time);

    void set_position(glm::vec3 pos);

    bool get_s_active() { return s_active; }
private:
    float random();

private:
    engine::ref<engine::sphere> m_sphere;
    engine::ref<engine::material> m_material;

    glm::vec3 m_start_colour;
    glm::vec3 m_end_colour;

    bool s_zap;
    float m_zap_level;
    float m_max_time;
    bool s_active;

    float m_timer;
    float m_scale;
    glm::vec3 m_position;
};
