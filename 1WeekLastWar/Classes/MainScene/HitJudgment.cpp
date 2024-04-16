#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "HitJudgment.h"

void HitJudgment::BulletEnemyIntercect(BulletManeger::BulletSturct& bulletStruct,
	EnemyManeger::EnemyManegerSturct& enemySturct)
{

	for (int i = 0; i < bulletStruct.s_bulletCount; i++)
	{
		Bullet::BulletStruct _bs = bulletStruct.m_bullet[i].GetBulletStruct();
		if (!_bs.GetFireOn())
		{
			continue;
		}

		for (int j = 0; j < enemySturct.s_enemyCount; j++)
		{
			Enemy::EnemyStruct _es = enemySturct.m_enemy[j].GetEnemyStruct();

			if (_bs.GetCollider().Intersects(_es.GetCollider()))
			{
				bulletStruct.m_bullet[i].Hit();
				enemySturct.m_enemy[j].Hit();
			}

		}

	}

}

void HitJudgment::PlayerItemIntercect(Player& player,
	ItemManeger::ItemMangerStruct& itemStruct)
{
	for (int i = 0; i < itemStruct.s_itemCount; i++)
	{
		Item::ItemStruct _is = itemStruct.m_item[i].GetItemStruct();
		if (_is.GetCollider().Intersects(player.GetCollider()))
		{
			itemStruct.m_item[i].Hit();
			player.ItemGet();
			break;
		}
	}
}