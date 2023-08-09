#pragma once

class Time {
private:
	static float m_LastFrameTimestamp;
	static float m_DeltaTime;

public:
	static float DeltaTime();
	static void Update();
};