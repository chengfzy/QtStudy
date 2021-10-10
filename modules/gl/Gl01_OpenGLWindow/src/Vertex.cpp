#include "Vertex.h"

Vertex::Vertex() {}

Vertex::Vertex(const QVector3D& pos) : pos_(pos) {}

Vertex::Vertex(const QVector3D& pos, const QVector3D& color) : pos_(pos), color_(color) {}

void Vertex::setPos(const QVector3D& pos) { pos_ = pos; }

void Vertex::setColor(const QVector3D& color) { color_ = color; }