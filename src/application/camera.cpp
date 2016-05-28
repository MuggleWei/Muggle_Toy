#include "camera.h"
#include "utility/timer.h"
#include "application.h"
#include "window.h"
#include "input.h"
#include "utility/math_utils.h"

NS_MUGGLE_BEGIN

Camera::Camera()
	: m_view_matrix(matrix4f::identify)
	, m_projection_matrix(matrix4f::identify)
	, m_direction(vec3f::forward)
	, m_right(vec3f::right)
	, m_up(vec3f::up)
	, m_position(vec3f(0.0f, 0.0f, -10.0f))
	, m_target_position(vec3f::zero)
	, m_rotate_y_axis(0.0f)
	, m_rotate_x_axis(0.0f)
	, m_fov(60.0f)
	, m_aspect(1.333f)
	, m_z_near(0.1f)
	, m_z_far(1000.0f)
	, m_move_speed(1.0f)
	, m_rotate_speed(0.002f)
	, m_drag_rotate_speed(0.005f)
	, m_last_mouse_pos_x(0)
	, m_last_mouse_pos_y(0)
	, m_absolute_free(false)
	, m_dirty(true)
{}
Camera::~Camera()
{}

const matrix4f& Camera::getViewMatrix()
{
	return m_view_matrix;
}
const matrix4f& Camera::getProjectionMatrix()
{
	return m_projection_matrix;
}

void Camera::Update()
{
	// get delta time
	double delta_time = Timer::GetMilliseconds() * 0.00002;

	// get delta pos
	vec2i mouse_pos = Input::GetMousePos();
	int delta_x_pos = mouse_pos.x - m_last_mouse_pos_x;
	int delta_y_pos = mouse_pos.y - m_last_mouse_pos_y;
	m_last_mouse_pos_x = mouse_pos.x;
	m_last_mouse_pos_y = mouse_pos.y;

	if (Input::GetMouse(eMouseButton::Right))
	{
		// rotate direction
		RotateDirection((float)delta_x_pos, (float)-delta_y_pos);
		m_dirty = true;
	}
	if (Input::GetMouse(eMouseButton::Left))
	{
		if (Input::GetKey(eKeyCode::LAlt))
		{
			// rotate around target
			RotateAroundTarget((float)delta_x_pos, (float)-delta_y_pos);
			m_dirty = true;
		}
	}
	if (Input::GetMouse(eMouseButton::Middle))
	{
		// drag
		Drag(delta_x_pos, -delta_y_pos);
		m_dirty = true;
	}
	// move forward
	if (Input::GetKey(eKeyCode::Up))
	{
		m_dirty = true;
		m_position += m_direction * (float)delta_time * m_move_speed;
	}
	// move backward
	if (Input::GetKey(eKeyCode::Down))
	{
		m_dirty = true;
		m_position -= m_direction * (float)delta_time * m_move_speed;
	}
	// move right
	if (Input::GetKey(eKeyCode::Right))
	{
		m_dirty = true;
		m_position += m_right * (float)delta_time * m_move_speed;
		// m_target_position += m_right * (float)delta_time * m_move_speed;
	}
	// move left
	if (Input::GetKey(eKeyCode::Left))
	{
		m_dirty = true;
		m_position -= m_right * (float)delta_time * m_move_speed;
		// m_target_position -= m_right * (float)delta_time * m_move_speed;;
	}
	// move forward
	int16_t scroll = Input::GetMouseWheel();
	if (scroll != 0)
	{
		scroll = scroll > 0 ? 1 : -1;
		m_dirty = true;
		m_position += m_direction * (float)scroll * m_move_speed;
	}

	if (m_dirty)
	{
		UpdateMatrix();
	}
}

void Camera::setFOV(float fov)
{
	m_fov = fov;
	m_dirty = true;
}
void Camera::setAspect(float aspect)
{
	m_aspect = aspect;
	m_dirty = true;
}
void Camera::setNearPlane(float z_near)
{
	m_z_near = z_near;
	m_dirty = true;
}
void Camera::setFarPlane(float z_far)
{
	m_z_far = z_far;
	m_dirty = true;
}

vec3f Camera::getPosition()
{
	return m_position;
}
float Camera::getFOV()
{
	return m_fov;
}
float Camera::getAspect()
{
	return m_aspect;
}
float Camera::getNearPlane()
{
	return m_z_near;
}
float Camera::getFarPlane()
{
	return m_z_far;
}

void Camera::UpdateMatrix()
{
	m_dirty = false;

	int width, height;
	Window* win = GetApplication()->getWindow();
	if (win)
	{
		win->getWinSize(width, height);
	}
	m_aspect = (float)width / (float)height;
	
	m_view_matrix = MathUtils::LookAt(m_position, m_position + m_direction, m_up);
	m_projection_matrix = MathUtils::Perspective(Math::DegreeToRad(m_fov), m_aspect, m_z_near, m_z_far);
}

void Camera::RotateDirection(float delta_x_pos, float delta_y_pos)
{
	m_rotate_y_axis += m_rotate_speed * delta_x_pos;
	m_rotate_x_axis += m_rotate_speed * delta_y_pos;

	float cos_rx_value = Math::Cos(m_rotate_x_axis);
	float sin_rx_value = Math::Sin(m_rotate_x_axis);
	float cos_ry_value = Math::Cos(m_rotate_y_axis);
	float sin_ry_value = Math::Sin(m_rotate_y_axis);

	// convert Spherical coordinates to Cartesian coordinates
	m_direction = vec3f(
		cos_rx_value * sin_ry_value,
		sin_rx_value,
		cos_rx_value * cos_ry_value
	);
	m_right = vec3f(
		cos_ry_value,
		0.0f,
		-sin_ry_value
	);
	m_up = vec3f::Cross(m_direction, m_right);
}
void Camera::RotateAroundTarget(float delta_x_pos, float delta_y_pos)
{
	float rotate_y_axis = m_drag_rotate_speed * (float)delta_x_pos;
	float rotate_x_axis = -m_drag_rotate_speed * (float)delta_y_pos;

	float cos_half_ry_value = Math::Cos(rotate_y_axis / 2);
	float sin_half_ry_value = Math::Sin(rotate_y_axis / 2);
	float cos_half_rx_value = Math::Cos(rotate_x_axis / 2);
	float sin_half_rx_value = Math::Sin(rotate_x_axis / 2);

	muggle::vec3f around_up = muggle::vec3f(0.0f, 1.0f, 0.0f);
	if (m_absolute_free)
	{
		around_up = m_up;
	}

	quatf qy = quatf(cos_half_ry_value, sin_half_ry_value*around_up.x, sin_half_ry_value*around_up.y, sin_half_ry_value*around_up.z);
	quatf qx = quatf(cos_half_rx_value, sin_half_rx_value*m_right.x, sin_half_rx_value*m_right.y, sin_half_rx_value*m_right.z);
	quatf q = qy * qx;

	q.Normalize();
	
	// position
	vec3f v = m_position - m_target_position;
	v = q * v;
	m_position = v + m_target_position;
	
	// direction
	m_direction = q * m_direction;
	m_right = q * m_right;
	m_up = q * m_up;
	
	// convert Cartesian coordinates to Spherical coordinates
	m_rotate_x_axis = Math::ASin(m_direction.y);
	m_rotate_y_axis = Math::ATan2(m_direction.x, m_direction.z);
}
void Camera::Drag(double delta_x_pos, double delta_y_pos)
{
	// get window width and height
	int width, height;
	Window* win = GetApplication()->getWindow();
	if (win)
	{
		win->getWinSize(width, height);
	}
	
	// get drag plane distance
	muggle::vec3f target = m_target_position - m_position;
	float drag_plane_distance = muggle::vec3f::Dot(target, m_direction);

	// calculate camera move
	float H = 2 * muggle::Math::Tan(muggle::Math::DegreeToRad(m_fov / 2)) * drag_plane_distance;
	muggle::vec3f delta_x = m_right * H * m_aspect * (float)delta_x_pos / (float)width;
	muggle::vec3f delta_y = m_up * H * (float)delta_y_pos / (float)height;

	m_position -= delta_x;
	m_position -= delta_y;
}

NS_MUGGLE_END