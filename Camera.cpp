#include "Camera.h"

#include <algorithm>

using namespace std;

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update()
{

}

void Camera::SetCameraDistance(int mouseWheelDirection)
{
	m_cameraDistance += mouseWheelDirection * m_cameraDistance * 0.2f;
	m_cameraDistance = clamp(m_cameraDistance, 0.01f, 200.0f);
}

void Camera::SetCameraPosition(Vector2 dMousePos)
{
	dMousePos.x *= m_viewRotateDirection;

	m_cameraPositionAngle.x += dMousePos.x;
	m_cameraPositionAngle.y += dMousePos.y;

	//printf("cameraPositionAngle %f %f\n", m_cameraPositionAngle.x, m_cameraPositionAngle.y);
}

void Camera::SetUpDirection()
{
	if (cos(m_cameraPositionAngle.y) > 0)
		m_viewUp.y = 1.0f;
	else
		m_viewUp.y = -1.0f;
}

void Camera::SetRotateDirection()
{

	if (cos(m_cameraPositionAngle.y) > 0)
		m_viewRotateDirection = -1;
	else
		m_viewRotateDirection = 1;
}

Matrix Camera::GetViewMatrix()
{
	Matrix viewMatrix;

	m_cameraPosition.x = m_cameraDistance * cos(m_cameraPositionAngle.y) * sin(m_cameraPositionAngle.x);
	m_cameraPosition.y = -m_cameraDistance * sin(m_cameraPositionAngle.y);
	m_cameraPosition.z = -m_cameraDistance * cos(m_cameraPositionAngle.y) * cos(m_cameraPositionAngle.x);

	viewMatrix = XMMatrixLookAtLH(m_cameraPosition, m_viewLookAt, m_viewUp);

	return viewMatrix;
}

Matrix Camera::GetProjectionMatrix()
{
	Matrix projectionMatrix;

	projectionMatrix = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fieldOfViewAngle), m_aspectRatio, m_nearZ, m_farZ);

	return projectionMatrix;
}
