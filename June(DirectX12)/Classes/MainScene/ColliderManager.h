#pragma once
#include "Scenes/Scene.h"
#include "Classes/MainScene/Player/Player.h"
#include "Classes/MainScene/Enemy/Enemy.h"
#include "Classes/ColliderInformation.h"

class ColliderManager
{
public:

	void ColliderResolve(Player&, Enemy&);



private:

	void PlayerToEnemyResolve(Player& player, Enemy& enemy);

	void EnemyToPlayerResolve(Enemy& enemy, Player& player);

};

