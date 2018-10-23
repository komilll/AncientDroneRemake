#pragma once
#ifndef _ANIMATIONIMPORTER_H_
#define _ANIMATIONIMPORTER_H_

#include <d3d11.h>
#include <d3dx10math.h>


class AnimationImporter
{
public:
	AnimationImporter();
	AnimationImporter(const AnimationImporter&);
	~AnimationImporter();

	bool ImportFile();

};

#endif