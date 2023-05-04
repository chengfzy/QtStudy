#include "MainWidget.h"
#include <DockAreaWidget.h>

using namespace ads;

MainWidget::MainWidget(QWidget* parent) : QMainWindow(parent), perspectiveManager_(new PerspectivesManager("persist")) {
    setupUI();
}

void MainWidget::setupUI() {
    // dock widget
    dockWidget_ = new DockInDockWidget(this, true, perspectiveManager_.get());
    dockWidget_->attachViewMenu(menuBar()->addMenu("View"));
    setCentralWidget(dockWidget_);

    CDockAreaWidget* previousDockWidget = nullptr;
    for (int i = 0; i != 3; ++i) {
        auto textEdit = new QPlainTextEdit();
        textEdit->setPlaceholderText("Simple text");
        previousDockWidget = dockWidget_->addTabWidget(textEdit, "Top label " + QString::number(i), previousDockWidget);
    }

    auto lastTopLevelDock = previousDockWidget;
    for (int j = 0; j != 2; ++j) {
        DockInDockWidget* groupManager = dockWidget_->createGroup("Group " + QString::number(j), lastTopLevelDock);

        previousDockWidget = nullptr;
        for (int i = 0; i != 3; ++i) {
            auto textEdit = new QPlainTextEdit();
            textEdit->setPlaceholderText("Simple text");
            previousDockWidget = groupManager->addTabWidget(
                textEdit, "ZInner " + QString::number(j) + "/" + QString::number(i), previousDockWidget);
        }

        // create sub-group
        auto subGroup = groupManager->createGroup("SubGroup " + QString::number(j), previousDockWidget);
        previousDockWidget = nullptr;
        for (int i = 0; i != 3; ++i) {
            auto textEdit = new QPlainTextEdit();
            textEdit->setPlaceholderText("Simple text");
            previousDockWidget = subGroup->addTabWidget(
                textEdit, "SubInner " + QString::number(j) + "/" + QString::number(i), previousDockWidget);
        }
    }

    perspectiveManager_->loadPerspectives();

    // // dock manager
    // CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    // CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    // CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    // CDockManager::setAutoHideConfigFlags(CDockManager::DefaultAutoHideConfig);
    // dockManager_ = new CDockManager(this);

    // // text edit
    // auto textEdit = new QPlainTextEdit();
    // textEdit->setPlaceholderText("Simple text");

    // // Create a dock widget with the title Label 1 and set the created label as the dock widget content
    // auto dockWidget = new CDockWidget("Doc 1");
    // dockWidget->setWidget(textEdit);

    // // Add the toggleViewAction of the dock widget to the menu to give the user the possibility to show the dock
    // widget
    // // if it has been closed
    // // auto viewMenu = menuBar()->addMenu("View");
    // // viewMenu->addAction(dockWidget->toggleViewAction());

    // // Add the dock widget to the top dock widget area
    // dockManager_->addDockWidget(TopDockWidgetArea, dockWidget);
}
