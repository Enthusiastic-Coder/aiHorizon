#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <meshData.h>
#include <QOpenGLFunctions>

class mesh : private QOpenGLFunctions
{
public:
    mesh(std::string name, std::vector<vertexData>* vd,std::vector<unsigned short>* id,std::vector<textureData>* td=NULL);
    ~mesh();
    void draw(unsigned int programId);
    std::string name() const;

private:
    std::string _name;
    std::vector<vertexData> _data;
    std::vector<textureData> _textures;
    std::vector<unsigned short> _indices;
    unsigned int VBO;
    unsigned int IND;
};

#endif // MESH_H
