#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include"global.h"

namespace rst {

	using namespace Eigen;

	class camera {

	public:

		Vector4f position;
		Vector4f forward;
		Vector4f upward;
		size_t width, height;
		float aspect_radio;
		float eye_FOV;
		float n = 0.1f, f = 50.f;


		camera(size_t _width,size_t _height,float _FOV,const Vector4f&_position,const Vector4f&_forward,const Vector4f&_upward):
			width(_width),height(_height),aspect_radio(width*1.0f/height),eye_FOV(Degree_to_Radian(_FOV)), 
			position(_position), forward(set_vector(_forward)), upward(set_vector(_upward)) {}
		Matrix4f View_Trans();
		Matrix4f Proj_Trans();

	};
}

#endif //CAMERA_HEADER
