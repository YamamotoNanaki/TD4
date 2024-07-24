#include "Ease.h"

using namespace IFE;

float IFE::Lerp(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	return change * time + startPos;
}
float IFE::InQuad(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	return change * time * time + startPos;
}
float IFE::InQuart(float startPos, float endPos, float maxTime, float time)
{
	time /= maxTime;
	float change = endPos - startPos;
	return change * time * time * time * time + startPos;
}
float IFE::InQuint(float startPos, float endPos, float maxTime, float time)
{
	time /= maxTime;
	float change = endPos - startPos;
	return change * time * time * time * time * time + startPos;
}
float IFE::InExpo(float startPos, float endPos, float maxTime, float time)
{
	float x = time / maxTime;
	float change = endPos - startPos;
	float r = x == 0 ? 0 : powf(2, 10 * x - 10) * change;
	return r + startPos;
}
float IFE::OutQuad(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	return -change * time * (time - 2) + startPos;
}
IFE::Float3 IFE::OutQuadFloat3(const Float3& startPos, const Float3& endPos, const FrameCountTime& timer)
{
	return { OutQuad(startPos.x, endPos.x, (float)timer.GetEndTime(), (float)timer.NowTime()),
			 OutQuad(startPos.y, endPos.y, (float)timer.GetEndTime(), (float)timer.NowTime()),
			 OutQuad(startPos.z, endPos.z, (float)timer.GetEndTime(), (float)timer.NowTime()) };
};

IFE::Float3 IFE::LerpFloat3(const Float3& startPos, const Float3& endPos, const FrameCountTime& timer)
{
	return { Lerp(startPos.x, endPos.x, (float)timer.GetEndTime(), (float)timer.NowTime()),
			 Lerp(startPos.y, endPos.y, (float)timer.GetEndTime(), (float)timer.NowTime()),
			 Lerp(startPos.z, endPos.z, (float)timer.GetEndTime(), (float)timer.NowTime()) };
}
IFE::Float3 IFE::OutQuadFloat3(const Float3& startPos, const Float3& endPos, float maxTime, float time)
{
	return { OutQuad(startPos.x, endPos.x, maxTime, time),
			 OutQuad(startPos.y, endPos.y, maxTime, time),
			 OutQuad(startPos.z, endPos.z, maxTime, time) };
}
IFE::Float3 IFE::LerpFloat3(const Float3& startPos, const Float3& endPos, float maxTime, float time)
{
	return { Lerp(startPos.x, endPos.x, maxTime, time),
			 Lerp(startPos.y, endPos.y, maxTime, time),
			 Lerp(startPos.z, endPos.z, maxTime, time) };
}
//
//IFE::Float4 IFE::LerpFloat4(const IFE::Float4& startPos, const IFE::Float4& endPos, float maxTime, float time)
//{
//	return { Lerp(startPos.x, endPos.x, maxTime, time),
//			 Lerp(startPos.y, endPos.y, maxTime, time),
//			 Lerp(startPos.z, endPos.z, maxTime, time),
//			 Lerp(startPos.w, endPos.w, maxTime, time) };
//}

float IFE::InOutQuad(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime / 2;
	float change = endPos - startPos;
	if (time < 1) return change / 2 * time * time + startPos;
	return -change / 2 * ((--time) * (time - 2) - 1) + startPos;
}
IFE::Float3 IFE::InOutQuad(const IFE::Float3& startPos, const IFE::Float3& endPos, float maxTime, float time)
{
	return { InOutQuad(startPos.x, endPos.x, maxTime, time),
			 InOutQuad(startPos.y, endPos.y, maxTime, time),
			 InOutQuad(startPos.z, endPos.z, maxTime, time) };
}
IFE::Float2 IFE::InOutQuad(const IFE::Float2& startPos, const IFE::Float2& endPos, float maxTime, float time)
{
	return { InOutQuad(startPos.x, endPos.x, maxTime, time),
			 InOutQuad(startPos.y, endPos.y, maxTime, time) };
}
float IFE::OutInRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time) {
	if (time < relayTime) {
		return IFE::OutQuad(relayPos, startPos, relayTime, time);
	}
	else {
		return IFE::InQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}

float IFE::InOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time) {
	if (time < relayTime) {
		return IFE::InQuad(relayPos, startPos, relayTime, time);
	}
	else {
		return IFE::OutQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}
float IFE::OutOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time) {
	if (time < relayTime) {
		return IFE::OutQuad(relayPos, startPos, relayTime, time);
	}
	else {
		return IFE::OutQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}

float IFE::Lerp(float startPos, float endPos, const FrameCountTime& timer)
{
	return Lerp(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}
float IFE::InQuad(float startPos, float endPos, const FrameCountTime& timer)
{
	return InQuad(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}
float IFE::OutQuad(float startPos, float endPos, const FrameCountTime& timer)
{
	return OutQuad(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}
float IFE::InOutQuad(float startPos, float endPos, const FrameCountTime& timer)
{
	return InOutQuad(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}
float IFE::EaseInBack(float startPos, float endPos, const FrameCountTime& timer)
{
	return EaseInBack(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}
float IFE::EaseInBack2(float startPos, float endPos, const FrameCountTime& timer)
{
	return EaseInBack2(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}
float IFE::EaseOutBounce(float startPos, float endPos, const FrameCountTime& timer)
{
	return EaseOutBounce(startPos, endPos, (float)timer.GetEndTime(), (float)timer.NowTime());
}

float IFE::EaseOutCirc(float time, const float startPoint, const float endPoint, const float maxTime)
{
	float x = time / maxTime;
	float v = sqrt(1.0f - pow(x - 1.0f, 2.0f));
	float ret = (endPoint - startPoint) * v + startPoint;
	return ret;
}

float IFE::EaseInOutBack(float time, const float startPoint, const float endPoint, const float maxTime)
{
	float x = time / maxTime;
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;
	float v = x < 0.5f
		? (pow(2.0f * x, 2.0f) * ((c2 + 1.0f) * 2.0f * x - c2)) / 2.0f
		: (pow(2.0f * x - 2.0f, 2.0f) * ((c2 + 1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
	float ret = (endPoint - startPoint) * v + startPoint;
	return ret;
}

float IFE::EaseInOutQuart(float time, const float startPoint, const float endPoint, const float maxTime)
{
	float x = time / maxTime;
	float v = x < 0.5f ? 8.0f * x * x * x * x : 1.0f - pow(-2.0f * x + 2.0f, 4.0f) / 2.0f;
	float ret = (endPoint - startPoint) * v + startPoint;
	return ret;
}

float IFE::EaseInBack(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	float c1 = 1.70158f;
	float c3 = c1 + 1.f;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
float IFE::EaseInBack2(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	float c1 = 1.2f;
	float c3 = c1 + 0.8f;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
float IFE::EaseOutQuint(float time, const float startPoint, const float endPoint, const float maxTime)
{
	float x = time / maxTime;
	float v = 1.0f - pow(1.0f - x, 5.0f);
	float ret = (endPoint - startPoint) * v + startPoint;
	return ret;
}
float IFE::EaseOutBounce(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	const float n1 = 7.5625;
	const float d1 = 2.75;

	float a = 0;

	if (time < 1 / d1)
	{
		a = n1 * time * time;
	}
	else if (time < 2 / d1)
	{
		a = n1 * (time -= 1.5f / d1) * time + 0.75f;
	}
	else if (time < 2.5f / d1)
	{
		a = n1 * (time -= 2.25f / d1) * time + 0.9375f;
	}
	else {
		a = n1 * (time -= 2.625f / d1) * time + 0.984375f;
	}

	return a * change + startPos;
}

float IFE::EaseInBounce(float startPos, float endPos, float maxTime, float time)
{
	time /= maxTime;
	return (endPos - startPos) * (1 - EaseOutBounce(1, 0, 1, time)) + startPos;
}
