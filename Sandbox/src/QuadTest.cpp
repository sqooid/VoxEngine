#include "voxpch.h"

#include "QuadTest.h"

#include "Engine/Subsystems/Render/Renderer.h"
#include <glm/glm.hpp>

QuadTest::QuadTest()
{
	max = random.max();
}

void QuadTest::onUpdate()
{
	vox::Renderer* renderer = dynamic_cast<vox::Renderer*>(getSub(vox::SubType::Renderer));

	for (int i = 0; i < 10000; ++i)
	{
		renderer->batchDrawQuad({
			glm::vec3(randFloat(), randFloat(), randFloat()),
			glm::vec3(randFloat(), randFloat(), randFloat()),
			glm::vec3(randFloat(), randFloat(), randFloat()),
			glm::vec3(randFloat(), randFloat(), randFloat()),
			glm::u8vec4(randByte(), randByte(), randByte(), randByte()),
			glm::vec3(randFloat(), randFloat(), randFloat()) });
	}
}

void QuadTest::onAttach()
{
	getEventHandler()->subscribeEvent<vox::KeyEvent>(EVENT_CALLBACK(print));
}

void QuadTest::print(const vox::KeyEvent& event)
{
	std::cout << event.getKey() << std::endl;
}

float QuadTest::randFloat()
{
	const float gen = static_cast<float>(random());
	return (gen / max * 2.0f) - 1.0f;
}

uint8_t QuadTest::randByte()
{
	const float gen = static_cast<float>(random());
	return gen / max * 255;
}
