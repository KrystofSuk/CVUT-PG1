#include "pch.h"
#include "renderer.h"

int main(int argc, char const *argv[])
{
	Scene scene; //loading cornell box

	cout << scene.triangles.size() << endl;

	Renderer r;
	r.w = 800;
	r.h = 800;
	r.Render(scene);
}
