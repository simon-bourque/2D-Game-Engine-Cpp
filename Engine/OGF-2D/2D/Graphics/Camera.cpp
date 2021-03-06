#include "Camera.h"

Camera::Camera(const geo::Rectangle& viewPort) : Camera(Transform(), viewPort) {}

Camera::Camera(const Transform& transform, const geo::Rectangle& viewPort) : transform(transform), m_viewPort(viewPort) {
	m_projectionMatrix = Matrix3f::orthographic(-viewPort.getHalfWidth(), viewPort.getHalfWidth(), viewPort.getHalfHeight(), -viewPort.getHalfHeight(), 1, 1000);
}

Camera::~Camera() {}

void Camera::updateViewProjectionMatrix() {
	m_viewProjectionMatrix = m_projectionMatrix * getViewMatrix();
}

Matrix3f Camera::getViewMatrix() const {
	return transform.toMatrix().inverse();
}