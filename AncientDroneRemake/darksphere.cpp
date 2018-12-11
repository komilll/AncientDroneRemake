#include "darksphere.h"

DarkSphere::DarkSphere()
{
	lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

bool DarkSphere::Init(GraphicsClass * graphicsClass, float translationX, float translationY)
{
	m_currentScale = m_minScale;

	if ((m_graphics = graphicsClass) == nullptr)
		return false;

	////Base model
	//m_modelBase = new ModelClass();
	//if (!m_modelBase)
	//	return false;

	//m_modelBase->Initialize(m_graphics->GetD3D()->GetDevice(), 4, 4);
	//m_modelBase->SetTranslation(translationX, translationY, 0.0f);

	//m_shader = new TextureShaderGeneralClass();
	//if (!m_shader)
	//	return false;

	//if (!m_shader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "DarkSphere_Base.dds"))
	//	return false;

	//if (!m_graphics->AddTextureShaderGeneral(m_shader))
	//{
	//	m_shader->Shutdown();
	//	delete m_shader;
	//	m_shader = 0;
	//}

	//m_shader->AddModel(m_modelBase);
	//m_shader->SetAsTransparent(true);
	//Lerped model
	m_modelLerped = new ModelClass();
	if (!m_modelLerped)
		return false;

	m_modelLerped->Initialize(m_graphics->GetD3D()->GetDevice(), 4, 4);
	m_modelLerped->SetTranslation(translationX, translationY, 0.0f);

	m_shader = new TextureShaderGeneralClass();
	if (!m_shader)
		return false;

	if (!m_shader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "DarkSphere_Core.dds", "darksphere.vs", "darksphere.ps"))
		return false;

	if (!m_graphics->AddTextureShaderGeneral(m_shader))
	{
		m_shader->Shutdown();
		delete m_shader;
		m_shader = 0;
	}

	m_shader->AddModel(m_modelLerped);
	m_shader->SetAsTransparent(true);
}

void DarkSphere::Update()
{
	__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	timer += (now - lastTime);
	lastTime = now;

	if (timer >= 20)
	{
		m_sphereTimer += 20;
		if (m_isActive && m_sphereTimer >= m_duration)
		{
			m_sphereTimer = 0;
			m_isActive = false;
			m_currentScale = m_minScale;
		}
		else if (!m_isActive && m_sphereTimer >= m_cooldown)
		{
			m_sphereTimer = 0;
			m_isActive = true;
			StartUsing();
		}
		FixedUpdate();
		timer -= 20;
	}
}

void DarkSphere::FixedUpdate()
{
	float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	random *= (m_maxRandom * 2.0f);
	random -= m_maxRandom;
	m_modelLerped->SetScale(m_currentScale + random, m_currentScale + random, 1.0f);
}

bool DarkSphere::TouchedPlayer(Player * player, float playerMinX, float playerMaxX, float playerMinY, float playerMaxY)
{
	if (!m_isActive)
		return false;

	if ((m_modelLerped->GetBounds().min.x < playerMaxX && playerMaxX < m_modelLerped->GetBounds().max.x) || //Enter from the left side		
		(m_modelLerped->GetBounds().max.x > playerMinX && playerMinX > m_modelLerped->GetBounds().min.x)) //Enter from the right side
	{
		if (playerMinY < m_modelLerped->GetBounds().max.y && playerMaxY > m_modelLerped->GetBounds().max.y || //Enter from the bottom
			playerMaxY > m_modelLerped->GetBounds().min.y && m_modelLerped->GetBounds().max.y > playerMinY) //Enter from the top
		{
			player->DealDamage(1, m_modelLerped->GetTranslation(), 0.5f);
			return true;
		}
	}

	return false;
}

void DarkSphere::StartUsing()
{
	m_currentScale = m_maxScale;
}