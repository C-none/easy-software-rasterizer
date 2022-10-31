#include"triangle.h"

namespace rst {

	using namespace Eigen;

	triangle::triangle() {
		vertex[0] << 0, 0, 0, 1;
		vertex[1] << 0, 0, 0, 1;
		vertex[2] << 0, 0, 0, 1;
		color[0] << 0, 0, 0;
		color[1] << 0, 0, 0;
		color[2] << 0, 0, 0;
		normal[0] << 0, 0, 0;
		normal[0] << 0, 0, 0;
		normal[0] << 0, 0, 0;
		tex_coordinate[0] << 0, 0;
		tex_coordinate[1] << 0, 0;
		tex_coordinate[2] << 0, 0;
	}

}

