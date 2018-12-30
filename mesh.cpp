#include "mesh.h"

mesh::mesh(std::string name, std::vector<vertexData>* vd,std::vector<unsigned short>* id,std::vector<textureData>* td)
{
    QOpenGLFunctions::initializeOpenGLFunctions();
    _name = name;
    _data=*vd;
    _indices=*id;
    if(td)
        _textures=*td;

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,_data.size()*sizeof(vertexData),&_data[0],GL_STATIC_DRAW);

    glGenBuffers(1,&IND);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IND);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,_indices.size()*sizeof(_indices[0]),&_indices[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

}

mesh::~mesh()
{
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&IND);
}

void mesh::draw(unsigned int programId)
{
    //attribute vec3 vertex
    int vertex=glGetAttribLocation(programId,"vertex"); //0

    int normal=glGetAttribLocation(programId,"normal"); //1
    int tangent=glGetAttribLocation(programId,"tangent"); //2
    int color=glGetAttribLocation(programId,"color"); //3
    int UV=glGetAttribLocation(programId,"UV"); //4

    //texture0
    //texture1...
    std::string str="texture";
    for(int i=0;i<_textures.size();i++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D,_textures[i].id);
        glUniform1i(glGetUniformLocation(programId,(str+(char)(i+'0')).c_str()),i);
    }

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IND);

    if( vertex != -1)
    {
        glEnableVertexAttribArray(vertex);
        glVertexAttribPointer(vertex,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),0);
    }

    if( normal != -1)
    {
        glEnableVertexAttribArray(normal);
        glVertexAttribPointer(normal,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(3*sizeof(float)));
    }

    if( tangent != -1)
    {
        glEnableVertexAttribArray(tangent);
        glVertexAttribPointer(tangent,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(6*sizeof(float)));
    }

    if( color != -1)
    {
        glEnableVertexAttribArray(color);
        glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(9*sizeof(float)));
    }

    if( UV != -1)
    {
        glEnableVertexAttribArray(UV);
        glVertexAttribPointer(UV,2,GL_FLOAT,GL_FALSE,sizeof(vertexData),(void*)(12*sizeof(float)));
    }

    glDrawElements(GL_TRIANGLES,_indices.size(),GL_UNSIGNED_SHORT,0);

    if(vertex != -1)    glDisableVertexAttribArray(vertex);
    if(normal != -1)    glDisableVertexAttribArray(normal);
    if(tangent != -1)   glDisableVertexAttribArray(tangent);
    if(color != -1)    glDisableVertexAttribArray(color);
    if(UV != -1)    glDisableVertexAttribArray(UV);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

std::string mesh::name() const
{
    return _name;
}
