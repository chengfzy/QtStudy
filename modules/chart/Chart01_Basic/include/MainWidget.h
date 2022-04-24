#pragma once
#include <QtCharts>
#include <QtWidgets>

class MainWidget : public QWidget {
  public:
    explicit MainWidget(QWidget* parent = nullptr);

  public slots:
    void addSeries();
    void removeSeries();
    void connectMarkers();
    void disconnectMarkers();

    void detachLegend();
    void handleMarkerClicked();

  private:
    QChart* chart_;
    QList<QLineSeries*> series_;
    QChartView* chartView_;
};
