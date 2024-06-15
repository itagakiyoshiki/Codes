#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "ItagakiMath.h"
#include <random>

using namespace SimpleMath;

/// <summary>
/// —^‚¦‚ç‚ê‚½”’l‚ÌŠÔ‚Ì—”‚ğ•Ô‚µ‚Ü‚·
/// </summary>
/// <param name="minRnage"></param>
/// <param name="maxRange"></param>
/// <returns></returns>
const int ItagakiMath::Random(const int minRnage, const int maxRange)
{
	std::random_device _randomDevice;
	std::mt19937 _mersenneTwisterGenerator(_randomDevice());
	std::uniform_int_distribution<> _randDistribution(minRnage, maxRange);

	float _randomValue = _randDistribution(_mersenneTwisterGenerator);

	return _randomValue;
}

/// <summary>
/// “ñ“_ŠÔ‚Ì“àÏ‚ğ•Ô‚µ‚Ü‚·
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
const float  ItagakiMath::Dot(const SimpleMath::Vector3 v1, const SimpleMath::Vector3 v2)
{
	float _x = v1.x * v2.x;
	float _z = v1.z * v2.z;

	float _dot = _x + _z;

	return _dot;
}

