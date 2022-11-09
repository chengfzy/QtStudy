#pragma once
#include <ft2build.h>
#include <QOffscreenSurface>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector2D>
#include FT_FREETYPE_H

struct Character {
    QOpenGLTexture* texture;  // texture
    QVector2D size;           // Size of glyph
    QVector2D baring;         // Offset from baseline to left/top of glyph
    GLuint advance;           // Horizontal offset to advance to next glyph
};

struct TextureQuad {
    float x;         // position x
    float y;         // position y
    float textureX;  // texture pos x
    float textureY;  // texture pos y
};

struct RenderTextPoints {
    std::vector<QOpenGLTexture*> textures;
    std::vector<std::array<TextureQuad, 6>> vertices;
};

struct Quad {
    // using Vertex = std::array<float, 4>;
    std::vector<QOpenGLTexture*> textures;
    std::vector<std::array<std::array<float, 4>, 6>> vertices;
};

class FontRender {
  public:
    explicit FontRender();
    ~FontRender() = default;

  public:
    void setup();

    void init();

    void drawFontGeometry(GLfloat pos_x, GLfloat pos_y, const QString& text, GLfloat scale_factor,
                          const QMatrix4x4& rotationMatrix, const QVector4D& color);

    void setText(float x, float y, const QString& text, float scale);
    void render(const QMatrix4x4& rotationMatrix, const QVector4D& color);

  private:
    void initFontGeometry();

  private:
  public:
    std::map<FT_ULong, Character> characters_;

    std::size_t dataNum_ = 0;  // point data number
    RenderTextPoints texData_;
    QOpenGLShaderProgram* shader_;
    QOpenGLBuffer* vbo_ = nullptr;  // vertex buffer object
};
