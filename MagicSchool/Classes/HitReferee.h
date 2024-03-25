#pragma once

#include "Classes/Player/Player.h"

#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"


class HitReferee
{
public:
	bool  BulletHitJudge(const SimpleMath::Rectangle,
		Player::Bullets&, const bool);

	bool OneHitJudge(const SimpleMath::Rectangle,
		const SimpleMath::Rectangle);

private:

};

