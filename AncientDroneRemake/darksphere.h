#pragma once
#ifndef _DARK_SPHERE_H_
#define _DARK_SPHERE_H_

#include "graphicsclass.h"

class DarkSphere 
{
public:
	DarkSphere();

	bool Init(GraphicsClass* graphicsClass, float translationX, float translationY);


private:	
	GraphicsClass* m_graphics;
	ModelClass* m_modelBase;
	ModelClass* m_modelLerped;
	TextureShaderGeneralClass* m_shader;
};


#endif // !_DARK_SPHERE_H_