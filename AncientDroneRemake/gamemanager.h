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
#include "darksphere.h"
#include "levelfinish.h"
#include <functional>
#include <typeinfo>
#include <type_traits>

enum EnemyType {
	G_WANDERER, G_ARCHER, G_FLYING
};

class GameManager
{
public:
	GameManager();
	GameManager(const GameManager&);
	~GameManager();

	void Update();
	bool Initialize(InputClass *inputClass, MouseClass* mouseClass, D3DClass *d3d, GraphicsClass *graphicsClass);
	void LMBPressed();
	void RMBPressed();
	void SetDroneRotation(float mousePosX, float mousePosY);
	void SetDroneDestination(float destX, float destY);
	void CallDroneToPlayer();
	Player* GetPlayer();
	bool SpawnObjects();
	UIController* GetMenuStartGame();
	void SetMenuStartGame();
	std::function<void()> StartGameBuildLevel;
	void StartGame();
	void RestartLevel();

	void AddNewEnemy(EnemyType type, void* enemy);
	void* GetEnemy(EnemyType type, int index);

	void AddDarkSphere(DarkSphere* darkSphere);
	
	void SetLevelFinish(LevelFinish* levelFinish);

	bool CheckNextLevel();

private:
	void PushNewEnemy(EnemyType enemyType, void* enemy);
	void* GetEnemyLocal(EnemyType enemyType, int index);
	void DestroyLevel();

private:
	InputClass* m_inputClass; //Singleton
	MouseClass* m_mouseClass;
	GraphicsClass* m_graphics;
	Player *player;
	EnemyWanderer* enemyWanderer;
	EnemyFlying* enemyFlying;
	EnemyArcher* enemyArcher;
	DroneController* droneController;

	std::vector<EnemyWanderer*> m_enemyWanderer;
	std::vector<EnemyFlying*> m_enemyFlying;
	std::vector<EnemyArcher*> m_enemyArcher;
	std::vector<DarkSphere*> m_darkSphere;
	LevelFinish* m_levelFinish;

	UIController* healthBarBackground;
	UIController* healthBar;
	UIController* progressBarBackground;
	UIController* progressBar;
	UIController* menuStartGame;
	UIController* menuQuit;
	UIController* menuTitle;

	bool m_gameStarted = false;
};
#endif // !_GAMEMANAGER_H_