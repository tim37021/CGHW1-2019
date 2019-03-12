#include "ShaderProgram.h"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

UniformVariable::UniformVariable()
    : m_id(0)
{

}
UniformVariable::UniformVariable(const GLuint program, GLint id)
    : m_program(program), m_id(id)
{

}
float UniformVariable::operator=(float val)
{
    glUseProgram(m_program);
    glUniform1f(m_id, val);
    return val;
}
uint32_t UniformVariable::operator=(uint32_t val)
{
    glUseProgram(m_program);
    glUniform1ui(m_id, val);
    return val;
}
int32_t UniformVariable::operator=(int32_t val)
{
    glUseProgram(m_program);
    glUniform1i(m_id, val);
    return val;
}
glm::uvec2 UniformVariable::operator=(const glm::uvec2 &v)
{
    glUseProgram(m_program);
    glUniform2uiv(m_id, 1, glm::value_ptr(v));
    return v;
}
glm::vec2 UniformVariable::operator=(const glm::vec2 &v)
{
    glUseProgram(m_program);
    glUniform2fv(m_id, 1, glm::value_ptr(v));
    return v;
}
glm::vec3 UniformVariable::operator=(const glm::vec3 &v)
{
    glUseProgram(m_program);
    glUniform3fv(m_id, 1, glm::value_ptr(v));
    return v;
}

glm::mat4 UniformVariable::operator=(const glm::mat4 &v)
{
    glUseProgram(m_program);
    // mat4 of glm is column major, same as opengl
    // we don't need to transpose it. so..GL_FALSE
    glUniformMatrix4fv(m_id, 1, GL_FALSE, glm::value_ptr(v));
    return v;
}

bool UniformVariable::valid() const
{
    return m_id != -1;
}

static std::string read_file(const std::string &filename) 
{
    std::ifstream t(filename);
    if(!t.good())
        throw std::runtime_error(filename + " not found!");
	return std::string((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
}

Program::Program(GLuint prog)
    : m_program(prog)
{

}

Program Program::LoadFromFile(const std::string &filename)
{
    auto cs_src = ::read_file(filename);

    try{
        GLuint cshader = CreateShader(GL_COMPUTE_SHADER, cs_src);

        GLuint id = glCreateProgram();
        glAttachShader(id, cshader);
        glLinkProgram(id);
    
        glDeleteShader(cshader);
        return Program(id);
    } catch(std::exception &e) {
        std::cerr<<e.what();
        return Program(0);
    }
}
#include <iostream>
Program Program::LoadFromFile(const std::string &vs, const std::string &fs)
{
    auto vs_src = ::read_file(vs);
	auto fs_src = ::read_file(fs);

    try{
        GLuint vshader = CreateShader(GL_VERTEX_SHADER, vs_src);
        GLuint fshader = CreateShader(GL_FRAGMENT_SHADER, fs_src);

        GLuint id = glCreateProgram();
        glAttachShader(id, vshader);
        glAttachShader(id, fshader);
        glLinkProgram(id);
    
        glDeleteShader(vshader);
        glDeleteShader(fshader);
        return Program(id);
    } catch(std::exception &e) {
        std::cerr<<e.what();
        return Program(0);
    }
}

#include <iostream>
Program Program::LoadFromFile(const std::string &vs, const std::string &gs, const std::string &fs)
{
    auto vs_src = ::read_file(vs);
    auto gs_src = ::read_file(gs);
	auto fs_src = ::read_file(fs);

    try{
        GLuint vshader = CreateShader(GL_VERTEX_SHADER, vs_src);
        GLuint gshader = CreateShader(GL_GEOMETRY_SHADER, gs_src);
        GLuint fshader = CreateShader(GL_FRAGMENT_SHADER, fs_src);

        GLuint id = glCreateProgram();
        glAttachShader(id, vshader);
        glAttachShader(id, gshader);
        glAttachShader(id, fshader);
        glLinkProgram(id);
    
        glDeleteShader(vshader);
        glDeleteShader(gshader);
        glDeleteShader(fshader);
        return Program(id);
    } catch(std::exception &e) {
        std::cerr<<e.what();
        return Program(0);
    }
}

GLuint Program::CreateShader(GLenum type, const std::string &src) 
{
    GLuint id=glCreateShader(type);
    int len = static_cast<int>(src.length());
    const char *c = src.c_str();
    glShaderSource(id, 1, &c, &len);
    glCompileShader(id);

    int status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE) {
        const char *shader_type;
        switch(type) {
            case GL_VERTEX_SHADER:
                shader_type = "VS:"; break;
            case GL_GEOMETRY_SHADER:
                shader_type = "GS:"; break; 
            case GL_FRAGMENT_SHADER:
                shader_type = "FS:"; break;
            case GL_COMPUTE_SHADER:
                shader_type = "CS:"; break;
            
        }
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        
        std::vector<char> err_str(len);
        glGetShaderInfoLog(id, len, &len, err_str.data());
        glDeleteShader(id);
        throw std::runtime_error(std::string(shader_type)+err_str.data());
    }

    return id;
}


bool Program::valid() const
{
    return m_program!=0;
}

void Program::use() const
{
    glUseProgram(m_program);
}

UniformVariable &Program::operator[](const std::string &name)
{
    // TODO
    use();
    auto it = m_uniformVariables.find(name);
    if(it==m_uniformVariables.cend()) {
        auto &obj = m_uniformVariables[name];
        obj = UniformVariable(m_program, glGetUniformLocation(m_program, name.c_str()));
        return obj;
    } else
        return it->second;
}

void Program::release()
{
    glDeleteProgram(m_program);
    m_program = 0;
}