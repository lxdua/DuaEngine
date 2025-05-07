#pragma once

namespace Dua {

	class Timestep
	{
	public:
		Timestep(double time = 0.0f)
			: m_Time(time) {}

		operator double() const { return m_Time; }

		double GetSecond() const { return m_Time; }
		double GetMillisecond() const { return m_Time * 1000.0f; }

	private:
		double m_Time = 0.0f;
	};

}