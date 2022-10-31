#include"Camera.h"

namespace rst {

	using namespace Eigen;

	Matrix4f camera::View_Trans() {
		Vector3f gxt = forward.template head<3>().cross(upward.template head<3>());
		Matrix4f R, T;
		R << gxt[0], gxt[1], gxt[2], 0,
			upward[0], upward[1], upward[2], 0,
			-forward[0], -forward[1], -forward[2], 0,
			0, 0, 0, 1;
		T << 1, 0, 0, -position[0],
			0, 1, 0, -position[1],
			0, 0, 1, -position[2],
			0, 0, 0, 1;
		return R * T;
	}

	Matrix4f camera::Proj_Trans() {
		float angle = eye_FOV / 2;
		float h = n * tan(angle) * 2;
		float w = h * aspect_radio;
		Matrix4f Ortho, Persp, Scale;
		Ortho << 1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1 / (f - n), n / (f - n),
			0, 0, 0, 1;
		Scale << 2 / w, 0, 0, 0,
			0, 2 / h, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
		Persp << -n, 0, 0, 0,
			0, -n, 0, 0,
			0, 0, -(n + f), -n * f,
			0, 0, 1, 0;
		return Ortho * Scale * Persp;
	}

}