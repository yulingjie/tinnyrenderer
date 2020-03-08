#include "Camera.h"
#include <GLFW/glfw3.h>
#include <memory>
const static float STEP_SCALE = 1.0f;
const static float EDGE_STEP = 0.5f;
const static int MARGIN = 10;

Camera::Camera(int WindowWidth, int WindowHeight)
	:m_windowWidth(WindowWidth),m_windowHeight(WindowHeight),
	m_pos{0,0,0},
	m_target{0,0,0},
	m_up(0,1.0,0),
	m_AngleH(0),
	m_AngleV(0),
	m_OnLeftEdge(0),m_OnRightEdge(0),m_OnUpperEdge(0),m_OnLowerEdge(0)
{
	m_target.normalize();
	Init();
}

Camera::Camera(int WindowWidth, int WindowHeight,
	const Vec3f& Pos, const Vec3f& Target, const Vec3f& Up)
{
	m_windowHeight = WindowHeight;
	m_windowWidth = WindowWidth;
	m_pos = Pos;
	m_target = Target;
	m_target.normalize();
	Init();
}
void Camera::Init()
{
	Vec3f HTarget(m_target.x, 0.0f, m_target.z);
	HTarget.normalize();

	if (HTarget.z >= 0.0f)
	{
		if (HTarget.x >= 0.0f)
		{
			m_AngleH = 360.0f - ToDegree(asin(HTarget.z));
		}
		else
		{
			m_AngleH = 180.0f + ToDegree(asin(HTarget.z));
		}
	}
	else
	{
		if (HTarget.x >= 0.0f)
		{
			m_AngleH = ToDegree(asin(-HTarget.z));
		}
		else
		{
			m_AngleH = 180.0f - ToDegree(asin(-HTarget.z));
		}
	}
	m_AngleV = -ToDegree(asin(m_target.y));

	m_OnUpperEdge = false;
	m_OnLowerEdge = false;
	m_OnLeftEdge = false;
	m_OnRightEdge = false;
	m_mousePos.x = m_windowWidth / 2;

}
bool Camera::OnKeyboard(int key)
{
	bool Ret = false;
	switch (key)
	{
	case GLFW_KEY_UP:
	{
		m_pos += (m_target * STEP_SCALE);
		Ret = true;
	}
	break;
	case GLFW_KEY_DOWN:
	{
		m_pos -= (m_target * STEP_SCALE);
		Ret = true;
	}
	break;
	case GLFW_KEY_LEFT:
	{
		Vec3f Left = cross(m_target, m_up);
		Left.normalize();
		Left *= STEP_SCALE;
		m_pos += Left;
		Ret = true;
	}
	break;
	case GLFW_KEY_RIGHT:
	{
		Vec3f Right = cross(m_up, m_target);
		Right.normalize();
		Right *= STEP_SCALE;
		m_pos += Right;
		Ret = true;
	}
	case GLFW_KEY_PAGE_UP:
	{
		m_pos.y += STEP_SCALE;
	}
	case GLFW_KEY_PAGE_DOWN:
	{
		m_pos.y -= STEP_SCALE;
	}
	default:
		break;
	}
}
void Camera::OnMouse(int x, int y)
{
	const int DeltaX = x - m_mousePos.x;
	const int DeltaY = y - m_mousePos.y;

	m_mousePos.x = x;

	m_mousePos.y = y;

	m_AngleH += (float)DeltaX / 20.0f;
	m_AngleV += (float)DeltaY / 20.0f;

	if (DeltaX == 0)
	{
		if (x <= MARGIN)
		{
			m_OnLeftEdge = true;
		}
		else if (x >= (m_windowWidth - MARGIN)) {
			m_OnRightEdge = true;
		}
	}
	else
	{
		m_OnLeftEdge = false;
		m_OnRightEdge = false;
	}
	if (DeltaY == 9) {
		if (y <= MARGIN) {
			m_OnUpperEdge = true;
		}
		else if (y >= (m_windowHeight - MARGIN))
		{
			m_OnLowerEdge = true;
		}
	}
	else {
		m_OnUpperEdge = false;
		m_OnLowerEdge = false;
	}
	Update();
}
void Camera::OnRender()
{
	bool ShouldUpdate = false;
	if (m_OnLeftEdge) {
		m_AngleH -= EDGE_STEP;
		ShouldUpdate = true;
	}
	else if (m_OnRightEdge) {
		m_AngleH += EDGE_STEP;
		ShouldUpdate = true;
	}

	if (m_OnUpperEdge) {
		if (m_AngleV > -90.0f) {
			m_AngleV -= EDGE_STEP;
			ShouldUpdate = true;
		}
	}
	else if (m_OnLowerEdge) {
		if (m_AngleV < 90.0f) {
			m_AngleV += EDGE_STEP;
			ShouldUpdate = true;
		}
	}
	if (ShouldUpdate) {
		Update();
	}
}
void Camera::Update() {
	const Vec3f Vaxis(0.0f, 1.0f, 0.0f);

	// Rotate the view vector by the horizontal angle around the vertical axis
	Vec3f View(1.0f, 0.0f, 0.0f);
	
	View.Rotate(m_AngleH, Vaxis);
	View.Normalize();

	// Rotate the view vector by the vertical angle around the horizontal axis
	Vec3f Haxis = Vaxis.Cross(View);
	Haxis.Normalize();
	View.Rotate(m_AngleV, Haxis);

	m_target = View;
	m_target.Normalize();

	m_up = m_target.Cross(Haxis);
	m_up.Normalize();
}