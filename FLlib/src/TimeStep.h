#pragma once

#include <GLFW/glfw3.h>

namespace FLlib
{
	class TimeStep
	{
	public:
		TimeStep(float time = 0.0f)
			:m_Time(time)
		{
		}

		operator float() const {
			return m_Time;
		}

		void Update() {
			double time = glfwGetTime();
			m_Time = (float)(time - m_LastFrame);
			m_LastFrame = time;
		}

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
		double m_LastFrame = 0.0;
	};
}
