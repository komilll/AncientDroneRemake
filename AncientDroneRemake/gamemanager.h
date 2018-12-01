///////////////////////////////
#include "player.h"
#include "inputclass.h"
#include "enemywanderer.h"
#include "enemyflying.h"
#include "enemyarcher.h"
#include "dronecontroller.h"
#include "uicontroller.h"
#include "mouseclass.h"
#include <functional>

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
	void StartGame();
	Player* GetPlayer();

private:
	InputClass* m_inputClass; //Singleton
	MouseClass* m_mouseClass;
	Player *player;
	EnemyWanderer* enemyWanderer;
	EnemyFlying* enemyFlying;
	EnemyArcher* enemyArcher;
	DroneController* droneController;

	UIController* healthBarBackground;
	UIController* healthBar;
	UIController* progressBarBackground;
	UIController* progressBar;
	UIController* menuStartGame;
	UIController* menuQuit;
	UIController* menuTitle;
};
