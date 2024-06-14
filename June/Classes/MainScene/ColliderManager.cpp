#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
#include "ColliderManager.h"

using namespace SimpleMath;

void ColliderManager::ColliderResolve(Player& player, Enemy& enemy)
{
	//�v���C���[�U���ƓG�̓����蔻��
	//�U�����Ȃ�Ώ������s��
	//�G���U�����󂯕t���Ȃ���ԂȂ珈�����s��Ȃ�
	if (player.GetAttackState().GetAttackNow() && enemy.GetHitOk())
	{

		//������ 
		if (player.GetSpear().GetCollider().Intersects(enemy.GetCollider()))
		{
			//�R���C�_�[�̃T�C�Y��0�Œ�`����Ă���Γ�����Ȃ��Ƃ���
			if (player.GetSpear().GetColliderSize() != Vector3::Zero)
			{
				ColliderInformation::Collider _collider;
				_collider._hitPosition = player.GetSpear().GetPosition();
				_collider._hitWeaponState = StateStorage::WeaponState::Spear;

				enemy.OnCollisionEnter(enemy, _collider);
				player.SpearOnCollisionEnter();
			}

		}
		//������
		if (player.GetAxe().GetCollider().Intersects(enemy.GetCollider()))
		{
			ColliderInformation::Collider _collider;
			//_collider._hitPosition = 
			_collider._hitWeaponState = StateStorage::WeaponState::Axe;

			enemy.OnCollisionEnter(enemy, _collider);
		}
	}

	//�G�U���ƃv���C���[�̓����蔻��
	if (enemy.GetAttackState().GetAttackNow())
	{
		//������
		if (enemy.GetSpear().GetCollider().Intersects(player.GetCollider()))
		{
			player.OnCollisionEnter();
			enemy.GetSpear().OnCollisionEnter();
		}
		//������
		if (enemy.GetAxe().GetCollider().Intersects(player.GetCollider()))
		{
			player.OnCollisionEnter();
		}
	}


}

