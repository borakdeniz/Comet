#pragma once

namespace engine
{
	class mesh;

	/// \brief Class creating a octahedron object with a mesh of a specified size
	class octahedron
	{
	public:
		/// \brief Constructor
		octahedron(std::vector<glm::vec3> vertices);

		/// \brief Destructor
		~octahedron();

		/// \brief Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<octahedron> create(std::vector<glm::vec3> vertices);
	private:
		/// \brief Fields
		// vertices of the octahedron specified by a vector of positions.
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}
