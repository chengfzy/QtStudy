#pragma once
#include <ft2build.h>
#include <QObject>
#include <QOffscreenSurface>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QThread>
#include <QVector2D>
#include FT_FREETYPE_H

struct Character {
    QOpenGLTexture* TextureID;
    QVector2D Size;     // Size of glyph
    QVector2D Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};

class FontProvider : public QThread, protected QOpenGLFunctions {
    Q_OBJECT
  public:
    explicit FontProvider();
    ~FontProvider();

  signals:
    void finished();

  public slots:
    void initializeFontProvider();

  private:
  public:
    QOpenGLContext* context = nullptr;
    QOffscreenSurface* surface = nullptr;
    void initFontGeometry();
    std::map<FT_ULong, Character> fontCharactersBold;
    void drawFontGeometry(QOpenGLShaderProgram* programFont, GLfloat pos_x, GLfloat pos_y, const QString& text,
                          GLfloat scale_factor);
};
