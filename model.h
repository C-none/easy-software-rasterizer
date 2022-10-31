#ifndef MODEL_HEADER
#define MODEL_HEADER
#include"Triangle.h"
#include<fstream>

namespace rst {

	using namespace Eigen;

	void load_cube(std::vector<triangle>&tri_list,Vector3f colour={1,1,1}) {
		std::array<Vector4f, 8> mytri;
		mytri[0] = { 1,1,1,1 };
		mytri[1] = { -1,1,1,1 };
		mytri[2] = { -1,-1,1,1 };
		mytri[3] = { 1,-1,1,1 };
		mytri[4] = { 1,1,-1,1 };
		mytri[5] = { -1,1,-1,1 };
		mytri[6] = { -1,-1,-1,1 };
		mytri[7] = { 1,-1,-1,1 };

		std::array<Vector3f, 6> normal;
		normal[0] = { 1,0,0 };
		normal[1] = { 0,1,0 };
		normal[2] = { 0,0,1 };
		normal[3] = { -1,0,0 };
		normal[4] = { 0,-1,0 };
		normal[5] = { 0,0,-1 };

		std::array<Vector3f, 8> color;
		color[0] = { 1,1,1 };
		color[1] = { .5,1,1 };
		color[2] = { .5,.5,1 };
		color[3] = { 1,.5,1 };
		color[4] = { 1,1,.5 };
		color[5] = { .5,1,.5 };
		color[6] = { .5,.5,.5 };
		color[7] = { 1,.5,.5 };

	/*	for (auto& i : color)i = colour;*/

		tri_list.clear();
		tri_list.emplace_back(std::array<Vector4f, 3>{ mytri[0], mytri[3], mytri[4] }, std::array<Vector3f, 3>{ color[0], color[3], color[4] }, std::array<Vector3f, 3>{ normal[0], normal[0], normal[0] });
		tri_list.emplace_back(std::array<Vector4f, 3>{ mytri[3], mytri[7], mytri[4] }, std::array<Vector3f, 3>{ color[3], color[7], color[4] }, std::array<Vector3f, 3>{ normal[0], normal[0], normal[0] });
		tri_list.emplace_back(std::array<Vector4f, 3>{ mytri[1], mytri[0], mytri[5] }, std::array<Vector3f, 3>{ color[1], color[0], color[5] }, std::array<Vector3f, 3>{ normal[1], normal[1], normal[1] });
		tri_list.emplace_back(std::array<Vector4f, 3>{ mytri[0], mytri[4], mytri[5] }, std::array<Vector3f, 3>{ color[0], color[4], color[5] }, std::array<Vector3f, 3>{ normal[1], normal[1], normal[1] });
		tri_list.emplace_back(std::array<Vector4f, 3>{ mytri[0], mytri[1], mytri[3] }, std::array<Vector3f, 3>{ color[0], color[1], color[3] }, std::array<Vector3f, 3>{ normal[2], normal[2], normal[2] });
		tri_list.emplace_back(std::array<Vector4f, 3>{ mytri[1], mytri[2], mytri[3] }, std::array<Vector3f, 3>{ color[1], color[2], color[3] }, std::array<Vector3f, 3>{ normal[2], normal[2], normal[2] });
		tri_list.emplace_back(std::array<Vector4f, 3>{ mytri[2], mytri[5], mytri[6] }, std::array<Vector3f, 3>{ color[2], color[5], color[6] }, std::array<Vector3f, 3>{ normal[3], normal[3], normal[3] });
		tri_list.emplace_back(std::array<Vector4f, 3>{ mytri[2], mytri[1], mytri[5] }, std::array<Vector3f, 3>{ color[2], color[1], color[5] }, std::array<Vector3f, 3>{ normal[3], normal[3], normal[3] });
		tri_list.emplace_back(std::array<Vector4f, 3>{ mytri[3], mytri[2], mytri[7] }, std::array<Vector3f, 3>{ color[3], color[2], color[7] }, std::array<Vector3f, 3>{ normal[4], normal[4], normal[4] });
		tri_list.emplace_back(std::array<Vector4f, 3>{ mytri[2], mytri[6], mytri[7] }, std::array<Vector3f, 3>{ color[2], color[6], color[7] }, std::array<Vector3f, 3>{ normal[4], normal[4], normal[4] });
		tri_list.emplace_back(std::array<Vector4f, 3>{ mytri[4], mytri[7], mytri[5] }, std::array<Vector3f, 3>{ color[4], color[7], color[5] }, std::array<Vector3f, 3>{ normal[5], normal[5], normal[5] });
		tri_list.emplace_back(std::array<Vector4f, 3>{ mytri[7], mytri[6], mytri[5] }, std::array<Vector3f, 3>{ color[7], color[6], color[5] }, std::array<Vector3f, 3>{ normal[5], normal[5], normal[5] });
	}

	void load_dragon(std::vector<triangle>& tri_list) {
		std::ifstream in;
		in.open("dinprint.txt");
		int n;
		in >> n;
		std::vector<Vector3f> vertex;
		vertex.resize(n);
		for (int i = 1; i <= n; i++)
		{
			char ch;
			in >> ch >> vertex[i][0] >> vertex[i][1] >> vertex[i][2];
		}
		in >> n;
		tri_list.clear();
		Vector3f color = { .8f,.8f,.8f };
		for (int i = 0; i < n; i++)
		{
			char ch;
			int a, b, c;
			in >> ch >> a >> b >> c;
			Vector3f normal = (vertex[b] - vertex[a]).cross(vertex[c] - vertex[a]).normalized();
			tri_list.emplace_back(std::array<Vector4f, 3>{ to_vector4(vertex[a]), to_vector4(vertex[b]), to_vector4(vertex[c]) }, std::array<Vector3f, 3>{ color, color, color }, std::array<Vector3f, 3>{ normal, normal, normal });
		}
		in.close();
		auto rota = rst::rotate(Eigen::Vector3f{ 1,0,0 }, 90);
		for (auto& i : tri_list) {
			for (auto& j : i.vertex)j = rota * j;
			for (auto& j : i.normal)j = ((rota.inverse().transpose()) * Eigen::Vector4f{ j[0],j[1],j[2],0 }).template head<3>();
		}
		rota = rst::rotate(Eigen::Vector3f{ 0,0,1 }, 90);
		for (auto& i : tri_list) {
			for (auto& j : i.vertex)j = rota * j;
			for (auto& j : i.normal)j = ((rota.inverse().transpose()) * Eigen::Vector4f{ j[0],j[1],j[2],0 }).template head<3>();
		}
		for (auto& i : tri_list) {
			for (auto& j : i.vertex)j[1] -= 7.5, j[2] -= 14, j[0] -= 15;
		}
	}

}
#endif //MODLE_HEADER