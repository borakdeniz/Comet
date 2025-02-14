#include "pch.h"
#include "octahedron.h"
#include <engine.h>	

engine::octahedron::octahedron(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;
	// top front normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));
	// top right normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(2)));
	// top back normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(4), vertices.at(0) - vertices.at(3)));
	// top left normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(4)));
	// bottom front normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(1), vertices.at(5) - vertices.at(2)));
	// bottom right normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(2), vertices.at(5) - vertices.at(3)));
	// bottom back normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(3), vertices.at(5) - vertices.at(4)));
	// bottom left normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(4), vertices.at(5) - vertices.at(1)));

	std::vector<mesh::vertex> octahedron_vertices
	{
		//top front
		//  position			normal			      tex coord       
		{ vertices.at(0),		normals.at(0),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(0),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(0),		{ 0.5f, 1.f } },

		//top right
		{ vertices.at(0),		normals.at(1),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(1),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(1),		{ 0.5f, 1.f } },

		//top back
		{ vertices.at(0),		normals.at(2),		{ 0.f,  0.f } },
		{ vertices.at(4),		normals.at(2),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(2),		{ 0.5f, 1.f } },

		//top left
		{ vertices.at(0),		normals.at(3),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(3),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(3),		{ 0.5f, 1.f } },

		//bottom front
		{ vertices.at(5),		normals.at(4),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(4),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(4),		{ 0.5f, 1.f } },

		//bottom right
		{ vertices.at(5),		normals.at(5),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(5),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(5),		{ 0.5f, 1.f } },

		//bottom back
		{ vertices.at(5),		normals.at(6),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(6),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(6),		{ 0.5f, 1.f } },

		//bottom left
		{ vertices.at(5),		normals.at(7),		{ 0.f,  0.f } },
		{ vertices.at(4),		normals.at(7),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(7),		{ 0.5f, 1.f } },


	};

	const std::vector<uint32_t> octahedron_indices
	{
		0,  1,  2,	//front
		3,  4,  5,	//left
		6,	7,  8,	//right
		9,  10,  11,//bottom
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
	};

	m_mesh = engine::mesh::create(octahedron_vertices, octahedron_indices);
}

engine::octahedron::~octahedron() {}

engine::ref<engine::octahedron> engine::octahedron::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::octahedron>(vertices);
}
