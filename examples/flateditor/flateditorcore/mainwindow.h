// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef FLATEDITORCORE_MAINWINDOW_H
#define FLATEDITORCORE_MAINWINDOW_H

#include <QMainWindow>
#include <memory>

class SampleModel;

//! The main window of this application.

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event);

private:
    void initApplication();
    void writeSettings();

    std::unique_ptr<SampleModel> m_sampleModel;
};

#endif // FLATEDITORCORE_MAINWINDOW_H
