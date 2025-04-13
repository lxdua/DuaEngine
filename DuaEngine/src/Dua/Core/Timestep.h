#pragma once

namespace Dua {

	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		operator float() const { return m_Time; }

		float GetSecond() const { return m_Time; }
		float GetMillisecond() const { return m_Time * 1000.0f; }

	private:
		float m_Time = 0.0f;
	};

}