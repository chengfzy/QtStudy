#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // create custom plot
    plot_ = new QCustomPlot(this);

    // add data
    addSeries();

    // center widget
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(plot_);
    auto centerWidget = new QWidget;
    centerWidget->setLayout(mainLayout);
    setCentralWidget(centerWidget);

    plot_->replot();
}

void MainWindow::addSeries() {
    // make some sine ware for data
    constexpr size_t kSize{250};
    QVector<double> x(kSize), y0(kSize), y1(kSize);
    int offset = plot_->graphCount();
    for (int i = 0; i < kSize; ++i) {
        x[i] = i;
        y0[i] = exp(-i / 150.) * cos(i / 10.);
        y1[i] = exp(-i / 150.f);
    }

    // create graph and assign data
    auto graph = plot_->addGraph();
    QPen pen(Qt::blue);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    graph->setPen(pen);
    graph->setBrush(QBrush(QColor(0, 0, 255, 20)));
    graph->setData(x, y0);
    graph->setName("Data 01");
    // set data
    graph = plot_->addGraph();
    graph->setPen(QPen(Qt::red));
    graph->setLineStyle(QCPGraph::LineStyle::lsLine);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    graph->setData(x, y1);
    graph->setName("Data 02");
    // set data selectable
    graph->setSelectable(QCP::stSingleData);
    // print out selection data
    connect(graph, static_cast<void (QCPGraph::*)(const QCPDataSelection&)>(&QCPGraph::selectionChanged), this,
            [&](const QCPDataSelection& s) {
                if (s.dataRangeCount() > 0) {
                    statusBar()->showMessage(tr("Select Point: (%1, %2)")
                                                 .arg(plot_->graph(1)->data()->at(s.dataRange().begin())->key)
                                                 .arg(plot_->graph(1)->data()->at(s.dataRange().begin())->value));
                }
                // for (auto it = plot_->graph(1)->data()->at(s.dataRange().begin());
                //      it != plot_->graph(1)->data()->at(s.dataRange().end()); ++it) {
                //     qDebug() << QString("select data: (%1, %2)").arg(it->key).arg(it->value);
                // }
            });

    // set ticks
    plot_->axisRect()->setupFullAxesBox();
    // set axes label
    plot_->xAxis->setLabel("X");
    plot_->yAxis->setLabel("Y");
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area
    plot_->graph(0)->rescaleAxes();
    plot_->graph(1)->rescaleAxes(true);

    // add legend
    plot_->legend->setVisible(true);

    // add title
    auto title = new QCPTextElement(plot_);
    title->setText("CustomPlot Drawing - Basic");
    auto font = title->font();
    font.setBold(true);
    title->setFont(font);
    plot_->plotLayout()->insertRow(0);
    plot_->plotLayout()->addElement(0, 0, title);

    // interaction
    plot_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iMultiSelect |
                           QCP::iSelectLegend | QCP::iSelectAxes);
}
