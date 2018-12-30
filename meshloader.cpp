#include "meshloader.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <meshData.h>
#include <QImage>
#include <QFileInfo>
#include <QDebug>

meshLoader::meshLoader(std::string filename)
{
    QOpenGLFunctions::initializeOpenGLFunctions();
    std::string::size_type size = filename.find_last_of("/\\");

    std::string baseFolder;
    if( size != std::string::npos)
        baseFolder = filename.substr(0, size );

    const aiScene* scene = aiImportFile( filename.c_str(),
                                         aiProcess_GenSmoothNormals
                                         | aiProcess_Triangulate
                                         | aiProcess_CalcTangentSpace
                                         | aiProcess_FlipUVs);

    if (scene == NULL)
    {
        std::cout << "The file wasn't successfuly opened " << filename << std::endl;
        std::cout << "Error : " << aiGetErrorString() << std::endl;

        return;
    }

    std::cout << "List of Textures " << std::endl << "--------------------" << std::endl;
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
    {
        aiMaterial *material = scene->mMaterials[i];
        aiString str;

        for (unsigned int j = 0; j < material->GetTextureCount(aiTextureType_DIFFUSE); ++j)
        {
            material->GetTexture(aiTextureType_DIFFUSE,j, &str);
            if (textures_[std::string(str.C_Str())] == 0)
            {
                std::cout << "[" << i << ", " << j << "] " << str.C_Str() << std::endl;
                textures_[std::string(str.C_Str())] = loadTexture(baseFolder + "/" + str.C_Str());
            }
        }
    }
    std::cout << "--------------" << std::endl;

    qDebug() << "Error : " << aiGetErrorString();

    recursiveProcess(scene->mRootNode, scene);
}

meshLoader::~meshLoader()
{
    for (int i = 0; i<_meshes.size(); i++)
        delete _meshes[i];
}

void meshLoader::recursiveProcess(aiNode* node,const aiScene* scene)
{
    //process
    for(int i=0;i<node->mNumMeshes;i++)
    {
        aiMesh* mesh=scene->mMeshes[node->mMeshes[i]];

        processMesh(node->mName, mesh,scene);
    }

    //recursion
    for(int i=0;i<node->mNumChildren;i++)
    {
        recursiveProcess(node->mChildren[i],scene);
    }
}

void meshLoader::processMesh(aiString name, aiMesh* m, const aiScene* scene)
{
    std::vector<vertexData> data;
    std::vector<unsigned short> indices;
    std::vector<textureData> textures;
    aiColor4D col;
    aiMaterial* mat=scene->mMaterials[m->mMaterialIndex];
    aiGetMaterialColor(mat,AI_MATKEY_COLOR_DIFFUSE,&col);
    vector3d defaultColor(col.r,col.g,col.b);

    for(int i=0;i<m->mNumVertices;i++)
    {
            vertexData tmp;
            vector3d tmpVec;

            //position
            tmpVec.x=m->mVertices[i].x;
            tmpVec.y=m->mVertices[i].y;
            tmpVec.z=m->mVertices[i].z;
            tmp.position=tmpVec;

            //normals
            tmpVec.x=m->mNormals[i].x;
            tmpVec.y=m->mNormals[i].y;
            tmpVec.z=m->mNormals[i].z;
            tmp.normal=tmpVec;


            //tangent
            if(m->mTangents)
            {
                tmpVec.x=m->mTangents[i].x;
                tmpVec.y=m->mTangents[i].y;
                tmpVec.z=m->mTangents[i].z;
            }else{
                tmpVec.x=1.0;
                tmpVec.y=tmpVec.z=0;
            }
            tmp.tangent=tmpVec;


            //colors
            if(m->mColors[0])
            {
                //!= material color
                tmpVec.x=m->mColors[0][i].r;
                tmpVec.y=m->mColors[0][i].g;
                tmpVec.z=m->mColors[0][i].b;
            }else{
                tmpVec=defaultColor;
            }
            tmp.color=tmpVec;

            //color
            if(m->mTextureCoords[0])
            {
                tmpVec.x=m->mTextureCoords[0][i].x;
                tmpVec.y=m->mTextureCoords[0][i].y;
            }else{
                tmpVec.x=tmpVec.y=tmpVec.z=0.0;
            }
            tmp.U=tmpVec.x;
            tmp.V=tmpVec.y;
            data.push_back(tmp);
    }

    for(int i=0;i<m->mNumFaces;i++)
    {
        aiFace face=m->mFaces[i];
        for(int j=0;j<face.mNumIndices;j++) //0..2
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    for(int i=0;i<mat->GetTextureCount(aiTextureType_DIFFUSE);i++)
    {
        aiString str;
        mat->GetTexture(aiTextureType_DIFFUSE,i,&str);
        textureData tmp;
        tmp.id = textures_[std::string(str.C_Str())];
        tmp.type=0;
        textures.push_back(tmp);
    }

     _meshes.push_back(new mesh(name.C_Str(),&data,&indices,&textures));
}


unsigned int meshLoader::loadTexture(std::string filename)
{
    unsigned int num = -1;
    glGenTextures(1,&num);

    QImage img = QImage(filename.c_str()).convertToFormat(QImage::Format_RGBA8888);

    glBindTexture(GL_TEXTURE_2D,num);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,
                        GL_RGBA, img.width(), img.height(),
                    0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
    return num;
}

void meshLoader::draw(unsigned int programId)
{
    for(int i=0;i < _meshes.size();i++)
    {
        _meshes[i]->draw(programId);
    }
}

std::vector<mesh*>& meshLoader::getMeshes()
{
    return _meshes;
}
