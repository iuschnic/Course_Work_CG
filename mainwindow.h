#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QFileDialog>
#include "Facade.h"
#include "DrawerSolution.h"
#include "Commands.h"
#include "Exceptions.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void setup_scene();
    void update_scene();

private slots:
    void on_load_scene_btn_clicked();

    void on_start_simulation_clicked();

    void simulate();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<QGraphicsScene> _scene;
    std::shared_ptr<BaseDrawer> _drawer;
    std::shared_ptr<Facade> _facade;
};
#endif // MAINWINDOW_H
