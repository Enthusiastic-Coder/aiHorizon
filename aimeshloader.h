#ifndef AIMESHLOADER_H
#define AIMESHLOADER_H

#include <jibbs/opengl/OpenGLShaderProgram.h>
#include <jibbs/mesh/meshloader.h>

class AIMeshLoader : public meshLoader
{
public:
    AIMeshLoader(std::string filename, std::shared_ptr<QtTextureManager> t, std::shared_ptr<AssimpMeshManager> meshManager);

    void draw(OpenGLShaderProgram &p);

    float _fBank = 0.0f;
    float _fPitch = 0.0f;
};

#endif // AIMESHLOADER_H
