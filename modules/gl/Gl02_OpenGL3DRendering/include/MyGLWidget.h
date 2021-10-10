#pragma once
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include "Transform3D.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
   public:
    explicit MyGLWidget(QWidget* parent = nullptr);
    ~MyGLWidget() = default;

   protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL();

   protected slots:
    void update();
    void teardownGL();

   private:
    void printContextInformation();

   private:
    // OpenGL state information
    QOpenGLBuffer vertex_;
    QOpenGLVertexArrayObject object_;
    QOpenGLShaderProgram* program_;

    // shader infromation
    int modelToWorld_;
    int worldToView_;
    QMatrix4x4 projection_;
    Transform3D transform_;
};
