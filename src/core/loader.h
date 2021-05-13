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

class Loader
{
public:
    // Constructor
    explicit Loader(const std::string& filename, bool normalized = false);

    std::map<std::string, std::shared_ptr<MeshTriangle>> MeshTriangles() const
    {
        return m_MeshTriangles;
    }

private:
    bool loadOBJFile(const std::string& filename);
    void normalizePositionVertices();

    // Private Data
    std::map<std::string, std::shared_ptr<MeshTriangle>> m_MeshTriangles;
    // std::map<std::string, std::shared_ptr<Material>>     m_Materials;

    std::vector<Vector3f> m_Verts;
    std::vector<Vector2f> m_TexCoords;
    std::vector<Vector3f> m_Normals;
};

#endif  // FORKERPATHTRACER_SRC_CORE_LOADER_H_
