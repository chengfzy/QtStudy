#include "MainWindow.h"
#include <DockAreaWidget.h>
#include <QtWidgets>

using namespace ads;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) { setupUI(); }

void MainWindow::setupUI() {
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

    // view menu bar
    auto viewMenu = menuBar()->addMenu("View");

    // Add the toggleViewAction of the dock widget to the menu to give the user the possibility to show the dock widget
    // if it has been closed
    viewMenu->addAction(dockWidget->toggleViewAction());

    // Add the dock widget to the top dock widget area
    dockManager_->addDockWidget(ads::TopDockWidgetArea, dockWidget);
}
