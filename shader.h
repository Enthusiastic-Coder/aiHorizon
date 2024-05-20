#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <QOpenGLFunctions>

class shader : private QOpenGLFunctions {
	unsigned int vs,fs,program;
	void loadFile(const char* fn,std::string& str);
	unsigned int loadShader(std::string& source,unsigned int mode);
	public:
        shader(std::string vss,std::string fss);
		~shader();
		void use();
		void release();
		unsigned int getProgramId();
};

#endif
