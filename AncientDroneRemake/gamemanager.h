///////////////////////////////
#include "player.h"
#include "inputclass.h"
#include "enemywanderer.h"
#include "enemyflying.h"
#include "enemyarcher.h"
#include "dronecontroller.h"
#include "uicontroller.h"

class GameManager
{
public:
	GameManager();
	GameManager(const GameManager&);
	~GameManager();

	void Update();
	bool Initialize(InputClass *inputClass, D3DClass *d3d, GraphicsClass *graphicsClass);
	void LMBPressed();
	void SetDroneRotation(float mousePosX, float mousePosY);
	void SetDroneDestination(float destX, float destY);

private:
	InputClass* m_inputClass; //Singleton
	Player *player;
	EnemyWanderer* enemyWanderer;
	EnemyFlying* enemyFlying;
	EnemyArcher* enemyArcher;
	DroneController* droneController;
	UIController* healthBarBackground;
};
