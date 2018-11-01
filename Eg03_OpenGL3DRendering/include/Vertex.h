#pragma once
#include <QVector3D>

class Vertex {
   public:
    // constructors
    Vertex();
    explicit Vertex(const QVector3D& pos);
    Vertex(const QVector3D& pos, const QVector3D& color);

    // accessors/ mutators
    inline const QVector3D& position() const { return pos_; }
    inline const QVector3D& color() const { return color_; }
    void setPos(const QVector3D& pos);
    void setColor(const QVector3D& color);

    // OpenGL helpers
    static const int PositionTupleSize = 3;
    static const int ColorTupleSize = 3;
    static inline constexpr int positionOffset() { return offsetof(Vertex, pos_); }
    static inline constexpr int colorOffset() { return offsetof(Vertex, color_); }
    static inline constexpr int stride() { return sizeof(Vertex); }

   private:
    QVector3D pos_;
    QVector3D color_;
};
