#pragma once

#include <GL/gl3w.h>

#include <array>
#include <cstring>

#include "common.h"

struct Position {
    Position(GLfloat x, GLfloat y) : x(x), y(y) {};
    GLfloat x, y;
};

struct Color {
    Color(GLfloat r, GLfloat g, GLfloat b) : r(r), g(g), b(b) {};
    GLfloat r, g, b;
};

class Renderer {
public:
    ~Renderer();
    void Init();

    void Draw();
    void PushTriangle(std::array<Position, 3> vertices, std::array<Color, 3> color);
private:
    GLuint GetAttributeIndex(const char* attribute);

    static constexpr u32 BUFFER_SIZE = 2048;

    template <typename T>
    struct Buffer {
        void Init() {
            glGenBuffers(1, &handle);
            glBindBuffer(GL_ARRAY_BUFFER, handle);

            constexpr GLsizeiptr size = sizeof(T) * BUFFER_SIZE;
            glBufferStorage(GL_ARRAY_BUFFER, size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
            ptr = static_cast<T*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT));
            Assert(ptr);

            std::memset((void*)ptr, 0, size);
        }

        ~Buffer() {
            glBindBuffer(GL_ARRAY_BUFFER, handle);
            glUnmapBuffer(GL_ARRAY_BUFFER);
            glDeleteBuffers(1, &handle);
        }

        void Set(u32 index, T value) {
            Assert(index < BUFFER_SIZE);
            ptr[index] = value;
        }

        GLuint handle = 0;
        T* ptr = nullptr;
    };

    Buffer<Position> positions;
    Buffer<Color> colors;
    u32 curr_vertices = 0;

    GLuint program = 0;
    GLuint VAO = 0;
};