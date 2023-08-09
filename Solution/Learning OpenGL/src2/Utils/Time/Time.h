#pragma once

namespace LearnOGL {

	class Time
	{
	private:
		static float m_LastFrameTimestamp;
		static float m_DeltaTime;

	public:
		static void Init();
		static void Update();

		static float DeltaTime();
	};

}

