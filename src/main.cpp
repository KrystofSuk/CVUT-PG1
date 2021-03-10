#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

#define WIDTH 800
#define HEIGHT 800

using namespace std;

const double epsilon = 1e-9; // Small value

struct vec3
{
	vec3() : x(0), y(0), z(0) {}
	vec3(double v) : x(v), y(v), z(v) {}
	vec3(double x0, double y0, double z0 = 0) : x(x0), y(y0), z(z0) {}
	vec3 operator*(double a) const { return vec3(x * a, y * a, z * a); }
	vec3 operator*(const vec3 r) const { return vec3(x * r.x, y * r.y, z * r.z); }
	vec3 operator/(const double r) const { return fabs(r) > epsilon ? vec3(x / r, y / r, z / r) : vec3(0, 0, 0); }
	vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
	vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
	vec3 operator-() const { return vec3(-x, -y, -z); }
	void operator+=(const vec3& v) { x += v.x, y += v.y, z += v.z; }
	void operator*=(double a) { x *= a, y *= a, z *= a; }
	void operator*=(const vec3& v) { x *= v.x, y *= v.y, z *= v.z; }
	double length() const { return sqrt(x * x + y * y + z * z); }
	double average() { return (x + y + z) / 3; }
	vec3 normalized() const { return (*this) / length(); }

	double x, y, z;
};

double
dot(const vec3& v1, const vec3& v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

vec3 cross(const vec3& v1, const vec3& v2)
{
	return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

ostream& operator<<(ostream& os, vec3 vec)
{
	os << vec.x << " " << vec.y << " " << vec.z;
	return os;
}

template <typename T>
inline T clamp(T val, T low, T high)
{
	return max(min(val, high), low);
}

struct Ray
{
	Ray(vec3 oV, vec3 dV) : o(oV), d(dV) {};
	vec3 o, d;
};

struct Sphere
{
	Sphere(float rad_, vec3 p_) : rad(rad_), p(p_) {}

	float intersect(const Ray& r) const
	{
		// returns distance, 0 if nohit
		vec3 op = p - r.o; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
		double t, eps = 1e-4, b = dot(op, r.d), det = b * b - dot(op, op) + rad * rad;

		if (det < 0)
			return INFINITY;
		else
			det = sqrt(det);

		return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : INFINITY);
	}

	float rad; // radius
	vec3 p;    // position
};

struct Camera
{
	Camera(vec3 pV, vec3 uV, vec3 fV, double fovV) : p(pV), u(uV), f(fV), fov(fovV)
	{
		s = cross(f, u).normalized();

		double w = 2.0 * tan(fov / 2.0);
		double h = w * HEIGHT / WIDTH;

		d0 = f - (s * w / 2) + (u * h / 2);

		m = s * (w / (WIDTH - 1));
		n = -u * (h / (HEIGHT - 1));
	};

	vec3 p, u, f, s, d0, m, n;
	double fov;

	Ray PrimaryRayForPixel(double x, double y)
	{
		vec3 dir = (d0 + m * x + n * y).normalized();

		return Ray(p, dir);
	}
};

struct Vertex {
	vec3 p; //position
	vec3 n; //normal

	Vertex()
	{
	};

	Vertex(vec3 pN, vec3 nN = vec3()) : p(pN), n(nN)
	{

	};
};

struct Triangle {
	Vertex a, b, c;
	Triangle(Vertex aV, Vertex bV, Vertex cV) : a(aV), b(bV), c(cV)
	{

	};
};


int main(int argc, char const* argv[])
{
	vector<Triangle> triangles;

	std::string inputfile = "./Resources/CornellBox-Original.obj";
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./"; // Path to material files

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(inputfile, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			Vertex vertexes[3];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

				if (attrib.normals.size() != 0) {
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
					vertexes[v] = Vertex(vec3(vx, vy, vz), vec3(nx, ny, nz));
				}
				else {
					vertexes[v] = Vertex(vec3(vx, vy, vz));
				}


			}

			triangles.push_back(Triangle(vertexes[0], vertexes[1], vertexes[2]));

			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}


	float* image = new float[WIDTH * HEIGHT * 3];

	//creating checker texture
	for (int i = 0; i < WIDTH; ++i)
		for (int j = 0; j < HEIGHT; ++j)
			for (int k = 0; k < 3; ++k)
				image[3 * (j * WIDTH + i) + k] = 0; //(i + j) % 2 * 255;

	Camera cam(vec3(0), vec3(0, 0, 1), vec3(0, 1, 0), 1.0);

	vector<Sphere> scene;
	scene.push_back(Sphere(.5, vec3(0, 15, 0)));
	scene.push_back(Sphere(.7, vec3(5, 15, 0)));
	scene.push_back(Sphere(.9, vec3(-2, 15, 0)));

	//image saving
	ofstream output("output.ppm");
	output << "P3\n"
		<< WIDTH << " " << HEIGHT << "\n"
		<< 255 << endl;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			Ray r = cam.PrimaryRayForPixel(x, y);

			double res = INFINITY;
			for (auto& obj : scene)
			{
				double t = obj.intersect(r);
				if (t < res && res != INFINITY)
					res = t;
				if (res == INFINITY)
					res = t;
			}

			if (res != INFINITY)
			{
				for (int k = 0; k < 3; ++k)
				{
					image[(y * WIDTH + x) * 3 + k] = clamp((int)res, 0, 255);
				}
			}
		}
	}

	for (int i = 0; i < WIDTH * HEIGHT * 3; ++i)
		output << (int)image[i] << " ";
	output.close();
}
