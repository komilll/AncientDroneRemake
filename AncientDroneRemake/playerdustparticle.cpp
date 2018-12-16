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
		if (!m_particleInUse.at(i))
			continue;

		if (now - m_spawnTime.at(i) >= m_liveTime)
		{
			//Stop updating here
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
	m_liveTimeLeft = 0;
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

		if (!shader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "dust_particles.dds"))
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

		shader->AddModel(model);
		model->SetVisibility(false);
		model->SetRotation(90.0f);
		SetNewAnimation(SIZE_1, i);
	}
}

void PlayerDustParticle::SpawnParticle(float movevementDirection)
{
	if (movevementDirection == 0)
		return;

	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - m_lastSpawnTime < m_spawnCooldown)
		return;

	m_lastSpawnTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	int dir = movevementDirection > 0 ? 1 : -1;

	//Create new shader for particles
		//--Move particle based on direction of player (to opposite side)
		//--Blend alpha based on live time

	//Create new pipeline class for particles

}