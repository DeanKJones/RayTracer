#pragma once

#include <random>

#include <glm/glm.hpp>

namespace Walnut {

	class Random
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static uint32_t UInt()
		{
			return s_Distribution(s_RandomEngine);
		}

		static uint32_t UInt(uint32_t min, uint32_t max)
		{
			return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
		}

		static float Float()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

        static float Float(float a, float b) {
            float random = (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
            float diff = b - a;
            float r = random * diff;
            return a + r;
        }

		static glm::vec3 Vec3()
		{
			return glm::vec3(Float(), Float(), Float());
		}

		static glm::vec3 Vec3(float min, float max)
		{
			return glm::vec3(Float() * (max - min) + min, Float() * (max - min) + min, Float() * (max - min) + min);
		}

		static glm::vec3 InUnitSphere()
		{
			return glm::normalize(Vec3(-1.0f, 1.0f));
		}
		
		static glm::vec3 InUnitHemi(const glm::vec3& normal)
		{
			glm::vec3 InUnitSphere = Random::InUnitSphere();
			if (glm::dot(InUnitSphere, normal) > 0.0f){
				return InUnitSphere;
			}
			else {
				return -InUnitSphere;
			}
		}

        static glm::vec3 RandomCosineDirection() {
            auto r1 = Float();
            auto r2 = Float();
            auto z = sqrt(1-r2);

            auto phi = 2 * M_PI * r1;
            auto x = cos(phi) * sqrt(r2);
            auto y = sin(phi) * sqrt(r2);

            return glm::vec3(x, y, z);
        }

        static glm::vec2 RandomPointInCircle() {
            float angle = Random::Float() * 2 * PI;
            glm::vec2 pointOnCircle = glm::vec2(glm::cos(angle), glm::sin(angle));
            return pointOnCircle * glm::sqrt(Random::Float());
        }

	private:
        constexpr static const float PI = 3.1415926f;
		static thread_local std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

}


