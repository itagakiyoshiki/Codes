#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "ColliderManager.h"

using namespace SimpleMath;

void ColliderManager::ColliderResolve(Player& player, Enemy& enemy)
{
	//プレイヤー攻撃と敵の当たり判定
	//攻撃中ならば処理を行う
	//敵が攻撃を受け付けない状態なら処理を行わない
	if (player.GetAttackState().GetAttackNow() && enemy.GetHitOk())
	{

		//槍判定 
		if (player.GetSpear().GetCollider().Intersects(enemy.GetCollider()))
		{
			//コライダーのサイズが0で定義されていれば当たらないとする
			if (player.GetSpear().GetColliderSize() != Vector3::Zero)
			{
				ColliderInformation::Collider _collider;
				_collider._hitPosition = player.GetSpear().GetPosition();
				_collider._hitWeaponState = StateStorage::WeaponState::Spear;

				enemy.OnCollisionEnter(enemy, _collider);
				player.SpearOnCollisionEnter();
			}

		}
		//斧判定
		if (player.GetAxe().GetCollider().Intersects(enemy.GetCollider()))
		{
			ColliderInformation::Collider _collider;
			//_collider._hitPosition = 
			_collider._hitWeaponState = StateStorage::WeaponState::Axe;

			enemy.OnCollisionEnter(enemy, _collider);
		}
	}

	//敵攻撃とプレイヤーの当たり判定
	if (enemy.GetAttackState().GetAttackNow())
	{
		//槍判定
		if (enemy.GetSpear().GetCollider().Intersects(player.GetCollider()))
		{
			player.OnCollisionEnter();
			enemy.GetSpear().OnCollisionEnter();
		}
		//斧判定
		if (enemy.GetAxe().GetCollider().Intersects(player.GetCollider()))
		{
			player.OnCollisionEnter();
		}
	}


}

