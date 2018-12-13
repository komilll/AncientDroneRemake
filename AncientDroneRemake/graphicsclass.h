////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"
#include "textureshaderclass.h"
#include "textureshadergeneralclass.h"
#include <vector>
#include <algorithm>

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float SCREEN_WIDTH = 800.0f;
const float SCREEN_HEIGHT = 600.0f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	D3DClass *GetD3D();
	HWND *GetHWND();
	void SetPlayerModel(ModelClass* player);
	void SetPlayerAnimation(int index, TextureShaderClass* shader);
	void SetPlayerAnimationOneShot(int index, TextureShaderClass* shader, int previousIndex = -1);
	void AddEnemyModel(ModelClass* enemyModel);
	void AddBombModel(ModelClass* bombModel);
	void RemoveBombModel(ModelClass* bombModel);
	void ClearBombs();
	
	bool AddTextureShader(TextureShaderClass* textureShader);
	void RemoveTextureShader(TextureShaderClass* textureShader);

	bool AddTextureShaderGeneral(TextureShaderGeneralClass* textureShader);
	void RemoveTextureShaderGeneral(TextureShaderGeneralClass* textureShader);

	bool AddColorShader(ColorShaderClass* colorShader);
	void RemoveColorShader(ColorShaderClass* colorShader);

	ModelClass* AddGroundModel(int width, int height, float posX, float posY);
	ModelClass* GetGroundModel(int index);
	void ClearGrounds();
	int GetGroundModelCount();

	D3DXVECTOR3 GetPlayerPositionDiff();
	D3DXVECTOR3 GetPlayerPosition();
	ModelClass* GetPlayerModel();

private:
	bool Render();

private:
	HWND* m_hwnd;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ColorShaderClass* m_ColorShader;

	ModelClass* playerModel;
	ModelClass* m_backgroundModel;
	const static int GROUND_MODEL_LENGTH = 3;
	//ModelClass* groundModel[GROUND_MODEL_LENGTH];

	TextureShaderGeneralClass* m_TextureShaderBackground;
	std::vector<TextureShaderClass*> m_TextureShaders;
	std::vector<TextureShaderGeneralClass*> m_TextureShadersGeneral;
	std::vector<ColorShaderClass*> m_ColorShaders;
	std::vector<ModelClass*> m_enemyModels;
	std::vector<ModelClass*> m_bombModels;
	std::vector<ModelClass*> m_groundModels;

	int a = 0;
	int index = 0;

	D3DXVECTOR3 m_lastFrameCameraPosition;
	D3DXVECTOR3 m_lastPlayerPosition;
	D3DXVECTOR3 m_playerPosDiff;
};

#endif