#include "obj_loader.h"
#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"

std::vector<Triangle*> ObjLoader::Load(const std::string& _path, std::vector<Material*>& _materials) const
{
	std::vector<Triangle*> triangles;

	std::string inputfile = _path;
	tinyobj::ObjReaderConfig reader_config;
	//reader_config.mtl_search_path = "./";
	reader_config.triangulate = true;

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(inputfile, reader_config))
	{
		if (!reader.Error().empty())
		{
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty())
	{
		std::cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();


	for (auto& material : materials)
	{
		Material* m = new Material();

		m->ambient.x = material.ambient[0];
		m->ambient.y = material.ambient[1];
		m->ambient.z = material.ambient[2];

		m->diffuse.x = material.diffuse[0];
		m->diffuse.y = material.diffuse[1];
		m->diffuse.z = material.diffuse[2];

		m->specular.x = material.specular[0];
		m->specular.y = material.specular[1];
		m->specular.z = material.specular[2];

		m->emission.x = material.emission[0];
		m->emission.y = material.emission[1];
		m->emission.z = material.emission[2];

		m->transmittance.x = material.transmittance[0];
		m->transmittance.y = material.transmittance[1];
		m->transmittance.z = material.transmittance[2];

		m->ior = material.ior;
		m->shininess = material.shininess;
		m->dissolve = material.dissolve;
		m->name = material.name;

		_materials.push_back(m);
	}

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			size_t fv = shapes[s].mesh.num_face_vertices[f];
			Vertex vertexes[3];

			bool recalculate = true;

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++)
			{
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

				if (attrib.normals.size() != 0)
				{
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
					vertexes[v] = Vertex(Vec3(vx, vy, vz), Vec3(nx, ny, nz));
					recalculate = false;
				}
				else
				{
					vertexes[v] = Vertex(Vec3(vx, vy, vz));
				}
			}

			Triangle* triangle = new Triangle(vertexes[0], vertexes[1], vertexes[2], recalculate);


			index_offset += fv;

			// per-face material
			int materialId = shapes[s].mesh.material_ids[f];
			triangle->material = _materials[materialId];

			triangles.push_back(triangle);
		}
	}

	return triangles;
}
