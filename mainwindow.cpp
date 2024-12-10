#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qtimer.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setup_scene();

    _facade = std::make_shared<Facade>(Facade());

    auto id = std::make_shared<std::size_t>(0);
    //Point location_cam(0, 0, 400);
    //Point direction_cam(0, 0, -1);
    Point location_cam(400, 0, 400);
    Point direction_cam(-1, 0, -1);
    Point up_cam(0, 1, 0);
    Point location_light(400, 0, 0);
    Point direction_light(-1, 0, 0);
    Point up_light(0, 1, 0);
    //Point location_light(0, 0, -500);
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(simulate()));
    AddCameraCommand add_cam_cmd (location_cam, direction_cam, up_cam, id);
    _facade->execute(add_cam_cmd);
    SetMainCameraCommand set_cam(*id);
    _facade->execute(set_cam);

    AddLightCommand add_light_cmd(location_light, direction_light, up_light, id);
    _facade->execute(add_light_cmd);
    SetMainLightCommand set_light(*id);
    _facade->execute(set_light);
}

MainWindow::~MainWindow()
{
    delete _timer;
    delete ui;
}

void MainWindow::update_scene()
{
    ClearGraphicsSceneCommand clear_cmd(_drawer);
    _facade->execute(clear_cmd);

    DrawSceneCommand draw_cmd(_drawer);
    _facade->execute(draw_cmd);
}

void MainWindow::setup_scene()
{
    _scene = std::make_shared<QGraphicsScene>(this);
    ui->graphicsView->setScene(_scene.get());
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    auto cont = ui->graphicsView->contentsRect();
    _scene->setSceneRect(0, 0, cont.width(), cont.height());
    auto solution(new DrawerSolution<QtFactory, QGraphicsScene>());
    _drawer = solution->create_drawer(_scene);
    delete(solution);
}

void MainWindow::on_load_scene_btn_clicked()
{
    auto path = QFileDialog::getOpenFileName(nullptr, "Загружаем сцену...", "../OOP_lab_03_final/points_links_cube.txt");
    if (path.isNull())
        return;

    std::vector<std::shared_ptr<VisibleObject>> objs;
    auto id = std::make_shared<size_t>(0);
    std::string file_name = path.toStdString();

    ClearSceneCommand clear_cmd;
    _facade->execute(clear_cmd);

    try
    {
        FileLoadModelCommand load_cmd(objs, file_name);
        _facade->execute(load_cmd);
        for (auto &obj: objs)
        {
            AddObjectCommand add_cmd(obj, id);
            _facade->execute(add_cmd);
            //_models.push_back(*id);
        }
    }
    catch (const BaseException &err)
    {
        QMessageBox::critical(nullptr, "Ошибка!", "Ошибка загрузки из файла!");
        return;
    }

    //_models.push_back(*id);
    //update_scene();
    ClearGraphicsSceneCommand clear_graphics_cmd(_drawer);
    _facade->execute(clear_graphics_cmd);

    DrawSceneCommand draw_cmd(_drawer);
    _facade->execute(draw_cmd);
    //auto filename = QFileInfo(path.toUtf8().data()).fileName();
    //QString figure = QString("Model ") + QString::number(*id);
    //ui->models_combobox->addItem(figure);
    //ui->models_combobox->setCurrentIndex(ui->models_combobox->count() - 1);
}


void MainWindow::on_start_simulation_clicked()
{
    _timer->start(1000 / 25);
}

void MainWindow::simulate()
{
    clock_t start = clock();
    SimIterationCommand sim(_drawer);
    _facade->execute(sim);
    //printf("Time: %.6fs\n", (double) (clock() - start) / CLOCKS_PER_SEC);
}


void MainWindow::on_stop_simulation_clicked()
{
    _timer->stop();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    direction dir = none;
    if (key == Qt::Key_W)
        dir = m_forward;
    else if (key == Qt::Key_S)
        dir = m_backward;
    else if (key == Qt::Key_D)
        dir = m_right;
    else if (key == Qt::Key_A)
        dir = m_left;
    else if (key == Qt::Key_E)
        dir = r_right;
    else if (key == Qt::Key_Q)
        dir = r_left;
    else if (key == Qt::Key_Z)
        dir = m_up;
    else if (key == Qt::Key_X)
        dir = m_down;
    if (dir != none)
    {
        MoveRotateCameraCommand cmd(dir);
        _facade->execute(cmd);
        update_scene();
    }
}

