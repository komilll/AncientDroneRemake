#include "playerdustparticle.h"

PlayerDustParticle::PlayerDustParticle()
{
}

void PlayerDustParticle::SetNewAnimation(int newState)
{
}

void PlayerDustParticle::Update()
{
	__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	for (int i = 0; i < m_particleInUse.size(); i++)
	{
		if (m_particleInUse.at(i) == false)
			continue;

		__int64 time = now - m_spawnTime.at(i);
		if (now - m_spawnTime.at(i) >= m_liveTime)
		{
			m_particleInUse.at(i) = false;
			m_particles.at(i)->SetVisibility(false);
			//Stop updating here
		}
		else
		{
			float diff = (now - m_spawnTime.at(i));
			float val = (diff / (float)m_liveTime);
			m_shaders.at(i)->SetColorTint(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.8f - val * val));
		}
	}

	//Update for all particles
		//--Catch when particle is done
			//--Close it
			//--Set invisible
			//--Set as available to spawn
}

void PlayerDustParticle::SetNewAnimation(int newState, int index)
{
	if (newState == m_animations.at(index)->GetCurrentState())
		return;

	m_animations.at(index)->SetState(newState);
	m_graphics->SetPlayerAnimation(m_animations.at(index)->GetCurrentState(), m_shaders.at(index));
}

bool PlayerDustParticle::Initialize(GraphicsClass* graphicsClass, int numberOfParticles)
{
	m_particleIndex = 0;
	m_lastSpawnTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	m_graphics = graphicsClass;
	if (m_graphics == nullptr)
		return false;
	
	for (int i = 0; i < numberOfParticles; i++)
	{
		ModelClass* model = new ModelClass();
		TextureShaderClass* shader = new TextureShaderClass();
		PlayerAnimationStates* animation = new PlayerAnimationStates(5);

		if (!model)
			return false;

		shader = new TextureShaderClass();
		if (!shader)
			return false;

		if (!shader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "dust_particles.dds", "particles.vs", "particles.ps"))
			return false;

		if (!m_graphics->AddTextureShader(shader))
		{
			shader->Shutdown();
			delete shader;
			shader = 0;
		}

		model->Initialize(m_graphics->GetD3D()->GetDevice(), 4, 4);	

		if (shader)
			shader->SetAnimationObject(this);

		shader->ImportFile(16, 16, 96, 96);
		shader->CreateNewAnimation(1, 5, 0);
		shader->CreateNewAnimation(1, 5, 1);
		shader->CreateNewAnimation(1, 5, 2);
		shader->CreateNewAnimation(1, 5, 3);
		shader->CreateNewAnimation(1, 5, 4);

		animation->PrepareAnimationPose(SIZE_1, SIZE_1);
		animation->PrepareAnimationPose(SIZE_2, SIZE_2);
		animation->PrepareAnimationPose(SIZE_3, SIZE_3);
		animation->PrepareAnimationPose(SIZE_4, SIZE_4);
		animation->PrepareAnimationPose(SIZE_5, SIZE_5);

		m_animations.push_back(animation);
		m_shaders.push_back(shader);
		m_particles.push_back(model);
		m_particleInUse.push_back(false);
		m_spawnTime.push_back(m_lastSpawnTime);

		shader->AddModel(model);
		shader->SetUseAlphaSourceBlending(true);
		model->SetVisibility(false);
		model->SetRotation(90.0f);
		SetNewAnimation(SIZE_1, i);
	}
}

void PlayerDustParticle::SpawnParticle(D3DXVECTOR3 spawnPos, float movevementDirection)
{
	if (movevementDirection == 0)
		return;

	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - m_lastSpawnTime < m_spawnCooldown)
		return;

	m_lastSpawnTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	int dir = movevementDirection > 0 ? 1 : -1;

	//for (int i = 0; i < 3; i++)
	{
		float randomX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 5.0f - 2.0f;
		float randomY = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 4.0f - 2.0f;
		int randomAnim = (std::rand() % 5);
		float randomScale = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 0.15f;
		SetNewAnimation(randomAnim, m_particleIndex);
		m_particles.at(m_particleIndex)->SetScale(randomScale, randomScale, randomScale);
		m_particleInUse.at(m_particleIndex) = true;
		m_particles.at(m_particleIndex)->SetTranslation(spawnPos.x - dir * 5.0f + randomX, spawnPos.y - 10.0f + randomY, spawnPos.z);
		m_particles.at(m_particleIndex)->SetVisibility(true);
		m_shaders.at(m_particleIndex)->SetColorTint(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.8f));
		m_spawnTime.at(m_particleIndex) = m_lastSpawnTime;
		m_particleIndex++;
		m_particleIndex %= m_particles.size();
	}
	//Create new shader for particles
		//--Move particle based on direction of player (to opposite side)
		//--Blend alpha based on live time

	//Create new pipeline class for particles

}