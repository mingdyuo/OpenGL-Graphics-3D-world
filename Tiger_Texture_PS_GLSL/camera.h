#pragma once
#define CAM_TSPEED 0.05f
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> // inverseTranspose, etc.

//typedef struct _Camera {
//	float pos[3];
//	float uaxis[3], vaxis[3], naxis[3];
//	float fovy, aspect_ratio, near_c, far_c;
//	int move;
//} Camera;

class Camera {
public:
	glm::vec3 pos;
	glm::vec3 uaxis, vaxis, naxis;
	float fovy, aspect_ratio, near_c, far_c;
	int move;
	Camera::Camera();
	void set_ViewMatrix_from_camera_frame(void);
	void initialize();
};

enum axes { X_AXIS, Y_AXIS, Z_AXIS };

extern int flag_translation_axis;

//extern Camera camera;

//void set_ViewMatrix_from_camera_frame(void);
//void initialize_camera(void);
void renew_cam_position(int del);
void renew_cam_orientation_rotation_around_v_axis(int angle);