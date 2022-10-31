#ifndef GLOBAL_HEADER
#define GLOBAL_HEADER

#define MY_PI 3.14159265f
#define Degree_to_Radian(x) ((x)/180.f*MY_PI)
#define set_color(x) ((x)>1?1:(x)<0?0:(x))

#include<type_traits>
#include<algorithm>
#include<eigen3/Eigen/Eigen>
#include<cmath>
#include<vector>
#include<tuple>
#include<easyx.h>
#include<omp.h>
#include<algorithm>
#include<iostream>
#include<functional>

namespace rst {

	using namespace Eigen;

	struct light {
		Eigen::Vector3f intensity;
		Eigen::Vector4f	position;
	};

	struct payload {
		IMAGE* texture;
		Vector4f position;
		Vector3f color;
		Vector3f normal;
		Vector2f tex_coordinate;
	};

	template<class T, class S, class = typename std::enable_if_t<std::is_convertible_v<S, unsigned long>>>
	T fast_exp(T num, S _exp) {
		unsigned long exp = static_cast<unsigned long>(_exp);
		T ans(1);
		while (exp)
		{
			if (exp & 1)ans = num * ans;
			num = num * num;
			exp >>= 1;
		}
		return ans;
	}

	Vector4f set_vector(const Vector4f& vertex);
	Vector4f to_vector4(const Vector3f& vertex, float w = 1);
	bool is_in_triangle(const std::array<Vector4f, 3>& tri, float x, float y);
	Matrix4f rotate(Vector3f axis, float degree);
	std::tuple<float, float, float> interpolate(const std::array<Vector4f, 3>& tri, float x, float y);
}

#endif //GLOBAL_HEADER