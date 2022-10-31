#include"global.h"

namespace rst {


	Vector4f set_vector(const Vector4f& vertex) {
		Vector4f ans = vertex;
		ans /= vertex[3];
		return ans;
	}

	Vector4f to_vector4(const Vector3f& vertex, float w) {
		return Vector4f{ vertex[0],vertex[1],vertex[2],w };
	}

	bool is_in_triangle(const std::array<Vector4f, 3>& tri, float x, float y) {
		int ans(0);
		float border = 0;
		if (Vector3f(tri[2][0] - tri[0][0], tri[2][1] - tri[0][1], 0)
			.cross(Vector3f(tri[1][0] - tri[2][0], tri[1][1] - tri[2][1], 0))[2] < 0) {
			ans += (Vector3f(tri[2][0] - tri[0][0], tri[2][1] - tri[0][1], 0)
				.cross(Vector3f(x - tri[0][0], y - tri[0][1], 0)))[2] <= border;
			ans += (Vector3f(tri[0][0] - tri[1][0], tri[0][1] - tri[1][1], 0)
				.cross(Vector3f(x - tri[1][0], y - tri[1][1], 0)))[2] <= border;
			ans += (Vector3f(tri[1][0] - tri[2][0], tri[1][1] - tri[2][1], 0)
				.cross(Vector3f(x - tri[2][0], y - tri[2][1], 0)))[2] <= border;
		}
		else
		{
			ans += (Vector3f(tri[2][0] - tri[0][0], tri[2][1] - tri[0][1], 0)
				.cross(Vector3f(x - tri[0][0], y - tri[0][1], 0)))[2] >= -border;
			ans += (Vector3f(tri[0][0] - tri[1][0], tri[0][1] - tri[1][1], 0)
				.cross(Vector3f(x - tri[1][0], y - tri[1][1], 0)))[2] >= -border;
			ans += (Vector3f(tri[1][0] - tri[2][0], tri[1][1] - tri[2][1], 0)
				.cross(Vector3f(x - tri[2][0], y - tri[2][1], 0)))[2] >= -border;
		}
		return ans == 3;
	}

	Matrix4f rotate(Vector3f axis, float degree) {
		axis = axis.normalized();
		Matrix4f rotation;
		float radian = Degree_to_Radian(degree);
		rotation << axis[0] * axis[0] + (1 - axis[0] * axis[0]) * cos(radian), axis[0] * axis[1] * (1 - cos(radian)) - axis[2] * sin(radian), axis[0] * axis[2] * (1 - cos(radian)) + axis[1] * sin(radian), 0,
			axis[0] * axis[1] * (1 - cos(radian)) + axis[2] * sin(radian), axis[1] * axis[1] + (1 - axis[1] * axis[1]) * cos(radian), axis[1] * axis[2] * (1 - cos(radian)) - axis[0] * sin(radian), 0,
			axis[0] * axis[2] * (1 - cos(radian)) - axis[1] * sin(radian), axis[1] * axis[2] * (1 - cos(radian)) + axis[0] * sin(radian), axis[2] * axis[2] + (1 - axis[2] * axis[2]) * cos(radian), 0,
			0, 0, 0, 1;
		return rotation;
	}

	std::tuple<float, float, float> interpolate(const std::array<Vector4f, 3>& tri, float x, float y) {
		float a = ((y - tri[1][1]) * (tri[2][0] - tri[1][0]) - (x - tri[1][0]) * (tri[2][1] - tri[1][1]))
			/ ((tri[0][1] - tri[1][1]) * (tri[2][0] - tri[1][0]) - (tri[0][0] - tri[1][0]) * (tri[2][1] - tri[1][1]));
		float b = ((y - tri[2][1]) * (tri[0][0] - tri[2][0]) - (x - tri[2][0]) * (tri[0][1] - tri[2][1])) /
			((tri[1][1] - tri[2][1]) * (tri[0][0] - tri[2][0]) - (tri[1][0] - tri[2][0]) * (tri[0][1] - tri[2][1]));
		float c = 1 - a - b;
		return { a,b,c };
	}
}