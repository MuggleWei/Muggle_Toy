#ifndef __MUGGLE_CAMERA_H__
#define __MUGGLE_CAMERA_H__

#include "base.h"
#include "math/muggle_math.h"

NS_MUGGLE_BEGIN

class Camera
{
public:
	MG_DLL Camera();
	MG_DLL virtual ~Camera();

	MG_DLL virtual const matrix4f& getViewMatrix();
	MG_DLL virtual const matrix4f& getProjectionMatrix();

	MG_DLL virtual void Update();

	MG_DLL void setFOV(float fov);
	MG_DLL void setAspect(float aspect);
	MG_DLL void setNearPlane(float z_near);
	MG_DLL void setFarPlane(float z_far);

	MG_DLL vec3f getPosition();
	MG_DLL float getFOV();
	MG_DLL float getAspect();
	MG_DLL float getNearPlane();
	MG_DLL float getFarPlane();

protected:
	void UpdateMatrix();

	void RotateDirection(float delta_x_pos, float delta_y_pos);
	void RotateAroundTarget(float delta_x_pos, float delta_y_pos);
	void Drag(double delta_x_pos, double delta_y_pos);

public:
	matrix4f m_view_matrix;
	matrix4f m_projection_matrix;

	vec3f m_direction;	// z-axis
	vec3f m_right;		// x-axis
	vec3f m_up;			// y-axis

	vec3f m_position;
	vec3f m_target_position;

	// Spherical coordinates
	float m_rotate_y_axis;
	float m_rotate_x_axis;

	// about perspective
	float m_fov;	// angle degree
	float m_aspect;
	float m_z_near;
	float m_z_far;

	float m_move_speed;				// move speed
	float m_rotate_speed;			// rotate speed
	float m_drag_rotate_speed;		// speed of rotate around target

	int m_last_mouse_pos_x;
	int m_last_mouse_pos_y;

	bool m_absolute_free;			// if false, up always (0, 1, 0)

	bool m_dirty;					// need update matrix
};

NS_MUGGLE_END

#endif