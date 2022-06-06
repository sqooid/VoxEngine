#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <entt.hpp>

namespace vox
{
	namespace Comp
	{
		struct Parent
		{
			entt::entity parent;
		};

		struct Transform
		{
			glm::vec3 position;
			glm::quat rotation;
			glm::vec3 scale;
		};

		struct Camera
		{
			glm::mat4 projection;
		};
	}
}