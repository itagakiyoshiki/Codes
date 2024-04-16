#pragma once
#include "Scenes/Scene.h"
#include "Classes/Player/Bullet/BulletManeger.h"
#include "Classes/Player/Player/Player.h"
#include "Classes/Enemy/EnemyManeger.h"
#include "Classes/Item/ItemManeger.h"

class HitJudgment
{
public:

	void BulletEnemyIntercect(BulletManeger::BulletSturct&,
		EnemyManeger::EnemyManegerSturct&);

	void PlayerItemIntercect(Player&,
		ItemManeger::ItemMangerStruct&);

private:


};

