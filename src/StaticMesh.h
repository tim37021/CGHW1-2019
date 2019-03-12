#ifndef _STATIC_MESH_H_
#define _STATIC_MESH_H_

#include <glad/glad.h>
#include <string>
class StaticMesh {
public:
    StaticMesh(const StaticMesh &rhs)=default;
    void release();

    static StaticMesh LoadMesh(const std::string &filename);
    void draw();

	bool hasNormal() const;
	bool hasUV() const;

    bool operator!=(const StaticMesh &rhs) const
    { return vao != rhs.vao; }
private:
    StaticMesh();
    GLuint vao;
    GLuint vbo[3];
    GLuint ibo;
    GLuint numIndices;

	bool m_uv, m_normal;
};

#endif