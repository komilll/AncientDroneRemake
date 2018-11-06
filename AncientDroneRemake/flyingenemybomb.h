#pragma once

class FlyingEnemyBomb
{
public:
	FlyingEnemyBomb();

	void Update();

private:
	bool m_isExploding = false;
	bool m_used = false;

	float m_explosionRadius;
	float m_explosionTime;
	float m_explosionMinScale;
	float m_explosionMaxScale;
};