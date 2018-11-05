///////////////////////////////
#include "player.h"
#include "inputclass.h"
#include "enemywanderer.h"

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
	EnemyWanderer* enemy;
};