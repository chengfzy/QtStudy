#pragma once
#include <QWidget>

/**
 * @brief Image viewer widget to show image
 */
class ImageViewer : public QWidget {
  public:
    /**
     * @brief Construct with parent widget
     * @param parent Parent widget
     */
    explicit ImageViewer(QWidget* parent = nullptr);

    ~ImageViewer() = default;

  public:
    /**
     * @brief Set the image
     * @param image Image
     */
    void setImage(const QPixmap& image);

    /**
     * @brief Get the image
     * @return Image
     */
    inline const QPixmap& image() const { return image_; }

  protected:
    /**
     * @brief Override function to repaint image to the widget
     * @param event Paint event
     */
    void paintEvent(QPaintEvent* event) override;

  private:
    QPixmap image_;  // image
};
