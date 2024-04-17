#include "IFETime.h"

using namespace std;
using namespace IFE;
using namespace std::chrono;

std::chrono::steady_clock::time_point IFETime::sStartTime_;
std::chrono::steady_clock::time_point IFETime::sNowTime_;
std::chrono::steady_clock::time_point IFETime::sFrontTime_;
float IFETime::sDeltaTime_;
float IFETime::sTime_;

void IFE::IFETime::Initialize()
{
	sStartTime_ = steady_clock::now();
	sFrontTime_ = sStartTime_;
	sNowTime_ = sStartTime_;
}

std::chrono::steady_clock::time_point IFE::IFETime::GetNowTime()
{
	return std::chrono::steady_clock::time_point();
}

std::chrono::steady_clock::time_point IFE::IFETime::GetStartTime()
{
	return std::chrono::steady_clock::time_point();
}

void IFE::IFETime::Update()
{
	sFrontTime_ = sNowTime_;
	sNowTime_ = steady_clock::now();
	milliseconds t = duration_cast<milliseconds>(sNowTime_ - sStartTime_);
	milliseconds dt = duration_cast<milliseconds>(sNowTime_ - sFrontTime_);

	sTime_ = (float)t.count() / 1000.f;
	sNoScaleDeltaTime_ = min((float)dt.count() / 1000.f, 1.0f);
	sTimeScale_ = max(0.f, sTimeScale_);
	sDeltaTime_ = sNoScaleDeltaTime_ * sTimeScale_;
}

void FrameCountTime::Set(int32_t e)
{
	this->end_ = e;
	timer_ = 0;
}

void FrameCountTime::TimeSet(int32_t t, int32_t e)
{
	this->timer_ = t;
	this->end_ = e;
}

void FrameCountTime::SetIf(int32_t e, bool flag)
{
	if (flag)Set(e);
}

void FrameCountTime::Update()
{
	timer_++;
}

void FrameCountTime::SafeDownUpdate()
{
	if (timer_ > 0)
	{
		timer_--;
	}
}

void FrameCountTime::SafeUpdate()
{
	if (IsEnd())
	{
		return;
	}
	timer_++;
}

int32_t FrameCountTime::NowTime() const
{
	return timer_;
}

bool FrameCountTime::IsEnd() const
{
	return timer_ >= end_;
}

int32_t FrameCountTime::GetEndTime() const
{
	return end_;
}