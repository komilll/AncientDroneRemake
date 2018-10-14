#pragma once
///Class for on-screen character input and gameplay values

class Player
{
public:
	Player();
	Player(const Player&);
	~Player();

	void Initialize();

private:
	void OnDestroy();


};