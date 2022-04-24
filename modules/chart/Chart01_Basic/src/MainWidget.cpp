#include "MainWidget.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
    // create chart view with the chart
    chart_ = new QChart();
    chartView_ = new QChartView(chart_, this);

    // create layout for grid and detached legend
    auto mainLayout = new QHBoxLayout();
    mainLayout->addWidget(chartView_);
    setLayout(mainLayout);

    // add series
    addSeries();
    addSeries();
    addSeries();
    addSeries();

    connectMarkers();

    // set title and show legend
    chart_->setTitle("Legendmarker Example");
    chart_->legend()->setVisible(true);
    chart_->legend()->setAlignment(Qt::AlignBottom);

    chartView_->setRenderHint(QPainter::Antialiasing);
}

void MainWidget::addSeries() {
    auto s = new QLineSeries;
    series_.append(s);
    s->setName(QString("Line %1").arg(series_.size()));

    // make some sine ware for data
    QList<QPointF> data;
    int offset = chart_->series().count();
    for (size_t i = 0; i < 360; i += 5) {
        float x = offset * 20 + i;
        data.append(QPointF(i, sin(qDegreesToRadians(x))));
    }
    s->append(data);

    chart_->addSeries(s);

    if (series_.size() == 1) {
        // set line color
        s->setColor(Qt::red);

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
        // set marker to orange circle
        float imageSize{20};
        QImage marker(imageSize, imageSize, QImage::Format_ARGB32);
        marker.fill(QColor(0, 0, 0, 0));
        QPainter painter;
        painter.begin(&marker);
        painter.setBrush(QColor(255, 127, 80));
        QPen pen = painter.pen();
        pen.setWidth(0);
        painter.setPen(pen);
        painter.drawEllipse(0, 0, imageSize * 0.9, imageSize * 0.9);
        painter.end();
        s->setLightMarker(marker);
        s->setMarkerSize(10);
#endif

        chart_->createDefaultAxes();
    }
}

void MainWidget::removeSeries() {
    // remove last series from chart
    if (series_.size() > 0) {
        auto s = series_.last();
        chart_->removeSeries(s);
        series_.removeLast();
        delete s;
    }
}

void MainWidget::connectMarkers() {
    auto markers = chart_->legend()->markers();
    for (auto& m : markers) {
        // disconnect possible existing conecting to avoid multiple connections
        QObject::disconnect(m, &QLegendMarker::clicked, this, &MainWidget::handleMarkerClicked);
        QObject::connect(m, &QLegendMarker::clicked, this, &MainWidget::handleMarkerClicked);
    }
}

void MainWidget::disconnectMarkers() {
    auto markers = chart_->legend()->markers();
    for (auto& m : markers) {
        QObject::disconnect(m, &QLegendMarker::clicked, this, &MainWidget::handleMarkerClicked);
    }
}

void MainWidget::handleMarkerClicked() {
    auto marker = qobject_cast<QLegendMarker*>(sender());
    Q_ASSERT(marker);

    switch (marker->type()) {
        case QLegendMarker::LegendMarkerTypeXY: {
            // toggle visibility of series
            marker->series()->setVisible(!marker->series()->isVisible());
            // turn legend marker back to visible, since hinding series also hides the marker and we don't want it to
            // happend now
            marker->setVisible(true);

            // Dim the marker, if series if not visible
            float alpha{1.0};
            if (!marker->series()->isVisible()) {
                alpha = 0.5;
            }
            // dim marker
            auto brush = marker->labelBrush();
            auto color = brush.color();
            color.setAlphaF(alpha);
            brush.setColor(color);
            marker->setLabelBrush(brush);
            // dim text
            auto pen = marker->pen();
            color = pen.color();
            color.setAlphaF(alpha);
            pen.setColor(color);
            marker->setPen(pen);
            break;
        }

        default:
            qDebug() << tr("Unknown marker type %1").arg(marker->type());
            break;
    }
}