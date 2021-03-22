#include "pch.h"

#include "obj_loader.h"
#include "scene.h"
#include "depth_pass.h"
#include "default_pass.h"
#include "ppm_exporter.h"

int main()
{
	/*
	ObjLoader loader;
	Camera camera = Camera(Vec3(278, 273, -1000), Vec3(0, 1, 0), Vec3(0, 0, 1), 0.6, 600, 600);
	Scene cornell_box(camera);
	cornell_box.LoadGeometry(std::vector<std::string>{"./resources/cornell_box_original.obj"}, loader);
	*/
	///*

	ObjLoader loader;
	Scene cornell_box;
	cornell_box.LoadGeometry(std::vector<std::string>{"./resources/cornell_box_sphere.obj"}, loader);
	cornell_box.LoadMetaFile("./resources/cornell_box_sphere.txt");
	//*/

	int* depth = new int[cornell_box.camera.width * cornell_box.camera.height * 3];
	int* color = new int[cornell_box.camera.width * cornell_box.camera.height * 3];
	std::pair<int*, int*> buffers(color, depth);
	
	/*
	DepthPass depthPass = DepthPass();
	depthPass.Render(cornell_box, buffers);

	PPM_Exporter ppm = PPM_Exporter();
	ppm.Export("depth", depth, std::pair<int, int>(cornell_box.camera.width, cornell_box.camera.height));
	*/

	DefaultPass defaultPass = DefaultPass();
	defaultPass.Render(cornell_box, buffers, 3);


	PPM_Exporter ppm = PPM_Exporter();
	//ppm.Export("depth", depth, std::pair<int, int>(cornell_box.camera.width, cornell_box.camera.height));
	ppm.Export("color", color, std::pair<int, int>(cornell_box.camera.width, cornell_box.camera.height));


}