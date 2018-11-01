#include "Transform3D.h"

Transform3D::Transform3D() : dirty_(true), scale_(1.0f, 1.0f, 1.0f) {}

void Transform3D::translate(const QVector3D& dt) {
    dirty_ = true;
    translation_ += dt;
}
void Transform3D::scale(const QVector3D& ds) {
    dirty_ = true;
    scale_ *= ds;
}
void Transform3D::scale(const float& factor) { scale(QVector3D(factor, factor, factor)); }
void Transform3D::rotate(const QQuaternion& dr) {
    dirty_ = true;
    rotation_ = dr * rotation_;
}
void Transform3D::rotate(const float& angle, const QVector3D& axis) {
    rotate(QQuaternion::fromAxisAndAngle(axis, angle));
}
void Transform3D::rotate(const float& angle, const float& ax, const float& ay, const float& az) {
    rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle));
}
void Transform3D::grow(const QVector3D& ds) {
    dirty_ = true;
    scale_ + ds;
}
void Transform3D::grow(float factor) { grow(QVector3D(factor, factor, factor)); }

void Transform3D::setTranslation(const QVector3D& t) {
    dirty_ = true;
    translation_ = t;
}

void Transform3D::setScale(const QVector3D& s) {
    dirty_ = true;
    scale_ = s;
}
void Transform3D::setScale(const float& k) { setScale(QVector3D(k, k, k)); }

void Transform3D::setRotation(const QQuaternion& r) {
    dirty_ = true;
    rotation_ = r;
}

void Transform3D::setRotation(const float& angle, const QVector3D& axis) {
    setRotation(QQuaternion::fromAxisAndAngle(axis, angle));
}

void Transform3D::setRotation(const float& angle, const float& ax, const float& ay, const float& az) {
    setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle));
}

const QMatrix4x4 Transform3D::toMatrix() {
    if (dirty_){
        dirty_ = false;
        world_.setToIdentity();
        world_.translate(translation_);
        world_.rotate(rotation_);
        world_.scale(scale_);
    }

    return world_;
}
