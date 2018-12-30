#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <QObject>

#include <vector>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"

#include <QOpenGLFunctions>

class meshLoader : private QOpenGLFunctions
{
public:
    explicit meshLoader(std::string filename);

    virtual ~meshLoader();
    void draw(unsigned int programId);
    std::vector<mesh*>& getMeshes();

protected:
    std::vector<mesh*> _meshes;
    void recursiveProcess(aiNode* node, const aiScene* scene);
    void processMesh(aiString name, aiMesh* mesh, const aiScene* scene);
    unsigned int loadTexture(std::string filename);
    std::map<std::string, unsigned int> textures_;

};

#endif // MESHLOADER_H
