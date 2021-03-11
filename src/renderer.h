#include "scene.h"


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

        Camera cam(vec3(278, 273, -1000), vec3(0, 1, 0), vec3(0, 0, 1), 0.6, w, h);
        


        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                Ray r = cam.PrimaryRayForPixel(x, y);

                double res = INFINITY;

                for (auto &triangle : scene_.triangles){
                    float b1, b2;
                    float t = triangle.Intersect(r, true, b1, b2);

                    if (t < res && res != INFINITY)
                        res = t;
                    if (res == INFINITY)
                        res = t;
                }

                if (res != INFINITY)
                {
                    for (int k = 0; k < 3; ++k)
                    {
                        image[(y * w + x) * 3 + k] = clamp((int)(res/20), 0, 255);
                    }
                }
            }
        }

        SaveImage(image);
    }


    void SaveImage(float * image){
        //image saving
        ofstream output("output.ppm");
        output << "P3\n"
            << w << " " << h << "\n"
            << 255 << endl;

        for (int i = 0; i < w * h * 3; ++i)
            output << (int)image[i] << " ";
        output.close();
    }
};