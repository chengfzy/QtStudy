#pragma once
#include "QMatrix4x4"
#include "QQuaternion"
#include "QVector3D"

class Transform3D {
   public:
    Transform3D();

    // Transform by (add/scale)
    void translate(const QVector3D& dt);
    void scale(const QVector3D& ds);
    void scale(const float& factor);
    void rotate(const QQuaternion& dr);
    void rotate(const float& angle, const QVector3D& axis);
    void rotate(const float& angle, const float& ax, const float& ay, const float& az);
    void grow(const QVector3D& ds);
    void grow(float factor);

    // Transform to (setters)
    void setTranslation(const QVector3D& t);
    void setScale(const QVector3D& s);
    void setScale(const float& k);
    void setRotation(const QQuaternion& r);
    void setRotation(const float& angle, const QVector3D& axis);
    void setRotation(const float& angle, const float& ax, const float& ay, const float& az);

    // Accessors
    inline const QVector3D& translation() const { return translation_; }
    inline const QVector3D& scale() const { return scale_; }
    inline const QQuaternion& rotation() const { return rotation_; }
    const QMatrix4x4 toMatrix();

   private:
    bool dirty_;
    QVector3D translation_;
    QVector3D scale_;
    QQuaternion rotation_;
    QMatrix4x4 world_;
};
