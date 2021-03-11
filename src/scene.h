#include "ray.h"
#include "tiny_obj_loader.h"

struct Vertex
{
    vec3 p; //position
    vec3 n; //normal
    bool calcuate = false;

    Vertex(){};

    Vertex(vec3 p_) : p(p_){
        calcuate = true;
    };
    
    Vertex(vec3 p_, vec3 n_) : p(p_), n(n_){
    };
};

struct Triangle
{
    Vertex a, b, c;
    Triangle(Vertex a_, Vertex b_, Vertex c_) : a(a_), b(b_), c(c_){
        if(a_.calcuate == true){

            vec3 n = cross(b.p -a.p, c.p - a.p);
            n = n.normalized();

            a.n = n;
            b.n = n;
            c.n = n;
        }
    };

    float Intersect(Ray & ray, bool cullback,  float & b1, float & b2)
    {
        vec3 e1(b.p - a.p), e2(c.p - a.p);
        vec3 pvec = cross(ray.d, e2);
        float det = dot(e1, pvec);

        if (cullback)
        {
            if (det < epsilon) // ray is parallel to triangle
                return INFINITY;
        }
        else
        {
            if (fabs(det) < epsilon) // ray is parallel to triangle
                return INFINITY;
        }

        float invDet = 1.0f / det;

        // Compute first barycentric coordinate
        vec3 tvec = ray.o - a.p;
        b1 = dot(tvec, pvec) * invDet;

        if (b1 < 0.0f || b1 > 1.0f)
            return INFINITY;

        // Compute second barycentric coordinate
        vec3 qvec = cross(tvec, e1);
        b2 = dot(ray.d, qvec) * invDet;

        if (b2 < 0.0f || b1 + b2 > 1.0f)
            return INFINITY;

        // Compute t to intersection point
        float t = dot(e2, qvec) * invDet;
        return t;
    }
};

struct Scene
{
    vector<Triangle> triangles;

    Scene(const string & objPath)
    {
        std::string inputfile = objPath;
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = "./";
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

        auto &attrib = reader.GetAttrib();
        auto &shapes = reader.GetShapes();
        auto &materials = reader.GetMaterials();

        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++)
        {
            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
            {
                int fv = shapes[s].mesh.num_face_vertices[f];
                Vertex vertexes[3];

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
                        vertexes[v] = Vertex(vec3(vx, vy, vz), vec3(nx, ny, nz));
                    }
                    else
                    {
                        vertexes[v] = Vertex(vec3(vx, vy, vz));
                    }
                }

                triangles.push_back(Triangle(vertexes[0], vertexes[1], vertexes[2]));

                index_offset += fv;

                // per-face material
                shapes[s].mesh.material_ids[f];
            }
        }
    }
};