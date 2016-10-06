#ifndef AIMESHLOADER_H
#define AIMESHLOADER_H

#include <meshloader.h>

class AIMeshLoader : public meshLoader
{
public:
    AIMeshLoader(std::string filename);

    void draw(unsigned int programId);

    float _fBank = 0.0f;
    float _fPitch = 0.0f;
};

#endif // AIMESHLOADER_H
