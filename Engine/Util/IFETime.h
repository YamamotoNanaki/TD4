#pragma once
#include <chrono>

namespace IFE
{
	class IFETime
	{
		IFETime() {};
		IFETime(const IFETime&) {};
		~IFETime() {};
		const IFETime& operator=(const IFETime&) {};
	private:
		static std::chrono::steady_clock::time_point sStartTime_;
		static std::chrono::steady_clock::time_point sNowTime_;
		static std::chrono::steady_clock::time_point sFrontTime_;

	public:
		static float sDeltaTime_;
		static float sTime_;
		inline static float sTimeScale_ = 1.0f;
		inline static float sNoScaleDeltaTime_ = 1.0f;

	public:
		static void Initialize();
		static std::chrono::steady_clock::time_point GetNowTime();
		static std::chrono::steady_clock::time_point GetStartTime();
		static void Update();
	};

	class FrameCountTime
	{
	private:
		int32_t timer_ = 0;
		int32_t end_ = 0;

	public:
		void Set(int32_t end);
		void TimeSet(int32_t timer, int32_t end);
		void SetIf(int32_t end, bool flag);
		void Update();
		void SafeDownUpdate();
		void SafeUpdate();//タイマーがendの数値を超えてカウントされないようになっています。
		int32_t NowTime()const;
		bool IsEnd()const;
		int32_t GetEndTime()const;
	};
}
