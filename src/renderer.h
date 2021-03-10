#include "scene.h"


struct Ray
{
	Ray(vec3 o_, vec3 d_) : o(o_), d(d_){};
	vec3 o, d;
};

struct Sphere
{
	Sphere(float rad_, vec3 p_) : rad(rad_), p(p_) {}

	float intersect(const Ray &r) const
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
	vec3 p;	   // position
};

struct Camera
{
	vec3 p, u, f, s, d0, m, n; //position, up, front, side, (d0, m, n) for pixel coords
	double fov; //fov

	Camera(vec3 p_, vec3 u_, vec3 f_, double fov_, int width, int height) : p(p_), u(u_), f(f_), fov(fov_)
	{
		s = cross(f, u).normalized();

		double w = 2.0 * tan(fov / 2.0);
		double h = w * width / height;

		d0 = f - (s * w / 2) + (u * h / 2);

		m = s * (w / (width - 1));
		n = -u * (h / (height - 1));
	};

	Ray PrimaryRayForPixel(double x, double y)
	{
		vec3 dir = (d0 + m * x + n * y).normalized();

		return Ray(p, dir);
	}
};

struct Renderer{
    int w, h = 800;

    void Render(Scene & scene_){
        float *image = new float[w * h * 3];

        //creating checker texture
        for (int i = 0; i < w; ++i)
            for (int j = 0; j < h; ++j)
                for (int k = 0; k < 3; ++k)
                    image[3 * (j * w + i) + k] = 0; //(i + j) % 2 * 255;

        Camera cam(vec3(0), vec3(0, 0, 1), vec3(0, 1, 0), 1.0, w, h);


        vector<Sphere> scene;
        scene.push_back(Sphere(.5, vec3(0, 15, 0)));
        scene.push_back(Sphere(.7, vec3(5, 15, 0)));
        scene.push_back(Sphere(.9, vec3(-2, 15, 0)));

        //image saving
        ofstream output("output.ppm");
        output << "P3\n"
            << w << " " << h << "\n"
            << 255 << endl;

        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                Ray r = cam.PrimaryRayForPixel(x, y);

                double res = INFINITY;
                for (auto &obj : scene)
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
                        image[(y * w + x) * 3 + k] = clamp((int)res, 0, 255);
                    }
                }
            }
        }

        for (int i = 0; i < w * h * 3; ++i)
            output << (int)image[i] << " ";
        output.close();
    }
};