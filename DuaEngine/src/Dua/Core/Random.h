#pragma once

#include <random>

namespace Dua {

	class Random
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static float Float() // [0.0,1.0)
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

		static float Range(float left, float right)
		{
			return left + (right - left) * Float();
		}

		static int RangeI(float left, float right)
		{
			return (int)Range(left, right);
		}

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

}