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

    auto cont = ui->graphicsView->contentsRect();
    auto id = std::make_shared<std::size_t>(0);
    Point location_cam(-cont.width() / 2.0, -cont.height() / 2.0, -500);
    Point location_light(-300, 0, 0);
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(simulate()));
    AddCameraCommand add_cam_cmd (location_cam, id);
    _facade->execute(add_cam_cmd);
    SetMainCameraCommand set_cam(*id);
    _facade->execute(set_cam);

    AddLightCommand add_light_cmd(location_light, id);
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
            std::cout << "added" << std::endl;
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
    std::cout << "added objs" << std::endl;

    //_models.push_back(*id);
    //update_scene();
    ClearGraphicsSceneCommand clear_graphics_cmd(_drawer);
    _facade->execute(clear_graphics_cmd);

    DrawSceneCommand draw_cmd(_drawer);
    std::cout << "created draw_cmd\n";
    _facade->execute(draw_cmd);
    std::cout << "after draw_cmd\n";
    //auto filename = QFileInfo(path.toUtf8().data()).fileName();
    //QString figure = QString("Model ") + QString::number(*id);
    //ui->models_combobox->addItem(figure);
    //ui->models_combobox->setCurrentIndex(ui->models_combobox->count() - 1);
}


void MainWindow::on_start_simulation_clicked()
{
    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(simulate()));
    _timer->start(1000 / 20);

    /*StartSimulationCommand start_cmd(_drawer);
    _facade->execute(start_cmd);*/
}

void MainWindow::simulate()
{
    //StartSimulationCommand start_cmd(_drawer);
    //_facade->execute(start_cmd);
    //clock_t start = clock();
    SimIterationCommand sim(_drawer);
    _facade->execute(sim);
    //printf("Time: %.6fs\n", (double) (clock() - start) / CLOCKS_PER_SEC);
}


void MainWindow::on_stop_simulation_clicked()
{
    _timer->stop();
}

