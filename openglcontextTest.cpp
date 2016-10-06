#include "openglcontextTest.h"
#include <QOpenGLFunctions>

void doTest(QOpenGLWindow *window)
{
    const char* s1 = (const char*)window->context()->functions()->glGetString(GL_VERSION);
    const char* s2 = (const char*)window->context()->functions()->glGetString(GL_RENDERER);
    const char* s3 = (const char*)window->context()->functions()->glGetString(GL_VENDOR);
}
