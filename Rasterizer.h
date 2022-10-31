#ifndef RASTERIZER_HEADER
#define RASTERIZER_HEADER

#include"Triangle.h"
#include"Camera.h"

namespace rst {

	using namespace Eigen;

	class rasterizer {

	public:
		rst::camera* CAMERA;
		std::vector<float>depth_buffer;
		std::vector<Vector3f>frame_buffer;
		std::vector<light>lights;
		std::function<Vector3f(rasterizer*, const payload&)> get_texture = &rasterizer::direct_sample;

		rasterizer(rst::camera* _CAMERA);
		size_t get_index(int x, int y);
		void set_lights(const std::vector<light>& _lights);
		void draw(std::vector<triangle>& list);
		const Vector4f& vertex_shader(const payload& vertex_in_payload);
		void rasterizer_triangle(const triangle& tri, const std::array<Vector4f, 3>& view_space_vertex,std::vector<light> lights_to_rst);
		Vector3f fragment_shader(const payload& load, std::vector<light> lights_to_rst);
		Vector3f direct_sample(const payload& load);
		Vector3f Bilinear_interpolation(const payload& load);
		void load_dword(DWORD* frame);
		void load_SSAA(DWORD* frame);
		void sky_box();
		void clear();
	};
}

#endif //RASTERIZER_HEADER
