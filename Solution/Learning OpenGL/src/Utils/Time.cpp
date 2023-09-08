#include "Time.h"
#include <GLFW/glfw3.h>

float Time::m_LastFrameTimestamp = 0;
float Time::m_DeltaTime = 0;

float Time::DeltaTime()
{
    return m_DeltaTime;
}

void Time::Update()
{
    m_DeltaTime = glfwGetTime() - m_LastFrameTimestamp;
    m_LastFrameTimestamp = glfwGetTime();
}

float Time::TimeSinceStartup()
{
    return glfwGetTime();
}
