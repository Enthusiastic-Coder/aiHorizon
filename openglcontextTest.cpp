#include "openglcontextTest.h"
#include <QOpenGLFunctions>

void doTest(QOpenGLFunctions *f)
{
    const char* s1 = (const char*)f->glGetString(GL_VERSION);
    const char* s2 = (const char*)f->glGetString(GL_RENDERER);
    const char* s3 = (const char*)f->glGetString(GL_VENDOR);
}
