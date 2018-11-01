#include "Vertex.h"

Vertex::Vertex() {}
Vertex::Vertex(const QVector3D& pos) : pos_(pos) {}
Vertex::Vertex(const QVector3D& pos, const QVector3D& color) : pos_(pos), color_(color) {}
