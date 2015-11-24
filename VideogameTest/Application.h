#pragma once

#include "Camera\SphericalCameraController.h"
#include "Camera\FPSCameraController.h"
#include "HelperTypes.h"
#include "Render\RenderManager.h"
#include "Material\MaterialManager.h"
#include "Render\RenderableObject.h"

class CDebugRender;
class CContextManager;
class CRenderManager;
class CMaterialManager;

class CApplication
{
public:
	CApplication(CDebugRender *_DebugRender, CContextManager *_ContextManager);
	~CApplication();

	void Init();
	void SwitchCamera();

	void Update(float _ElapsedTime);
	void Render();

private:

	CSphericalCameraController m_SphericalCamera;
	CFPSCameraController m_FPSCamera;

	int m_CurrentCamera;

	CDebugRender *m_DebugRender;
	CContextManager *m_ContextManager;
	CRenderManager m_RenderManager;
	CMaterialManager m_MaterialManager;

	CColor m_BackgroundColor;
	CRenderableObject m_Cube;
};

