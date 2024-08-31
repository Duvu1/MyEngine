#pragma once

#include "framework.h"

class Camera
{
public:
	Camera();
	~Camera();

	void Update();

	void SetCameraDistance(int mouseWheel);
	void SetCameraPosition(Vector2 dMousePos);
	void SetUpDirection();
	void SetRotateDirection();

	Matrix GetViewMatrix();
	Matrix GetProjectionMatrix();

	Vector3 GetCameraPosition() { return m_cameraPosition; };
	Vector3 GetViewDirection() { return m_viewLookAt - m_cameraPosition; };

	void SetAspectRatio(float width, float height) { m_aspectRatio = width / height; };
	float GetAspectRatio() { return m_aspectRatio; };

public:
	//Matrix m_viewMatrix = Matrix();
	//Matrix m_projectionMatrix = Matrix();

	// view
	int m_viewRotateDirection = -1; // 보통 회전:-1, 화면 상하 반전 시 회전:1
	float m_cameraDistance = 5.0f;
	float m_zoomLevel = 1.0f;

	Vector3 m_cameraPosition = { 0.5f, 0.5f, -m_cameraDistance };
	Vector2 m_cameraPositionAngle = { 0.0f, 0.0f };
	Vector3 m_viewLookAt = { 0.0f, 0.0f, 0.0f };
	Vector3 m_viewDir = { 0.0f, 0.0f, 1.0f };
	Vector3 m_viewUp = { 0.0f, 1.0f, 0.0f };

	Vector3 m_viewVertical = { 0.0f, 1.0f, 0.0f };
	Vector3 m_viewRight = { 1.0f, 0.0f, 0.0f };

	float m_fieldOfViewAngle = 90.0f;
	float m_nearZ = 0.01f;  // not used
	float m_farZ = 100.0f;  // not used
	float m_aspectRatio = 1.0f; // not used
};

