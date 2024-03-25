#include "HitReferee.h"

bool HitReferee::BulletHitJudge(const SimpleMath::Rectangle rect,
	Player::Bullets& bulletsRect, const bool isPlayerJudge)
{

	//弾が何かと当たっているかを判別する

	for (int i = 0; i < bulletsRect.s_fireCount; i++)
	{
		if (!bulletsRect.m_fireBullet[i].GetShootingOK())
		{
			if (rect.Intersects(bulletsRect.m_fireBullet[i].GetCollider()))
			{
				//プレイヤーが弾に当たったら位置をそのままにしておきたい
				if (isPlayerJudge)
				{
					return true;
				}
				bulletsRect.m_fireBullet[i].Hit();
				return true;
			}
		}
	}

	for (int i = 0; i < bulletsRect.s_iceCount; i++)
	{
		if (!bulletsRect.m_iceBullet[i].GetShootingOK())
		{
			if (rect.Intersects(bulletsRect.m_iceBullet[i].GetCollider()))
			{
				//プレイヤーが弾に当たったら位置をそのままにしておきたい
				if (isPlayerJudge)
				{
					return true;
				}
				bulletsRect.m_iceBullet[i].Hit();
				return true;
			}
		}
	}

	for (int i = 0; i < bulletsRect.s_windCount; i++)
	{
		if (!bulletsRect.m_windBullet[i].GetShootingOK())
		{
			if (rect.Intersects(bulletsRect.m_windBullet[i].GetCollider()))
			{
				//プレイヤーが弾に当たったら位置をそのままにしておきたい
				if (isPlayerJudge)
				{
					return true;
				}
				bulletsRect.m_windBullet[i].Hit();
				return true;
			}
		}
	}

	return false;
}

bool HitReferee::OneHitJudge(const SimpleMath::Rectangle rect1,
	const SimpleMath::Rectangle rect2)
{

	if (rect1.Intersects(rect2))
	{
		return true;
	}

	return false;
}