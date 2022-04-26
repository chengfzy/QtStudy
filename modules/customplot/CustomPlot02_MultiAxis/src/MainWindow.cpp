#include "MainWindow.h"

using namespace std;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // create custom plot
    plot_ = new QCustomPlot(this);

    // add data
    setupPlot();

    // center widget
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(plot_);
    auto centerWidget = new QWidget;
    centerWidget->setLayout(mainLayout);
    setCentralWidget(centerWidget);

    plot_->replot();
}

void MainWindow::setupPlot() {
    // make some sine ware for data
    constexpr size_t kSize{250};
    QVector<double> x0(kSize), x1(kSize), y0(kSize), y1(kSize);
    int offset = plot_->graphCount();
    for (int i = 0; i < kSize; ++i) {
        x0[i] = i;
        x1[i] = 2 * i;
        y0[i] = exp(-i / 150.) * cos(i / 10.);
        y1[i] = exp(-i / 150.f);
    }

    plot_->yAxis->setTickLabels(false);
    plot_->xAxis2->setVisible(true);
    plot_->axisRect()->addAxis(QCPAxis::atBottom);
    auto xAxis0 = plot_->axisRect()->axis(QCPAxis::atBottom, 0);
    auto xAxis1 = plot_->axisRect()->axis(QCPAxis::atBottom, 1);
    xAxis1->setLabelColor(Qt::blue);
    xAxis1->setTickLabelColor(Qt::blue);
    xAxis0->setPadding(10);  // add some padding, see
    // xAxis0;
    // set ticks
    plot_->axisRect()->setupFullAxesBox();
    // set axes label
    xAxis0->setLabel("X0");
    xAxis1->setLabel("X1");
    plot_->yAxis->setLabel("Y");
    // sync xAxis - xAxis2, yAxis - yAxis2
    connect(plot_->xAxis, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged), this,
            [&](const QCPRange& t) { plot_->xAxis2->setRange(t); });
    connect(plot_->yAxis, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged), this,
            [&](const QCPRange& t) { plot_->yAxis2->setRange(t); });
    // sync xAxis0 - xAxis1
    connect(xAxis0, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged), this,
            [&](const QCPRange& t) {
                vector<double> x0Range{0, 250};  // should obtain from data
                vector<double> x1Range{0, 2 * 250};
                double ka = (t.lower - x0Range[0]) / (x0Range[1] - x0Range[0]);
                double kb = (t.upper - x0Range[0]) / (x0Range[1] - x0Range[0]);
                double x1a = x1Range[0] + ka * (x1Range[1] - x1Range[0]);
                double x1b = x1Range[0] + kb * (x1Range[1] - x1Range[0]);
                plot_->axisRect()->axis(QCPAxis::atBottom, 1)->setRange(x1a, x1b);
            });

    // graph 01, x0 - y
    auto graph = plot_->addGraph(xAxis0, plot_->yAxis);
    QPen pen(Qt::blue);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    graph->setPen(pen);
    graph->setBrush(QBrush(QColor(0, 0, 255, 20)));
    graph->setData(x0, y0);
    graph->setName("Data 01");

    // graph 02, x1 - y
    graph = plot_->addGraph(xAxis1, plot_->yAxis);
    graph->setPen(QPen(Qt::red));
    graph->setLineStyle(QCPGraph::LineStyle::lsNone);  // only scatter
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    graph->setSelectable(QCP::stSingleData);  // set data selectable
    graph->selectionDecorator()->setPen(QPen(Qt::magenta));
    auto selectionScatterStyle = graph->selectionDecorator()->scatterStyle();
    selectionScatterStyle.setSize(10);
    graph->selectionDecorator()->setScatterStyle(selectionScatterStyle);
    graph->setData(x1, y1);
    graph->setName("Data 02");
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

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area
    plot_->graph(0)->rescaleAxes();
    plot_->graph(1)->rescaleAxes(true);

    // add legend
    plot_->legend->setVisible(true);

    // add title
    auto title = new QCPTextElement(plot_);
    title->setText("CustomPlot Drawing - Multi Axis");
    auto font = title->font();
    font.setBold(true);
    title->setFont(font);
    plot_->plotLayout()->insertRow(0);
    plot_->plotLayout()->addElement(0, 0, title);

    // interaction
    plot_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iMultiSelect |
                           QCP::iSelectLegend | QCP::iSelectAxes);
}
