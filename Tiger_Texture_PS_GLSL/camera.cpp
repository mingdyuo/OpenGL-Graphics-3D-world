#include "externs.h"
#include "camera.h"


//PRP_distance_level = (PRP_distance_level + 1) % 6;
//fprintf(stdout, "^^^ Distance level = %d.\n", PRP_distance_level);
//PRP += 0.02f;
//if (PRP > 15.0f) PRP = 15.0f;
//else if (PRP < 0.2f) PRP = 0.2f;
//
//ViewMatrix = glm::lookAt(PRP * glm::vec3(500.0f, 300.0f, 500.0f),
//	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

Camera::Camera() {
	pos = glm::vec3(400.0f, 300.0f, 400.0f);
	uaxis = glm::vec3(0.0f, 1.0f, 0.0f);
	naxis = glm::normalize(pos);
	uaxis = glm::normalize(glm::cross(uaxis, naxis));
	vaxis = glm::normalize(glm::cross(naxis, uaxis));
	move = 0;
	fovy = 45.0f; aspect_ratio = 1.0f; near_c = 100.0f; far_c = 20000.0f;

}

void Camera::initialize() {
	camera.set_ViewMatrix_from_camera_frame();
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

void Camera::set_ViewMatrix_from_camera_frame(void) {
	ViewMatrix = glm::mat4(uaxis[0], vaxis[0], naxis[0], 0.0f,
		uaxis[1], vaxis[1], naxis[1], 0.0f,
		uaxis[2], vaxis[2], naxis[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	ViewMatrix = glm::translate(ViewMatrix, -pos);
}

/*ViewMatrix = glm::lookAt(PRP_distance_scale[0] * glm::vec3(500.0f, 300.0f, 500.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/
		//ProjectionMatrix = glm::perspective(45.0f*TO_RADIAN, aspect_ratio, 100.0f, 20000.0f);
//void initialize_camera(void) {
//	/*camera.pos[0] = 500.0f; camera.pos[1] = 300.0f;  camera.pos[2] = 500.0f;
//	camera.uaxis[0] = 0.71f; camera.uaxis[1] = 0.0f; camera.uaxis[2] = -0.71f;
//	camera.vaxis[0] = -0.28f; camera.vaxis[1] = -0.92f; camera.vaxis[2] = -0.28f;
//	camera.naxis[0] = 0.65; camera.naxis[1] = 0.39f; camera.naxis[2] = 0.65f;*/
//	camera.pos[0] = 100.0f; camera.pos[1] = 40.0f;  camera.pos[2] = 100.0f;
//	camera.uaxis[0] = 0.0f; camera.uaxis[1] = 0.0f; camera.uaxis[2] = -1.0f;
//	camera.vaxis[0] = 0.0f; camera.vaxis[1] = 1.0f; camera.vaxis[2] = 0.0f;
//	camera.naxis[0] = 0.7f; camera.naxis[1] = 0.7f; camera.naxis[2] = 0.0f;
//
//	
//
//	camera.move = 0;
//	camera.fovy = 70.0f, camera.aspect_ratio = 1.0f; camera.near_c = 100.0f; camera.far_c = 20000.0f;
//
//	set_ViewMatrix_from_camera_frame();
//	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
//}

void renew_cam_position(int del) {
	switch (flag_translation_axis) {
	case X_AXIS:
		camera.pos[0] += CAM_TSPEED * del * (camera.uaxis[0]);
		camera.pos[1] += CAM_TSPEED * del * (camera.uaxis[1]);
		camera.pos[2] += CAM_TSPEED * del * (camera.uaxis[2]);
		break;
	case Y_AXIS:
		camera.pos[0] += CAM_TSPEED * del * (camera.vaxis[0]);
		camera.pos[1] += CAM_TSPEED * del * (camera.vaxis[1]);
		camera.pos[2] += CAM_TSPEED * del * (camera.vaxis[2]);
		break;
	case Z_AXIS:
		camera.pos[0] += CAM_TSPEED * del * (-camera.naxis[0]);
		camera.pos[1] += CAM_TSPEED * del * (-camera.naxis[1]);
		camera.pos[2] += CAM_TSPEED * del * (-camera.naxis[2]);
		break;
	}
}
#define CAM_RSPEED 0.1f
void renew_cam_orientation_rotation_around_v_axis(int angle) {
	// let's get a help from glm
	glm::mat3 RotationMatrix;
	glm::vec3 direction;

	RotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0), CAM_RSPEED * TO_RADIAN * angle,
		glm::vec3(camera.vaxis[0], camera.vaxis[1], camera.vaxis[2])));

	direction = RotationMatrix * glm::vec3(camera.uaxis[0], camera.uaxis[1], camera.uaxis[2]);
	camera.uaxis[0] = direction.x; camera.uaxis[1] = direction.y; camera.uaxis[2] = direction.z;
	direction = RotationMatrix * glm::vec3(camera.naxis[0], camera.naxis[1], camera.naxis[2]);
	camera.naxis[0] = direction.x; camera.naxis[1] = direction.y; camera.naxis[2] = direction.z;
}