#pragma once

#ifndef _UI_CONTROLLER_H_
#define _UI_CONTROLLER_H_

#include "modelclass.h"
#include "textureshadergeneralclass.h"
#include "graphicsclass.h"

class UIController 
{
public:
	UIController();

	void Init(GraphicsClass* graphicsClass, float width, float height, float originalWidth, float originalHeight, float positionX, float positionY, CHAR* filename, bool transparent = true);

private:
	ModelClass* m_model;
	TextureShaderGeneralClass* m_shader;

};


#endif // !_UI_CONTROLLER_H_