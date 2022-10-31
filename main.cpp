#include<iostream>
#include<conio.h>
#include"Rasterizer.h"
#include"model.h"

using namespace std;

void render_cube();
void render_cube_with_ssaa();
void render_pic_with_interpolation();
void render_pic_without_interpolation();
void render_dragon();
void move_object(vector<rst::triangle>& object,Eigen::Vector3f displacement);
void rotate_object(vector<rst::triangle>& object, Eigen::Vector3f axis, float degree);

int main() {
	render_cube();
	//render_cube_with_ssaa();
	//render_pic_without_interpolation();
	//render_pic_with_interpolation();
	//render_dragon();
	return 0;
}

void render_cube() {
	vector<rst::triangle> tri_list;
	IMAGE myim;
	loadimage(&myim, _T("151.jpg"));
	Eigen::Vector3f ta = Eigen::Vector3f{ -1,0,0 }.normalized();
	Eigen::Vector3f tb = Eigen::Vector3f{ 0,0,1 }.normalized();
	rst::camera came(1000, 750, 45.f, Eigen::Vector4f{ 10,0,0,1 }, Eigen::Vector4f{ ta[0],ta[1],ta[2],1 }, Eigen::Vector4f{ tb[0],tb[1],tb[2],1 });
	rst::rasterizer myrst(&came);
	vector<rst::light>lights;
	lights.emplace_back(Eigen::Vector3f{ 150,150,150 }, Eigen::Vector4f{ 10,10,-10,1 });
	lights.emplace_back(Eigen::Vector3f{ 150,150,150 }, Eigen::Vector4f{ 10,-10,10,1 });
	myrst.lights = lights;
	rst::load_cube(tri_list);

	initgraph(static_cast<int>(myrst.CAMERA->width), static_cast<int>(myrst.CAMERA->height));
	while (1) {
		myrst.draw(tri_list);
		myrst.sky_box();
		myrst.load_dword(GetImageBuffer());
		auto message = _getch();
		switch (message)
		{
		case 'd': {
			auto rota = rst::rotate(Eigen::Vector3f{ 0,0,1 }, 2);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 'a': {
			auto rota = rst::rotate(Eigen::Vector3f{ 0,0,-1 }, 2);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 'w': {
			Eigen::Vector4f tmp = came.position / 25;
			tmp[3] = 0;
			came.position -= tmp;
			break;
		}
		case 's': {
			Eigen::Vector4f tmp = came.position / 24;
			tmp[3] = 0;
			came.position += tmp;
			break;
		}
		case 72: {
			auto tmp = came.upward.template head<3>().cross(came.forward.template head<3>()).normalized();
			auto rota = rst::rotate(tmp, 2);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 80: {
			auto tmp = came.upward.template head<3>().cross(came.forward.template head<3>()).normalized();
			auto rota = rst::rotate(tmp, -2);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 27: {
			goto end;
		}
		default:
			break;
		}

		myrst.clear();
	}
end:
	closegraph();
}
void render_cube_with_ssaa(){
	vector<rst::triangle> tri_list;
	IMAGE myim;
	loadimage(&myim, _T("151.jpg"));
	Eigen::Vector3f ta = Eigen::Vector3f{ -1,0,0 }.normalized();
	Eigen::Vector3f tb = Eigen::Vector3f{ 0,0,1 }.normalized();
	rst::camera came(4000, 3000, 45.f, Eigen::Vector4f{ 10,0,0,1 }, Eigen::Vector4f{ ta[0],ta[1],ta[2],1 }, Eigen::Vector4f{ tb[0],tb[1],tb[2],1 });
	rst::rasterizer myrst(&came);
	vector<rst::light>lights;
	lights.emplace_back(Eigen::Vector3f{ 150,150,150 }, Eigen::Vector4f{ 10,10,-10,1 });
	lights.emplace_back(Eigen::Vector3f{ 150,150,150 }, Eigen::Vector4f{ 10,-10,10,1 });
	myrst.lights = lights;
	rst::load_cube(tri_list);

	initgraph(static_cast<int>(myrst.CAMERA->width/4), static_cast<int>(myrst.CAMERA->height)/4);
	while (1) {
		myrst.draw(tri_list);
		myrst.sky_box();
		myrst.load_SSAA(GetImageBuffer());
		auto message = _getch();
		switch (message)
		{
		case 'd': {
			auto rota = rst::rotate(Eigen::Vector3f{ 0,0,1 }, 10);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 'a': {
			auto rota = rst::rotate(Eigen::Vector3f{ 0,0,-1 }, 10);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 'w': {
			Eigen::Vector4f tmp = came.position / 25;
			tmp[3] = 0;
			came.position -= tmp;
			break;
		}
		case 's': {
			Eigen::Vector4f tmp = came.position / 24;
			tmp[3] = 0;
			came.position += tmp;
			break;
		}
		case 72: {
			auto tmp = came.upward.template head<3>().cross(came.forward.template head<3>()).normalized();
			auto rota = rst::rotate(tmp, 10);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 80: {
			auto tmp = came.upward.template head<3>().cross(came.forward.template head<3>()).normalized();
			auto rota = rst::rotate(tmp, -10);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 27: {
			goto end;
		}
		default:
			break;
		}

		myrst.clear();
	}
end:
	closegraph();
}
void render_pic_without_interpolation() {
	vector<rst::triangle> tri_list;
	array<Eigen::Vector4f, 3> _vertex = { Eigen::Vector4f{0,-1.f,-1,1},Eigen::Vector4f{0,1.f,-1,1}, Eigen::Vector4f{0,-1,1,1} };
	array<Eigen::Vector4f, 3> _vertex1 = { Eigen::Vector4f{0,1.f,-1,1},Eigen::Vector4f{0,1,1.f,1}, Eigen::Vector4f{0,-1,1,1} };
	array<Eigen::Vector3f, 3> _color = { Eigen::Vector3f{1,0,0},Eigen::Vector3f{0,1,0}, Eigen::Vector3f{0,0,1} };
	array<Eigen::Vector3f, 3> _normal = { Eigen::Vector3f{1,0,0},Eigen::Vector3f{1,0,0}, Eigen::Vector3f{1,0,0} };
	array<Eigen::Vector2f, 3> tex0 = { Eigen::Vector2f{0,0},Eigen::Vector2f{1.f,0}, Eigen::Vector2f{0,1} };
	array<Eigen::Vector2f, 3> tex1 = { Eigen::Vector2f{1,0},Eigen::Vector2f{1.f,1}, Eigen::Vector2f{0,1} };
	IMAGE myim;
	loadimage(&myim, _T("560x560.jpg"));
	tri_list.emplace_back(_vertex, _color, _normal, tex0, &myim);
	tri_list.emplace_back(_vertex1, _color, _normal, tex1, &myim);
	Eigen::Vector3f ta = Eigen::Vector3f{ -1,0,0 }.normalized();
	Eigen::Vector3f tb = Eigen::Vector3f{ 0,0,1 }.normalized();
	rst::camera came(1200, 900, 45.f, Eigen::Vector4f{ 10,0,0,1 }, Eigen::Vector4f{ ta[0],ta[1],ta[2],1 }, Eigen::Vector4f{ tb[0],tb[1],tb[2],1 });
	rst::rasterizer myrst(&came);
	vector<rst::light>lights;
	lights.emplace_back(Eigen::Vector3f{ 150,150,150 }, Eigen::Vector4f{ 10,10,-10,1 });
	lights.emplace_back(Eigen::Vector3f{ 150,150,150 }, Eigen::Vector4f{ 10,-10,10,1 });
	myrst.lights = lights;

	initgraph(static_cast<int>(myrst.CAMERA->width), static_cast<int>(myrst.CAMERA->height));
	while (1) {
		myrst.draw(tri_list);
		myrst.sky_box();
		myrst.load_dword(GetImageBuffer());
		auto message = _getch();
		switch (message)
		{
		case 'd': {
			auto rota = rst::rotate(Eigen::Vector3f{ 0,0,1 }, 10);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 'a': {
			auto rota = rst::rotate(Eigen::Vector3f{ 0,0,-1 }, 10);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 'w': {
			Eigen::Vector4f tmp = came.position / 25;
			tmp[3] = 0;
			came.position -= tmp;
			break;
		}
		case 's': {
			Eigen::Vector4f tmp = came.position / 24;
			tmp[3] = 0;
			came.position += tmp;
			break;
		}
		case 72: {
			auto tmp = came.upward.template head<3>().cross(came.forward.template head<3>()).normalized();
			auto rota = rst::rotate(tmp, 2);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 80: {
			auto tmp = came.upward.template head<3>().cross(came.forward.template head<3>()).normalized();
			auto rota = rst::rotate(tmp, -2);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 27: {
			goto end;
		}
		default:
			break;
		}

		myrst.clear();
	}
end:
	closegraph();
}
void render_pic_with_interpolation() {
	vector<rst::triangle> tri_list;
	array<Eigen::Vector4f, 3> _vertex = { Eigen::Vector4f{0,-1.f,-1,1},Eigen::Vector4f{0,1.f,-1,1}, Eigen::Vector4f{0,-1,1,1} };
	array<Eigen::Vector4f, 3> _vertex1 = { Eigen::Vector4f{0,1.f,-1,1},Eigen::Vector4f{0,1,1.f,1}, Eigen::Vector4f{0,-1,1,1} };
	array<Eigen::Vector3f, 3> _color = { Eigen::Vector3f{1,0,0},Eigen::Vector3f{0,1,0}, Eigen::Vector3f{0,0,1} };
	array<Eigen::Vector3f, 3> _normal = { Eigen::Vector3f{1,0,0},Eigen::Vector3f{1,0,0}, Eigen::Vector3f{1,0,0} };
	array<Eigen::Vector2f, 3> tex0 = { Eigen::Vector2f{0,0},Eigen::Vector2f{1.f,0}, Eigen::Vector2f{0,1} };
	array<Eigen::Vector2f, 3> tex1 = { Eigen::Vector2f{1,0},Eigen::Vector2f{1.f,1}, Eigen::Vector2f{0,1} };
	IMAGE myim;
	loadimage(&myim, _T("560x560.jpg"));
	tri_list.emplace_back(_vertex, _color, _normal, tex0, &myim);
	tri_list.emplace_back(_vertex1, _color, _normal, tex1, &myim);
	Eigen::Vector3f ta = Eigen::Vector3f{ -1,0,0 }.normalized();
	Eigen::Vector3f tb = Eigen::Vector3f{ 0,0,1 }.normalized();
	rst::camera came(1200, 900, 45.f, Eigen::Vector4f{ 10,0,0,1 }, Eigen::Vector4f{ ta[0],ta[1],ta[2],1 }, Eigen::Vector4f{ tb[0],tb[1],tb[2],1 });
	rst::rasterizer myrst(&came);
	vector<rst::light>lights;
	lights.emplace_back(Eigen::Vector3f{ 150,150,150 }, Eigen::Vector4f{ 10,10,-10,1 });
	lights.emplace_back(Eigen::Vector3f{ 150,150,150 }, Eigen::Vector4f{ 10,-10,10,1 });
	myrst.lights = lights;
	myrst.get_texture = &rst::rasterizer::Bilinear_interpolation;

	initgraph(static_cast<int>(myrst.CAMERA->width), static_cast<int>(myrst.CAMERA->height));
	while (1) {
		myrst.draw(tri_list);
		myrst.sky_box();
		myrst.load_dword(GetImageBuffer());
		auto message = _getch();
		switch (message)
		{
		case 'd': {
			auto rota = rst::rotate(Eigen::Vector3f{ 0,0,1 }, 10);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 'a': {
			auto rota = rst::rotate(Eigen::Vector3f{ 0,0,-1 }, 10);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 'w': {
			Eigen::Vector4f tmp = came.position / 25;
			tmp[3] = 0;
			came.position -= tmp;
			break;
		}
		case 's': {
			Eigen::Vector4f tmp = came.position / 24;
			tmp[3] = 0;
			came.position += tmp;
			break;
		}
		case 72: {
			auto tmp = came.upward.template head<3>().cross(came.forward.template head<3>()).normalized();
			auto rota = rst::rotate(tmp, 2);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 80: {
			auto tmp = came.upward.template head<3>().cross(came.forward.template head<3>()).normalized();
			auto rota = rst::rotate(tmp, -2);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 27: {
			goto end;
		}
		default:
			break;
		}

		myrst.clear();
	}
end:
	closegraph();
}
void move_object(vector<rst::triangle>& object, Eigen::Vector3f displacement){
	Eigen::Matrix4f T;
	T << 1, 0, 0, displacement[0],
		0, 1, 0, displacement[1],
		0, 0, 1, displacement[2],
		0, 0, 0, 1;
	for (auto& i : object)for (auto& j : i.vertex)j = T * j;
}
void rotate_object(vector<rst::triangle>& object, Eigen::Vector3f axis, float degree) {
	auto rota = rst::rotate(axis, degree);
	for (auto& tri : object) {
		auto& ver = tri.vertex;
		for (auto& i : ver) {
			i = rota*i;
		}
	}
}
void room_object(vector<rst::triangle>& object, float radio) {
	//只能在原点
	for (auto& tri : object) {
		auto& ver = tri.vertex;
		for (auto& i : ver) {
			i.x() *= radio, i.y() *= radio, i.y() *= radio;
		}
	}
}
void render_dragon() {
	vector<rst::triangle> tri_list;
	IMAGE myim;
	loadimage(&myim, _T("151.jpg"));
	Eigen::Vector3f ta = Eigen::Vector3f{ -1,0,0 }.normalized();
	Eigen::Vector3f tb = Eigen::Vector3f{ 0,0,1 }.normalized();
	rst::camera came(1000, 750, 45.f, Eigen::Vector4f{ 50,0,0,1 }, Eigen::Vector4f{ ta[0],ta[1],ta[2],1 }, Eigen::Vector4f{ tb[0],tb[1],tb[2],1 });
	rst::rasterizer myrst(&came);
	vector<rst::light>lights;
	lights.emplace_back(Eigen::Vector3f{ 600,600,600 }, Eigen::Vector4f{ 20,20,-20,1 });
	lights.emplace_back(Eigen::Vector3f{ 500,350,350 }, Eigen::Vector4f{ 20,-20,20,1 });
	myrst.lights = lights;
	rst::load_dragon(tri_list);

	initgraph(static_cast<int>(myrst.CAMERA->width), static_cast<int>(myrst.CAMERA->height));
	while (1) {
		myrst.draw(tri_list);
		myrst.sky_box();
		myrst.load_dword(GetImageBuffer());
		auto message = _getch();
		switch (message)
		{
		case 'd': {
			auto rota = rst::rotate(Eigen::Vector3f{ 0,0,1 }, 5);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 'a': {
			auto rota = rst::rotate(Eigen::Vector3f{ 0,0,-1 }, 5);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 'w': {
			Eigen::Vector4f tmp = came.position / 25;
			tmp[3] = 0;
			came.position -= tmp;
			break;
		}
		case 's': {
			Eigen::Vector4f tmp = came.position / 24;
			tmp[3] = 0;
			came.position += tmp;
			break;
		}
		case 72: {
			auto tmp = came.upward.template head<3>().cross(came.forward.template head<3>()).normalized();
			auto rota = rst::rotate(tmp, 5);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 80: {
			auto tmp = came.upward.template head<3>().cross(came.forward.template head<3>()).normalized();
			auto rota = rst::rotate(tmp, -5);
			came.position = rst::set_vector(rota * came.position);
			came.upward = rst::set_vector(rota * came.upward);
			came.forward = rst::set_vector(rota * came.forward);
			break;
		}
		case 27: {
			goto end;
		}
		default:
			break;
		}

		myrst.clear();
	}
end:
	closegraph();
}