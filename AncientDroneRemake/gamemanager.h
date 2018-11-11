///////////////////////////////
#include "player.h"
#include "inputclass.h"
#include "enemywanderer.h"
#include "enemyflying.h"
#include "enemyarcher.h"

class GameManager
{
public:
	GameManager();
	GameManager(const GameManager&);
	~GameManager();

	void Update();
	bool Initialize(InputClass *inputClass, D3DClass *d3d, GraphicsClass *graphicsClass);




private:
	InputClass* m_inputClass; //Singleton
	Player *player;
	EnemyWanderer* enemyWanderer;
	EnemyFlying* enemyFlying;
	EnemyArcher* enemyArcher;
};
