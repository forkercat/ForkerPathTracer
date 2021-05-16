//
// Created by Junhao Wang (@Forkercat) on 2021/5/13.
//

#include "loader.h"

#include <fstream>
#include <sstream>
#include <string>

#include "material.h"
#include "tgaimage.h"
#include "triangle.h"
#include "texture.h"

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

/////////////////////////////////////////////////////////////////////////////////

// Constructor
Loader::Loader(const std::string& filename, bool normalized, bool flipTexCoordY)
    : m_MeshTriangles()
{
    spdlog::info("Loading OBJ file: {}", filename);

    m_Normalized = normalized;
    m_FlipTexCoordY = flipTexCoordY;

    bool success = loadOBJFile(filename);

    if (!success)
    {
        spdlog::error("Failed to load model file.");
        return;
    }

    // Post-Processing
    if (normalized) normalizePositionVertices();

    // Info
    spdlog::info(
        "OBJ Info: v# {}, vt# {}, vn# {}, mesh# {} | normalized: {}, filpTexCoordY: {}",
        m_Verts.size(), m_TexCoords.size(), m_Normals.size(), m_MeshTriangles.size(),
        normalized, flipTexCoordY);

    for (auto iter = m_MeshTriangles.begin(); iter != m_MeshTriangles.end(); ++iter)
    {
        std::shared_ptr<MeshTriangle> meshTriangle = iter->second;
        spdlog::info(">> [{}] #tri {}", meshTriangle->MeshName(),
                     meshTriangle->NumTriangles());
    }
}

bool Loader::loadOBJFile(const std::string& filename)
{
    // Load Object File
    std::ifstream in;
    in.open(filename, std::ifstream::in);

    if (in.fail())
    {
        spdlog::error("Failed to open the OBJ file.");
        return false;
    }

    std::string line;
    std::string meshName;
    std::string materialName;
    while (!in.eof())
    {
        std::getline(in, line);
        line = ltrim(line);
        std::istringstream iss(line.c_str());

        // Trash Filters
        char        chTrash;
        std::string strTrash;
        Float       floatTrash;

        // Material File
        if (line.compare(0, 7, "mtllib ") == 0)  // mtllib
        {
            std::string mtlFilename;
            iss >> strTrash >> mtlFilename;

            std::string directory;
            size_t      slashPos = filename.find_last_of("/");

            // Format: "horizon.obj" or "...../obj/horizon.obj"
            directory = (slashPos == std::string::npos)
                            ? ""
                            : filename.substr(0, slashPos + 1);  // include "/"
            loadMaterials(directory, mtlFilename);
        }
        // Vertices
        else if (line.compare(0, 2, "v ") == 0)  // v
        {
            iss >> strTrash;  // skip 'v' and ' '
            Vector3f vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            m_Verts.push_back(vertex);
        }
        else if (line.compare(0, 3, "vt ") == 0)  // vt
        {
            iss >> strTrash;  // skip 'vt' and ' '
            Vector2f texCoord;
            iss >> texCoord.x >> texCoord.y;
            iss >> floatTrash;  // ignore the last value
            m_TexCoords.push_back(texCoord);
        }
        else if (line.compare(0, 3, "vn ") == 0)  // vn
        {
            iss >> strTrash;  // skip 'vn' and ' '
            Vector3f normal;
            iss >> normal.x >> normal.y >> normal.z;
            m_Normals.push_back(normal);
        }
        // Change Mesh / Material Status
        // Only supported when there are "g" and "usemtl" keywords (in order)
        else if (line.compare(0, 2, "g ") == 0)  // g
        {
            iss >> chTrash >> meshName;
            m_MeshTriangles[meshName] = std::make_shared<MeshTriangle>(meshName);
        }
        else if (line.compare(0, 7, "usemtl ") == 0)  // usemtl
        {
            iss >> strTrash >> materialName;  // update current material name

            // At this time, materials are initialized in m_Materials,
            // but they are not set up in Triangles. We can't do it because
            // Triangles are not created in a MeshTriangle.
            // Solution: configure when creating a triangle
            // Improvement: move material info to MeshTriangle
        }
        // Faces
        else if (line.compare(0, 2, "f ") == 0)  // f
        {
            iss >> chTrash;  // skip 'f' and ' '
            std::vector<Vector3i> vertIndices;
            unsigned int          v, t, n;

            // f 24/1/24 25/2/25 26/3/26
            while (iss >> v >> chTrash >> t >> chTrash >> n)
            {
                vertIndices.push_back(Vector3i(--v, --t, --n));  // index starts from 1
            }

            std::shared_ptr<MeshTriangle> meshTriangle = m_MeshTriangles[meshName];
            for (int i = 1; i < vertIndices.size() - 1; ++i)
            {
                // Make a triangle
                Vector3f& v0 = m_Verts[vertIndices[0].x];
                Vector3f& v1 = m_Verts[vertIndices[i].x];
                Vector3f& v2 = m_Verts[vertIndices[i + 1].x];

                Vector3f e1 = v1 - v0;
                Vector3f e2 = v2 - v0;

                if (e1.NearZero() || e2.NearZero())
                {
                    continue;
                }

                std::shared_ptr<Triangle> triangle =
                    std::make_shared<Triangle>(v0, v1, v2);
                triangle->material = m_Materials[materialName];

                // TexCoords
                triangle->t0 = m_TexCoords[vertIndices[0].y];
                triangle->t1 = m_TexCoords[vertIndices[i].y];
                triangle->t2 = m_TexCoords[vertIndices[i + 1].y];

                // Normals
                triangle->n0 = m_Normals[vertIndices[0].z];
                triangle->n1 = m_Normals[vertIndices[i].z];
                triangle->n2 = m_Normals[vertIndices[i + 1].z];

                meshTriangle->AddTriangle(triangle);
            }
        }
    }

    return true;
}

void Loader::loadMaterials(const std::string& directory, const std::string& filename)
{
    std::string mtlFilename = directory + filename;
    spdlog::info("[Loader] Loading MTL file: {}", mtlFilename);

    std::ifstream in;
    in.open(mtlFilename, std::ifstream::in);

    if (in.fail())
    {
        spdlog::error("Cannot open the MTL file: {}", mtlFilename);
        return;
    }

    std::string line;
    std::string materialName;
    while (!in.eof())
    {
        std::getline(in, line);
        line = ltrim(line);
        std::istringstream iss(line.c_str());

        // Trash
        std::string strTrash;

        if (line.compare(0, 7, "newmtl ") == 0)  // newmtl
        {
            iss >> strTrash >> materialName;
            m_Materials[materialName] = std::make_shared<Lambertian>(Color3(1.f));
        }
        // Texture Maps
        else if (line.compare(0, 7, "map_Kd ") == 0)  // map_Kd
        {
            std::string filename;
            iss >> strTrash >> filename;
            std::string textureFilename = directory + filename;
            loadTexture(textureFilename, m_Materials[materialName]->colorMap);
        }
    }
}

// Load Texture File
void Loader::loadTexture(const std::string&        textureFilename,
                         std::shared_ptr<Texture>& texture)
{
    TGAImage image;
    bool     success = image.ReadTgaFile(textureFilename.c_str());

    if (!success)
    {
        spdlog::warn("Failed to load texture: {}", textureFilename);
        return;
    }

    if (m_FlipTexCoordY) image.FlipVertically();  // flip y coordinate

    texture = std::make_shared<Texture>(image, Texture::NoWrap, Texture::Linear);
}

void Loader::normalizePositionVertices()
{
    Float xmin = MaxFloat, xmax = MinFloat;
    Float ymin = MaxFloat, ymax = MinFloat;
    Float zmin = MaxFloat, zmax = MinFloat;

    for (size_t i = 0; i < m_Verts.size(); ++i)
    {
        Vector3f v = m_Verts[i];
        xmin = Min(xmin, v.x), xmax = Max(xmax, v.x);
        ymin = Min(ymin, v.y), ymax = Max(ymax, v.y);
        zmin = Min(zmin, v.z), zmax = Max(zmax, v.z);
    }

    CHECK_NE(xmax - xmin, 0);
    CHECK_NE(ymax - ymin, 0);
    CHECK_NE(zmax - zmin, 0);

    Float    scale = 2.f / Max(xmax - xmin, Max(ymax - ymin, zmax - zmin));
    Float    tx = -(xmax + xmin) / (xmax - xmin);
    Float    ty = -(ymax + ymin) / (ymax - ymin);
    Float    tz = -(zmax + zmin) / (zmax - zmin);
    Vector3f translate = Vector3f(tx, ty, tz);

    for (auto iter = m_MeshTriangles.begin(); iter != m_MeshTriangles.end(); ++iter)
    {
        std::shared_ptr<MeshTriangle> meshTriangle = iter->second;

        for (size_t i = 0; i < meshTriangle->NumTriangles(); ++i)
        {
            std::shared_ptr<Triangle> triangle = meshTriangle->GetTriangle(i);

            triangle->v0 = triangle->v0 * scale + translate;
            triangle->v1 = triangle->v1 * scale + translate;
            triangle->v2 = triangle->v2 * scale + translate;

            triangle->e1 = triangle->v1 - triangle->v0;
            triangle->e2 = triangle->v2 - triangle->v0;

            triangle->normal = Normalize(Cross(triangle->e1, triangle->e2));
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////
