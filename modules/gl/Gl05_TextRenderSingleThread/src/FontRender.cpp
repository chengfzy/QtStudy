#include "FontRender.h"
#include <glog/logging.h>
#include <iostream>

using namespace std;

FontRender::FontRender() {
    context = new QOpenGLContext;
    surface = new QOffscreenSurface;
    shader_ = new QOpenGLShaderProgram;
}

void FontRender::setup() {
    // init
    context->makeCurrent(surface);
    initializeOpenGLFunctions();
    initFontGeometry();
}

void FontRender::init() {
    context->makeCurrent(surface);
    initializeOpenGLFunctions();
    initFontGeometry();

    // create shader
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
}

void FontRender::drawFontGeometry(QOpenGLShaderProgram* programFont, GLfloat pos_x, GLfloat pos_y, const QString& text,
                                  GLfloat scale_factor) {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
            GLfloat vertices[6][4] = {
                {xpos, ypos + h, 0.0, 0.0}, {xpos, ypos, 0.0, 1.0},     {xpos + w, ypos, 1.0, 1.0},

                {xpos, ypos + h, 0.0, 0.0}, {xpos + w, ypos, 1.0, 1.0}, {xpos + w, ypos + h, 1.0, 0.0}};
            ch.texture->bind();
            QOpenGLBuffer* arrayBufFont = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
            arrayBufFont->create();
            arrayBufFont->bind();
            arrayBufFont->allocate(vertices, 4 * sizeof(vertices));
            int vertexLocation = programFont->attributeLocation("a_position");
            programFont->enableAttributeArray(vertexLocation);
            programFont->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));
            int texcoordLocation = programFont->attributeLocation("a_texcoord");
            programFont->enableAttributeArray(texcoordLocation);
            programFont->setAttributeBuffer(texcoordLocation, GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            arrayBufFont->release();
            arrayBufFont->destroy();
            ch.texture->release();
            pos_x += (ch.advance >> 6) * scale_factor;  // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount
                                                        // of 1/64th pixels by 64 to get amount of pixels))
        }
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void FontRender::drawFontGeometry(GLfloat pos_x, GLfloat pos_y, const QString& text, GLfloat scale_factor,
                                  const QMatrix4x4& rotationMatrix, const QVector4D& color) {
    shader_->bind();

    shader_->setUniformValue("mvp_matrix", rotationMatrix);
    shader_->setUniformValue("textColor", color);
    shader_->setUniformValue("text", 0);

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
            GLfloat vertices[6][4] = {
                {xpos, ypos + h, 0.0, 0.0}, {xpos, ypos, 0.0, 1.0},     {xpos + w, ypos, 1.0, 1.0},

                {xpos, ypos + h, 0.0, 0.0}, {xpos + w, ypos, 1.0, 1.0}, {xpos + w, ypos + h, 1.0, 0.0}};
            ch.texture->bind();
            QOpenGLBuffer* arrayBufFont = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
            arrayBufFont->create();
            arrayBufFont->bind();
            arrayBufFont->allocate(vertices, 4 * sizeof(vertices));
            int vertexLocation = shader_->attributeLocation("a_position");
            shader_->enableAttributeArray(vertexLocation);
            shader_->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));
            int texcoordLocation = shader_->attributeLocation("a_texcoord");
            shader_->enableAttributeArray(texcoordLocation);
            shader_->setAttributeBuffer(texcoordLocation, GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            arrayBufFont->release();
            arrayBufFont->destroy();
            ch.texture->release();
            pos_x += (ch.advance >> 6) * scale_factor;  // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount
                                                        // of 1/64th pixels by 64 to get amount of pixels))
        }
    }

    shader_->release();
}

void FontRender::setText(float x, float y, const std::string& text, float scale) {
    // for (auto& v : text) {
    //     auto& ch = characters_[QChar(v).unicode()];

    //     if (ch.texture != NULL) {
    //         GLfloat xpos = x + ch.baring.x() * scale;
    //         GLfloat ypos = y - (ch.size.y() - ch.baring.y()) * scale;

    //         GLfloat w = ch.size.x() * scale;
    //         GLfloat h = ch.size.y() * scale;
    //         GLfloat vertices[6][4] = {{xpos, ypos + h, 0.0, 0.0}, {xpos, ypos, 0.0, 1.0},
    //                                   {xpos + w, ypos, 1.0, 1.0}, {xpos, ypos + h, 0.0, 0.0},
    //                                   {xpos + w, ypos, 1.0, 1.0}, {xpos + w, ypos + h, 1.0, 0.0}};

    //         ch.texture->bind();
    //         QOpenGLBuffer* arrayBufFont = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    //         arrayBufFont->create();
    //         arrayBufFont->bind();
    //         arrayBufFont->allocate(vertices, 4 * sizeof(vertices));
    //         int vertexLocation = programFont->attributeLocation("a_position");
    //         programFont->enableAttributeArray(vertexLocation);
    //         programFont->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));
    //         int texcoordLocation = programFont->attributeLocation("a_texcoord");
    //         programFont->enableAttributeArray(texcoordLocation);
    //         programFont->setAttributeBuffer(texcoordLocation, GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));
    //         glDrawArrays(GL_TRIANGLES, 0, 6);
    //         arrayBufFont->release();
    //         arrayBufFont->destroy();
    //         ch.texture->release();

    //         x += (ch.advance >> 6) * scale;  // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount
    //                                          // of 1/64th pixels by 64 to get amount of pixels))
    //     }
    // }
}

void FontRender::render(const QMatrix4x4& rotationMatrix, const QVector4D& color) {
    shader_->bind();

    shader_->setUniformValue("mvp_matrix", rotationMatrix);
    shader_->setUniformValue("textColor", color);
    shader_->setUniformValue("text", 0);
    // auto f = QOpenGLContext::currentContext()->functions();
    // f->glDrawArrays(GL_TRIANGLES, 0, 6);

    shader_->release();
}

void FontRender::initFontGeometry() {
    // FreeType
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) qDebug() << "ERROR::FREETYPE: Could not init FreeType Library\n";

    // Load font as face
    // std::string fontFile{"./modules/gl/Gl05_TextRenderSingleThread/res/font/AntonioRegular.ttf"};
    std::string fontFile{"./modules/gl/Gl05_TextRenderSingleThread/res/font/ReadboldV2000.ttf"};
    FT_Face face;
    if (FT_New_Face(ft, fontFile.c_str(), 0, &face)) {
        qDebug() << "ERROR::FREETYPE: Failed to load font\n";
    } else {
        FT_Select_Charmap(face, FT_ENCODING_UNICODE);
        // Set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
            characters_.insert(std::pair<FT_ULong, Character>(c, character));
            text->release();
            c = FT_Get_Next_Char(face, c, &index);
        }
        qDebug() << QString("font character count = %1").arg(characters_.size());
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
}