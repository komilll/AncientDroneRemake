///////////////////////////////
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "player.h"
#include "inputclass.h"
#include "enemywanderer.h"
#include "enemyflying.h"
#include "enemyarcher.h"
#include "dronecontroller.h"
#include "uicontroller.h"
#include "mouseclass.h"
#include <functional>
#include <typeinfo>
#include <type_traits>

class GameManager
{
public:
	GameManager();
	GameManager(const GameManager&);
	~GameManager();

	void Update();
	bool Initialize(InputClass *inputClass, MouseClass* mouseClass, D3DClass *d3d, GraphicsClass *graphicsClass);
	void LMBPressed();
	void SetDroneRotation(float mousePosX, float mousePosY);
	void SetDroneDestination(float destX, float destY);
	void CallDroneToPlayer();
	void StartGame();
	Player* GetPlayer();

	template <typename T>
	void AddNewEnemy(T type);

private:
	void PushNewEnemy(void* enemy);

private:
	InputClass* m_inputClass; //Singleton
	MouseClass* m_mouseClass;
	Player *player;
	EnemyWanderer* enemyWanderer;
	EnemyFlying* enemyFlying;
	EnemyArcher* enemyArcher;
	DroneController* droneController;

	std::vector<EnemyWanderer*> m_enemyWanderer;
	std::vector<EnemyFlying*> m_enemyFlying;
	std::vector<EnemyArcher*> m_enemyArcher;

	UIController* healthBarBackground;
	UIController* healthBar;
	UIController* progressBarBackground;
	UIController* progressBar;
	UIController* menuStartGame;
	UIController* menuQuit;
	UIController* menuTitle;
};

template<typename T>
inline void GameManager::AddNewEnemy(T type)
{
	if (type == nullptr || type == 0)
		return;

	PushNewEnemy(type);
}
#endif // !_GAMEMANAGER_H_