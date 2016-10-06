#include "aimeshloader.h"

AIMeshLoader::AIMeshLoader(std::string filename)
    :meshLoader(filename)
{

}

void AIMeshLoader::draw(unsigned int programId)
{
    for(size_t i=0;i < _meshes.size();i++)
    {
        std::string name = _meshes[i]->name();
        bool bMod=false;
        if( name == "secondary_ai_color_Disk")
        {
            bMod = true;


        }
        else
            _meshes[i]->draw(programId);

    }
}

