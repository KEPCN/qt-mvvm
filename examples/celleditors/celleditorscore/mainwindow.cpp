// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mainwindow.h"
#include "modeleditorwidget.h"
#include "samplemodel.h"
#include <QCoreApplication>
#include <QSettings>

namespace {
const QString main_window_group = "MainWindow";
const QString size_key = "size";
const QString pos_key = "pos";
} // namespace

namespace CellEditors {

MainWindow::MainWindow() : m_model(std::make_unique<SampleModel>())
{
    setCentralWidget(new ModelEditorWidget(m_model.get()));
    initApplication();
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    QMainWindow::closeEvent(event);
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup(main_window_group);
    settings.setValue(size_key, size());
    settings.setValue(pos_key, pos());
    settings.endGroup();
}

void MainWindow::initApplication()
{
    QCoreApplication::setApplicationName("celleditors");
    QCoreApplication::setApplicationVersion("0.1");
    QCoreApplication::setOrganizationName("qt-mvvm");

    QSettings settings;
    if (settings.childGroups().contains(main_window_group)) {
        settings.beginGroup(main_window_group);
        resize(settings.value(size_key, QSize(400, 400)).toSize());
        move(settings.value(pos_key, QPoint(200, 200)).toPoint());
        settings.endGroup();
    }

    populateModel();
}

//! Populating the model with the content.

void MainWindow::populateModel()
{
    m_model->insertItem<DemoPropertiesItem>();
    m_model->insertItem<DemoPropertiesItem>();
    m_model->insertItem<DemoPropertiesItem>();
}

} // namespace CellEditors
