#include "Time.h"
#include <GLFW/glfw3.h>

namespace LearnOGL {

	float Time::m_LastFrameTimestamp = 0.0f;  // Definition of the static member variables
	float Time::m_DeltaTime = 0.0f;
	
	void LearnOGL::Time::Init()
	{
		m_LastFrameTimestamp = 0;
		m_DeltaTime = 0;
	}

	void Time::Update()
	{
		m_DeltaTime = glfwGetTime() - m_LastFrameTimestamp;
		m_LastFrameTimestamp = glfwGetTime();
	}

	float Time::DeltaTime()
	{
		return m_DeltaTime;
	}
}