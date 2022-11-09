#include "FontRender.h"
#include <glog/logging.h>
#include <iostream>

using namespace std;

FontRender::FontRender() {
    shader_ = new QOpenGLShaderProgram;
    vbo_ = new QOpenGLBuffer;
}

void FontRender::setup() {
    // init
    // initializeOpenGLFunctions();
    initFontGeometry();
}

void FontRender::init() {
    initFontGeometry();

    // create shader
    vbo_->destroy();
    if (shader_->isLinked()) {
        shader_->release();
        shader_->removeAllShaders();
    }
    shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/text.vert");
    shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/text.frag");
    if (!shader_->link()) {
        qDebug() << "shader link failed";
    }
    if (!shader_->bind()) {
        qDebug() << "shader bind failed";
    }
    vbo_->create();
}

void FontRender::drawFontGeometry(GLfloat pos_x, GLfloat pos_y, const QString& text, GLfloat scale_factor,
                                  const QMatrix4x4& rotationMatrix, const QVector4D& color) {
    shader_->bind();
    shader_->setUniformValue("mvp_matrix", rotationMatrix);
    shader_->setUniformValue("textColor", color);
    shader_->setUniformValue("text", 0);
    auto f = QOpenGLContext::currentContext()->functions();

    int nCount = text.count();
    for (int i = 0; i < nCount; i++) {
        QChar cha = text.at(i);
        Character ch;
        ushort uni = cha.unicode();
        ch = characters_[uni];

        if (ch.texture != NULL) {
            GLfloat xpos = pos_x + ch.baring.x() * scale_factor;
            GLfloat ypos = pos_y - (ch.size.y() - ch.baring.y()) * scale_factor;

            GLfloat w = ch.size.x() * scale_factor;
            GLfloat h = ch.size.y() * scale_factor;
            GLfloat vertices[6][4] = {{xpos, ypos + h, 0.0, 0.0}, {xpos, ypos, 0.0, 1.0},
                                      {xpos + w, ypos, 1.0, 1.0}, {xpos, ypos + h, 0.0, 0.0},
                                      {xpos + w, ypos, 1.0, 1.0}, {xpos + w, ypos + h, 1.0, 0.0}};
            ch.texture->bind();
            QOpenGLBuffer* arrayBufFont = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
            arrayBufFont->create();
            arrayBufFont->bind();
            arrayBufFont->allocate(vertices, 4 * sizeof(vertices));

            shader_->enableAttributeArray("a_position");
            shader_->setAttributeBuffer("a_position", GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));
            shader_->enableAttributeArray("a_texcoord");
            shader_->setAttributeBuffer("a_texcoord", GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));

            f->glDrawArrays(GL_TRIANGLES, 0, 6);
            arrayBufFont->release();
            arrayBufFont->destroy();
            ch.texture->release();
            pos_x += (ch.advance >> 6) * scale_factor;  // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount
                                                        // of 1/64th pixels by 64 to get amount of pixels))
        }
    }

    shader_->release();
}

void FontRender::setText(float x, float y, const QString& text, float scale) {
    for (auto& v : text) {
        auto& ch = characters_[QChar(v).unicode()];

        if (ch.texture != NULL) {
            GLfloat xpos = x + ch.baring.x() * scale;
            GLfloat ypos = y - (ch.size.y() - ch.baring.y()) * scale;

            GLfloat w = ch.size.x() * scale;
            GLfloat h = ch.size.y() * scale;
            array<TextureQuad, 6> vertices = {
                TextureQuad{xpos, ypos + h, 0.0, 0.0}, TextureQuad{xpos, ypos, 0.0, 1.0},
                TextureQuad{xpos + w, ypos, 1.0, 1.0}, TextureQuad{xpos, ypos + h, 0.0, 0.0},
                TextureQuad{xpos + w, ypos, 1.0, 1.0}, TextureQuad{xpos + w, ypos + h, 1.0, 0.0}};

            // calculate VBO
            texData_.textures.emplace_back(ch.texture);
            texData_.vertices.emplace_back(vertices);

            x += (ch.advance >> 6) * scale;  // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount
                                             // of 1/64th pixels by 64 to get amount of pixels))
        }
    }

    // upload vbo
    shader_->bind();
    vbo_->bind();
    vbo_->allocate(texData_.vertices.size() * 6 * sizeof(TextureQuad));
    vbo_->write(0, texData_.vertices.data(), texData_.vertices.size() * 6 * sizeof(TextureQuad));

    // position
    shader_->enableAttributeArray("a_position");
    shader_->setAttributeBuffer("a_position", GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));
    // texture coordinate
    shader_->enableAttributeArray("a_texcoord");
    shader_->setAttributeBuffer("a_texcoord", GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));

    vbo_->release();
    shader_->release();
}

void FontRender::render(const QMatrix4x4& rotationMatrix, const QVector4D& color) {
    shader_->bind();

    shader_->setUniformValue("mvp_matrix", rotationMatrix);
    shader_->setUniformValue("textColor", color);
    shader_->setUniformValue("text", 0);

    auto f = QOpenGLContext::currentContext()->functions();
    for (size_t i = 0; i < texData_.textures.size(); ++i) {
        texData_.textures[i]->bind();
        f->glDrawArrays(GL_TRIANGLES, i * 6, 6);
        texData_.textures[i]->release();
    }

    shader_->release();
}

void FontRender::initFontGeometry() {
    // FreeType
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) qDebug() << "ERROR::FREETYPE: Could not init FreeType Library\n";

    // Load font as face
    // string fontFile{"./modules/gl/Gl05_TextRenderSingleThread/res/font/AntonioRegular.ttf"};
    string fontFile{"./modules/gl/Gl05_TextRenderSingleThread/res/font/ReadboldV2000.ttf"};
    FT_Face face;
    if (FT_New_Face(ft, fontFile.c_str(), 0, &face)) {
        qDebug() << "ERROR::FREETYPE: Failed to load font\n";
    } else {
        FT_Select_Charmap(face, FT_ENCODING_UNICODE);
        // Set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // Disable byte-alignment restriction
        auto f = QOpenGLContext::currentContext()->functions();
        f->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        FT_UInt index;
        FT_ULong c = FT_Get_First_Char(face, &index);

        // Load all glyphs in ttf file
        while (index) {
            // Load character glyph
            if (FT_Load_Glyph(face, index, FT_LOAD_RENDER)) {
                qDebug() << "ERROR::FREETYTPE: Failed to load Glyph\n";
                continue;
            }
            // Generate texture
            QOpenGLTexture* text = new QOpenGLTexture(QOpenGLTexture::Target::Target2D);
            text->setMinificationFilter(QOpenGLTexture::Nearest);
            text->setMagnificationFilter(QOpenGLTexture::Linear);
            text->setWrapMode(QOpenGLTexture::Repeat);
            text->create();
            text->setSize(face->glyph->bitmap.width, face->glyph->bitmap.rows, 1);
            text->setFormat(QOpenGLTexture::RGBA8_UNorm);
            // text->setFormat(QOpenGLTexture::AlphaFormat);
            // text->allocateStorage();
            text->allocateStorage(QOpenGLTexture::Alpha, QOpenGLTexture::UInt8);
            text->setData(QOpenGLTexture::Alpha, QOpenGLTexture::UInt8, face->glyph->bitmap.buffer);
            Character character = {text, QVector2D(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                                   QVector2D(face->glyph->bitmap_left, face->glyph->bitmap_top),
                                   face->glyph->advance.x};
            characters_.insert(pair<FT_ULong, Character>(c, character));
            text->release();
            c = FT_Get_Next_Char(face, c, &index);
        }
        qDebug() << QString("font character count = %1").arg(characters_.size());
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
}