//
// Created by Junhao Wang (@Forkercat) on 2021/5/13.
//

#ifndef FORKERPATHTRACER_SRC_CORE_LOADER_H_
#define FORKERPATHTRACER_SRC_CORE_LOADER_H_

#include <spdlog/spdlog.h>

#include <map>
#include <memory>
#include <vector>

#include "common.h"

class MeshTriangle;
class Triangle;
class Material;
class Texture;

class Loader
{
public:
    // Constructor
    explicit Loader(const std::string& filename, bool normalized = false,
                    bool flipTexCoordY = true);

    std::vector<std::shared_ptr<MeshTriangle>> MeshTriangles() const  // expensive
    {
        std::vector<std::shared_ptr<MeshTriangle>> v;
        for (const auto& pair : m_MeshTriangles)
        {
            v.push_back(pair.second);
        }
        return v;
    }

private:
    bool loadOBJFile(const std::string& filename);
    void loadMaterials(const std::string& directory, const std::string& filename);
    void loadTexture(const std::string& textureFilename, std::shared_ptr<Texture>& texture);
    void normalizePositionVertices();

    // Private Data
    std::map<std::string, std::shared_ptr<MeshTriangle>> m_MeshTriangles;
    std::map<std::string, std::shared_ptr<Material>>     m_Materials;

    std::vector<Vector3f> m_Verts;
    std::vector<Vector2f> m_TexCoords;
    std::vector<Vector3f> m_Normals;

    bool m_Normalized;
    bool m_FlipTexCoordY;
};

#endif  // FORKERPATHTRACER_SRC_CORE_LOADER_H_
