#include "darksphere.h"

DarkSphere::DarkSphere()
{

}

bool DarkSphere::Init(GraphicsClass * graphicsClass, float translationX, float translationY)
{
	if ((m_graphics = graphicsClass) == nullptr)
		return false;

	m_modelBase = new ModelClass();
	if (!m_modelBase)
		return false;

	m_modelBase->Initialize(m_graphics->GetD3D()->GetDevice(), 16.0f, 16.0f);
	m_modelBase->SetTranslation(translationX, translationY, 0.0f);

	m_shader = new TextureShaderGeneralClass();
	if (!m_shader)
		return false;

	if (!m_shader->Initialize(m_graphics->GetD3D()->GetDevice(), *m_graphics->GetHWND(), "DarkSphere_Base.dds"))
		return false;

	if (!m_graphics->AddTextureShaderGeneral(m_shader))
	{
		m_shader->Shutdown();
		delete m_shader;
		m_shader = 0;
	}

	m_shader->AddModel(m_modelBase);
	m_shader->SetAsTransparent(true);
}
