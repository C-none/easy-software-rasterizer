#ifndef TRIANGLE_HEADER
#define TRIANGLE_HEADER

#include"global.h"


namespace rst {

	using namespace Eigen;

	class triangle {

	public:

		IMAGE* texture;
		std::array<Vector4f, 3>vertex;
		std::array<Vector3f, 3>color;//rgb
		std::array<Vector3f, 3>normal;//normal vector
		std::array<Vector2f, 3>tex_coordinate;//coordinate of each vertex in texture


	public:
		triangle();
		triangle(const std::array<Vector4f, 3>&_vertex,
			const std::array<Vector3f, 3>&_color,
			const std::array<Vector3f, 3>&_normal,
			const std::array<Vector2f, 3>&_tex_coordinate= std::array<Vector2f, 3>(),
			IMAGE* _texture=nullptr):
			vertex(_vertex),color(_color),normal(_normal),tex_coordinate(_tex_coordinate),texture(_texture){}

	};
}

#endif //TRIANGLE_HEADER