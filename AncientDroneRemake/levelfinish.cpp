#include "levelfinish.h"

LevelFinish::LevelFinish()
{
}

bool LevelFinish::Init(GraphicsClass * graphicsClass, float translationX, float translationY)
{
	if ((m_graphics = graphicsClass) == nullptr)
		return false;

	m_model = new ModelClass();
	if (!m_model)
		return false;

	m_model->Initialize(m_graphics->GetD3D()->GetDevice(), 4, 4);
	m_model->SetTranslation(translationX, translationY, 0.0f);

	m_shader = new TextureShaderGeneralClass();
	if (!m_shader)
		return false;

	if (!m_shader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "DarkSphere_Core.dds", "endsphere.vs", "endsphere.ps"))
		return false;

	if (!m_graphics->AddTextureShaderGeneral(m_shader))
	{
		m_shader->Shutdown();
		delete m_shader;
		m_shader = 0;
	}

	m_shader->AddModel(m_model);
	m_shader->SetAsTransparent(true);
}

void LevelFinish::Update()
{
	__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	timer += (now - lastTime);
	lastTime = now;

	if (timer >= 20)
	{
		FixedUpdate();
		timer -= 20;
	}
}

void LevelFinish::FixedUpdate()
{
	float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	random *= (m_maxRandom * 2.0f);
	random -= m_maxRandom;
	m_model->SetScale(m_currentScale + random, m_currentScale + random, 1.0f);
}

bool LevelFinish::TouchedPlayer(Player * player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
{
	if ((m_model->GetBounds().min.x < playerMaxX && playerMaxX < m_model->GetBounds().max.x) || //Enter from the left side		
		(m_model->GetBounds().max.x > playerMinX && playerMinX > m_model->GetBounds().min.x)) //Enter from the right side
	{
		if (playerMinY < m_model->GetBounds().max.y && playerMaxY > m_model->GetBounds().max.y || //Enter from the bottom
			playerMaxY > m_model->GetBounds().min.y && m_model->GetBounds().max.y > playerMinY) //Enter from the top
		{
			m_nextLevelReady = true;
			return true;
		}
	}

	return false;
}

bool LevelFinish::ReadyForNextLevel()
{
	if (m_nextLevelReady)
	{
		m_nextLevelReady = false;
		return true;
	}
	return false;
}

void LevelFinish::Shutdown()
{
	m_graphics->RemoveTextureShaderGeneral(m_shader);
	if (m_model)
	{
		m_model->Shutdown();
		delete m_model;
		m_model = 0;
	}
	m_graphics = 0;
}
