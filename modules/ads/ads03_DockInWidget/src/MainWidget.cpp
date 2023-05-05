#include "MainWidget.h"
#include <DockAreaWidget.h>
#include <QtWidgets>

using namespace ads;

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) { setupUI(); }

void MainWidget::setupUI() {
    // dock manager
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    CDockManager::setAutoHideConfigFlags(CDockManager::DefaultAutoHideConfig);
    dockManager_ = new CDockManager(this);

    // text edit
    auto textEdit = new QPlainTextEdit();
    textEdit->setPlaceholderText("Simple text");

    // Create a dock widget with the title Label 1 and set the created label as the dock widget content
    auto dockWidget = new CDockWidget("Doc 1");
    dockWidget->setWidget(textEdit);

    // Add the dock widget to the top dock widget area
    dockManager_->addDockWidget(ads::TopDockWidgetArea, dockWidget);

    // set main layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(dockWidget);
    setLayout(layout);
}
