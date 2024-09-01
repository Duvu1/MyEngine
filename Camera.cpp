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
	Vector3 toCamera = -GetViewDirection();
	toCamera.Normalize();

	m_cameraDistance += mouseWheelDirection * m_cameraDistance * 0.2f;
	m_cameraDistance = clamp(m_cameraDistance, 0.01f, 200.0f);

	toCamera *= m_cameraDistance;
	m_cameraPosition = m_viewLookAt + toCamera;
}

void Camera::SetCameraPosition(Vector2 dMousePos)
{
	m_cameraPosition -= m_viewRight * dMousePos.x;
	m_cameraPosition -= m_viewVertical * dMousePos.y;
	m_viewLookAt -= m_viewRight * dMousePos.x;
	m_viewLookAt -= m_viewVertical * dMousePos.y;

	//printf("dMousePos %f %f\n", dMousePos.x, dMousePos.y);
	//printf("viewVertical %f %f %f\n", m_viewVertical.x, m_viewVertical.y, m_viewVertical.z);
	//printf("viewRight %f %f %f\n", m_viewRight.x, m_viewRight.y, m_viewRight.z);
}

void Camera::SetCameraRotation(Vector2 dMousePos)
{
	Vector3 toCamera = -GetViewDirection() * m_cameraDistance;

	Quaternion q;

	q = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), dMousePos.x);
	Matrix yaw = Matrix::CreateFromQuaternion(q);

	q = Quaternion::CreateFromAxisAngle(m_viewRight, -dMousePos.y);
	Matrix pitch = Matrix::CreateFromQuaternion(q);

	Matrix rotationMatrix = yaw * pitch;

	toCamera = Vector3::Transform(toCamera, rotationMatrix);

	m_cameraPosition = m_viewLookAt + toCamera;
	
	m_viewDir.Normalize();
	m_viewDir = Vector3::Transform(m_viewDir, rotationMatrix);

	m_viewVertical.Normalize();
	m_viewVertical = Vector3::Transform(m_viewVertical, rotationMatrix);

	m_viewRight = m_viewVertical.Cross(-toCamera);
	m_viewRight.Normalize();
	m_viewRight = Vector3::Transform(m_viewRight, rotationMatrix);

	//printf("cameraPosition %f %f %f\n", m_cameraPosition.x, m_cameraPosition.y, m_cameraPosition.z);
	//printf("viewVertical %f %f %f\n", m_viewVertical.x, m_viewVertical.y, m_viewVertical.z);
	//printf("viewRight %f %f %f\n", m_viewRight.x, m_viewRight.y, m_viewRight.z);
}

Matrix Camera::GetViewMatrix()
{
	Matrix curViewMatrix;

	curViewMatrix = XMMatrixLookAtLH(m_cameraPosition, m_viewLookAt, m_viewVertical);
	//m_viewMatrix = curViewMatrix * m_viewMatrix;

	return curViewMatrix;
}

Matrix Camera::GetProjectionMatrix()
{
	Matrix projectionMatrix;

	projectionMatrix = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fieldOfViewAngle), m_aspectRatio, m_nearZ, m_farZ);

	return projectionMatrix;
}
