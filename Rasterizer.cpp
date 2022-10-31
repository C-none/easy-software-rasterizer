#include"Rasterizer.h"

namespace rst {

	using namespace Eigen;

	rasterizer::rasterizer(rst::camera* _CAMERA) {
		CAMERA = _CAMERA;
		depth_buffer.resize(CAMERA->height * CAMERA->width);
		frame_buffer.resize(CAMERA->height * CAMERA->width);
		for (size_t i = 0; i < CAMERA->height * CAMERA->width; i++)
			depth_buffer[i] = -FLT_MAX, frame_buffer[i] = Vector3f{ 0,0,0 };
	}

	size_t rasterizer::get_index(int x, int y) {
		return y * CAMERA->width + x;
	}

	void rasterizer::set_lights(const std::vector<light>& _lights) {
		lights.clear();
		for (auto& i : _lights)lights.emplace_back(light{ i.intensity,set_vector(i.position) });
	}


	void rasterizer::draw(std::vector<triangle>& list) {

		std::vector<light> lights_to_rst = lights;
		for (auto& i : lights_to_rst) {
			i.position = CAMERA->View_Trans() * i.position;
		}

		for (auto& tri : list) {

			triangle tri_to_draw;
			tri_to_draw.texture = tri.texture;
			tri_to_draw.tex_coordinate = tri.tex_coordinate;
			tri_to_draw.color = tri.color;

			std::array<Vector4f, 3> view_space_vertex;
			for (size_t i = 0; i < 3; i++) {
				view_space_vertex[i] = CAMERA->View_Trans() * tri.vertex[i];
				tri_to_draw.vertex[i] = set_vector(CAMERA->Proj_Trans() * view_space_vertex[i]);
			}

			std::array<Vector4f, 3> normal4d;
			std::transform(std::begin(tri.normal), std::end(tri.normal),
				std::begin(normal4d), [](auto v) {return Vector4f{ v[0],v[1],v[2],0 }; });

			for (auto& i : normal4d)i = (CAMERA->View_Trans().inverse().transpose()) * i;
			std::transform(std::begin(normal4d), std::end(normal4d),
				std::begin(tri_to_draw.normal), [](auto v) {return (v.template head<3>()).normalized(); });

			for (auto& i : tri_to_draw.vertex) {
				i.x() = CAMERA->width * (i.x() + 1) / 2;
				i.y() = CAMERA->height * (i.y() + 1) / 2;
			}

			rasterizer_triangle(tri_to_draw, view_space_vertex, lights_to_rst);
		}
	}

	const Vector4f& rasterizer::vertex_shader(const payload& vertex_in_payload) {
		return vertex_in_payload.position;
	}

	void rasterizer::rasterizer_triangle(const triangle& tri, const std::array<Vector4f, 3>& view_space_vertex, std::vector<light> lights_to_rst) {

		int left = std::max<int>(0, static_cast<int>(std::min<float>({ tri.vertex[0][0],tri.vertex[1][0],tri.vertex[2][0] })));
		int down = std::max<int>(0, static_cast<int>(std::min<float>({ tri.vertex[0][1],tri.vertex[1][1],tri.vertex[2][1] })));
		int right = std::min<int>(static_cast<int>(CAMERA->width) - 1, 1 + static_cast<int>(std::max<float>({ tri.vertex[0][0],tri.vertex[1][0],tri.vertex[2][0] })));
		int up = std::min<int>(static_cast<int>(CAMERA->height) - 1, 1 + static_cast<int>(std::max<float>({ tri.vertex[0][1],tri.vertex[1][1],tri.vertex[2][1] })));

		omp_set_num_threads(16);
#pragma omp parallel for schedule(dynamic)

		for (int y = down; y <= up; y++)
		{
			for (int x = left; x <= right; x++)
			{
				if (is_in_triangle(tri.vertex, x, y)) {

					auto [alpha, beta, gamma] = interpolate(tri.vertex, x, y);
					float depth = 1. / (alpha / view_space_vertex[0].z() + beta / view_space_vertex[1].z() + gamma / view_space_vertex[2].z());

					if (depth<0 && depth>depth_buffer[get_index(x, y)]) {

						depth_buffer[get_index(x, y)] = depth;

						Vector4f position = depth * (alpha * view_space_vertex[0] / view_space_vertex[0][2] +
							beta * view_space_vertex[1] / view_space_vertex[1][2] +
							gamma * view_space_vertex[2] / view_space_vertex[2][2]);

						Vector3f color = depth * (alpha * tri.color[0] / view_space_vertex[0][2] +
							beta * tri.color[1] / view_space_vertex[1][2] +
							gamma * tri.color[2] / view_space_vertex[2][2]);

						Vector3f normal = (depth * (alpha * tri.normal[0] / view_space_vertex[0][2] +
							beta * tri.normal[1] / view_space_vertex[1][2] +
							gamma * tri.normal[2] / view_space_vertex[2][2])).normalized();

						Vector2f tex_coordinate = depth * (alpha * tri.tex_coordinate[0] / view_space_vertex[0][2] +
							beta * tri.tex_coordinate[1] / view_space_vertex[1][2] +
							gamma * tri.tex_coordinate[2] / view_space_vertex[2][2]);

						payload fragment_shader_load{ tri.texture,std::move(position),std::move(color),std::move(normal),std::move(tex_coordinate) };
						frame_buffer[get_index(x, y)] = fragment_shader(fragment_shader_load, lights_to_rst);
					}
				}
			}
		}
	}

	Vector3f rasterizer::fragment_shader(const payload& load, std::vector<light> lights_to_rst) {
		Vector3f ka;
		Vector3f kd;
		Vector3f ks = { 1,1,1 };
		auto x = load.tex_coordinate.x(), y = load.tex_coordinate.y();
		if (load.texture and x <= 1 and x >= 0 and y <= 1 and y >= 0) {
			kd = get_texture(this, load);
		}
		else
		{
			kd = load.color;
		}

		ka = kd * .5;
		int p = 150;

		Vector3f ans = { 0,0,0 };

		for (auto& i : lights_to_rst) {

			Vector3f l = (i.position - vertex_shader(load)).template head<3>();
			Vector3f v = -vertex_shader(load).template head<3>();
			Vector3f n = v.dot(load.normal) > 0 ? load.normal : -load.normal;
			Vector3f intensity = i.intensity / (l.norm() * l.norm());

			if (n.dot(l) < 0) {
				ans += ka.cwiseProduct(intensity); continue;
			}

			l.normalize(), v.normalize();
			Vector3f h = (l + v).normalized();
			ans += ka.cwiseProduct(intensity) + kd.cwiseProduct(intensity) * n.dot(l) + ks.cwiseProduct(intensity) * fast_exp(n.dot(h), p);

		}

		return ans;
	}

	Vector3f rasterizer::direct_sample(const const payload& load) {
		auto x = load.tex_coordinate.x(), y = load.tex_coordinate.y();
		Vector3f kd;
		auto textu = GetImageBuffer(load.texture);
		DWORD color = textu[int(load.texture->getheight() * (1 - y)) * load.texture->getwidth() + int(x * load.texture->getwidth())];
		kd[0] = ((color >> 16) & 0xff) / 255.f;
		kd[1] = ((color >> 8) & 0xff) / 255.f;
		kd[2] = ((color >> 0) & 0xff) / 255.f;
		return kd;
	}

	Vector3f rasterizer::Bilinear_interpolation(const payload& load) {
		Vector3f kd = { 0,0,0 };
		auto x = load.tex_coordinate.x(), y = load.tex_coordinate.y();
		y = 1 - y;
		int h = load.texture->getheight() - 1, w = load.texture->getwidth() - 1;
		int int_x = static_cast<int>(x * w), int_y = static_cast<int>(y * h);
		auto textu = GetImageBuffer(load.texture);
		DWORD color;
		color = textu[int_y * (w + 1) + int_x];
		kd[0] += ((color >> 16) & 0xff) * (int_x + 1 - x * w) * (int_y + 1 - y * h);
		kd[1] += ((color >> 8) & 0xff) * (int_x + 1 - x * w) * (int_y + 1 - y * h);
		kd[2] += ((color >> 0) & 0xff) * (int_x + 1 - x * w) * (int_y + 1 - y * h);
		color = textu[(int_y + 1) * (w + 1) + int_x + 1];
		kd[0] += ((color >> 16) & 0xff) * (x * w - int_x) * (y * h - int_y);
		kd[1] += ((color >> 8) & 0xff) * (x * w - int_x) * (y * h - int_y);
		kd[2] += ((color >> 0) & 0xff) * (x * w - int_x) * (y * h - int_y);
		color = textu[(int_y) * (w + 1) + int_x + 1];
		kd[0] += ((color >> 16) & 0xff) * (x * w - int_x) * (int_y + 1 - y * h);
		kd[1] += ((color >> 8) & 0xff) * (x * w - int_x) * (int_y + 1 - y * h);
		kd[2] += ((color >> 0) & 0xff) * (x * w - int_x) * (int_y + 1 - y * h);
		color = textu[(int_y + 1) * (w + 1) + int_x];
		kd[0] += ((color >> 16) & 0xff) * (int_x + 1 - x * w) * (y * h - int_y);
		kd[1] += ((color >> 8) & 0xff) * (int_x + 1 - x * w) * (y * h - int_y);
		kd[2] += ((color >> 0) & 0xff) * (int_x + 1 - x * w) * (y * h - int_y);
		kd[0] /= 255.f, kd[1] /= 255.f, kd[2] /= 255.f;
		return kd;
	}

	void rasterizer::load_dword(DWORD* frame) {
		omp_set_num_threads(16);
#pragma omp parallel for schedule(dynamic)
		for (int y = 0; y < int(CAMERA->height); y++)
		{
			for (int x = 0; x < int(CAMERA->width); x++)
			{
				BYTE r = 0, g = 0, b = 0;
				r += set_color(frame_buffer[get_index(x, y)].x()) * 255;
				g += set_color(frame_buffer[get_index(x, y)].y()) * 255;
				b += set_color(frame_buffer[get_index(x, y)].z()) * 255;
				frame[(CAMERA->height - 1 - y) * CAMERA->width + x] = BGR(RGB(r, g, b));
			}
		}
	}

	void rasterizer::load_SSAA(DWORD* frame) {
		omp_set_num_threads(16);
#pragma omp parallel for schedule(dynamic)
		for (int y = 0; y < int(CAMERA->height) / 4; y++)
		{
			for (int x = 0; x < int(CAMERA->width) / 4; x++)
			{
				float r = 0, g = 0, b = 0;
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						r += frame_buffer[get_index(4 * x + i, 4 * y + j)].x();
						g += frame_buffer[get_index(4 * x + i, 4 * y + j)].y();
						b += frame_buffer[get_index(4 * x + i, 4 * y + j)].z();
					}
				}
				frame[(CAMERA->height / 4 - 1 - y) * (CAMERA->width / 4) + x] = BGR(RGB(set_color(r / 16) * 255, set_color(g / 16) * 255, set_color(b / 16) * 255));

			}
		}
	}

	void rasterizer::sky_box() {
		auto forward = (CAMERA->forward.template head<3>()).normalized();
		auto upward = (CAMERA->upward.template head<3>()).normalized();
		auto rightward = forward.cross(upward).normalized();
		upward *= tan(CAMERA->eye_FOV / 2) * 2;
		rightward *= tan(CAMERA->eye_FOV / 2) * 2 * CAMERA->aspect_radio;
		omp_set_num_threads(16);
#pragma omp parallel for schedule(dynamic)
		for (int y = 0; y < int(CAMERA->height); y++)
		{
			for (int x = 0; x < int(CAMERA->width); x++)
			{
				if (depth_buffer[y * CAMERA->width + x] == -FLT_MAX) {
					auto tmp = ((x - int(CAMERA->width) / 2) * 1. / CAMERA->width * rightward + (y - int(CAMERA->height) / 2) * 1. / CAMERA->height * upward + forward).normalized();
					auto color = .5f + tmp[2] * .5f;
					frame_buffer[y * CAMERA->width + x] = Vector3f{ color,color,color };
				}
			}
		}
	}

	void rasterizer::clear() {
		omp_set_num_threads(16);
#pragma omp parallel for schedule(dynamic)
		for (int i = 0; i < CAMERA->height * CAMERA->width; i++)
			depth_buffer[i] = -FLT_MAX, frame_buffer[i] = Vector3f{ 0,0,0 };
	}
}