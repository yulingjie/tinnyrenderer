#pragma once
#include "geometry.h"
class Camera
{
public:

	Camera(int WindowWidth, int WindowHeight);

	Camera(int WindowWidth, int WindowHeight, 
		const Vec3f& Pos, 
		const Vec3f& Target,
		const Vec3f& Up);

	bool OnKeyboard(int Key);

	void OnMouse(int x, int y);

	void OnRender();

	const Vec3f& GetPos() const
	{
		return m_pos;
	}

	const Vec3f& GetTarget() const
	{
		return m_target;
	}

	const Vec3f& GetUp() const
	{
		return m_up;
	}

	//void AddToATB(TwBar* bar);

private:

	void Init();
	void Update();

	Vec3f m_pos;
	Vec3f m_target;
	Vec3f m_up;

	int m_windowWidth;
	int m_windowHeight;

	float m_AngleH;
	float m_AngleV;

	bool m_OnUpperEdge;
	bool m_OnLowerEdge;
	bool m_OnLeftEdge;
	bool m_OnRightEdge;

	Vec3f m_mousePos;
};