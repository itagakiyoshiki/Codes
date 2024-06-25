#pragma once
#include "Scenes/Scene.h"

class ItagakiMath
{
public:

	/// <summary>
	/// —^‚¦‚ç‚ê‚½ˆÊ’u“¯m‚Ì‹——£‚ğ•Ô‚µ‚Ü‚·
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static const float Distance(const SimpleMath::Vector3& v1, const SimpleMath::Vector3& v2)
	{
		float _distance = std::sqrt(pow(v1.x - v2.x, 2) + pow(v1.z - v2.z, 2));

		return _distance;
	}

	/// <summary>
	/// —^‚¦‚ç‚ê‚½ƒxƒNƒgƒ‹“¯m‚ÌŠOÏ‚ğ•Ô‚µ‚Ü‚·
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static const SimpleMath::Vector3 Cross(
		const SimpleMath::Vector3& v1,
		const SimpleMath::Vector3& v2)
	{
		return  SimpleMath::Vector3(
			(v1.y * v2.z) - (v1.z * v2.y),
			(v1.z * v2.x) - (v1.x * v2.z),
			(v1.x * v2.y) - (v1.y * v2.x));
	}

	static const int Random(const int minRnage, const int maxRange);

	static const float Dot(const SimpleMath::Vector3 v1, const SimpleMath::Vector3 v2);

private:


};

