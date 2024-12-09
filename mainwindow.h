#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QKeyEvent>
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
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_load_scene_btn_clicked();

    void on_start_simulation_clicked();

    void simulate();

    void on_stop_simulation_clicked();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<QGraphicsScene> _scene;
    std::shared_ptr<BaseDrawer> _drawer;
    std::shared_ptr<Facade> _facade;
    QTimer *_timer;
};
#endif // MAINWINDOW_H
