#pragma once
#include "IFMath.h"
#include "IFETime.h"
#include "Float3.h"

namespace IFE
{
	/// <summary>
	/// �����ړ�
	/// </summary>
	/// <param name="startPos">�����n�_</param>
	/// <param name="endPos">�I���n�_</param>
	/// <param name="maxTime">�ő厞��</param>
	/// <param name="time">���݂̎���</param>
	/// <returns></returns>
	float Lerp(float startPos, float endPos, float maxTime, float time);

	float InQuad(float startPos, float endPos, float maxTime, float time);
	float InQuart(float startPos, float endPos, float maxTime, float time);
	float InQuint(float startPos, float endPos, float maxTime, float time);
	float InExpo(float startPos, float endPos, float maxTime, float time);

	float OutQuad(float startPos, float endPos, float maxTime, float time);

	IFE::Float3 OutQuadFloat3(const IFE::Float3& startPos, const IFE::Float3& endPos, const IFE::FrameCountTime& timer);
	IFE::Float3 LerpFloat3(const IFE::Float3& startPos, const IFE::Float3& endPos, const IFE::FrameCountTime& timer);
	IFE::Float3 OutQuadFloat3(const IFE::Float3& startPos, const IFE::Float3& endPos, float maxTime, float time);
	IFE::Float3 LerpFloat3(const IFE::Float3& startPos, const IFE::Float3& endPos, float maxTime, float time);

	float InOutQuad(float startPos, float endPos, float maxTime, float time);
	IFE::Float3 InOutQuad(const IFE::Float3& startPos, const IFE::Float3& endPos, float maxTime, float time);
	IFE::Float2 InOutQuad(const IFE::Float2& startPos, const IFE::Float2& endPos, float maxTime, float time);

	float EaseInBack(float startPos, float endPos, float maxTime, float time);

	float EaseInBack2(float startPos, float endPos, float maxTime, float time);

	float EaseOutBounce(float startPos, float endPos, float maxTime, float time);

	float EaseInBounce(float startPos, float endPos, float maxTime, float time);

	float OutInRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time);

	float InOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time);

	float OutOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time);


	float Lerp(float startPos, float endPos, const FrameCountTime& timer);
	float InQuad(float startPos, float endPos, const FrameCountTime& timer);
	float OutQuad(float startPos, float endPos, const FrameCountTime& timer);
	float InOutQuad(float startPos, float endPos, const FrameCountTime& timer);
	float EaseInBack(float startPos, float endPos, const FrameCountTime& timer);
	float EaseInBack2(float startPos, float endPos, const FrameCountTime& timer);
	float EaseOutBounce(float startPos, float endPos, const FrameCountTime& timer);
}