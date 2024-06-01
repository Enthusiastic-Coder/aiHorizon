#include "aimeshloader.h"

#include <QOpenGLShaderProgram>

AIMeshLoader::AIMeshLoader(std::string filename, std::shared_ptr<QtTextureManager> t, std::shared_ptr<AssimpMeshManager> meshManager)
    :meshLoader(t, meshManager)
{
    load(QString::fromStdString(filename));
}

void AIMeshLoader::draw(OpenGLShaderProgram& p)
{
    auto& meshes = getMeshes();
    for(auto& mesh: meshes)
    {
        std::string name = mesh->name();
        bool bMod=false;
        if( name == "secondary_ai_color_Disk")
        {
            bMod = true;


        }
        else
            mesh.draw(p);

    }
}

