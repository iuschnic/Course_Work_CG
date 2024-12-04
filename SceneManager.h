#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "DrawAdapter.h"
#include <vector>
#include <iostream>

#define Ia 0.1
#define ka 1
#define Il 1
#define kd 0.5
#define ks 0.4
#define ndeg 5
#define K 1

class SceneManager
{
public:
    SceneManager()
    {
        _camera = nullptr;
        _light = nullptr;
        _scene = std::make_shared<Scene>(Scene());
        _adapter = std::make_shared<DrawAdapter>(DrawAdapter());
        _sim_flag = 0;
    }
    ~SceneManager() = default;

    void set_main_camera(const std::size_t cam_id)
    {
        std::shared_ptr<InvisibleObject> cam = _scene->get_camera(cam_id);
        auto cam_pnt = std::dynamic_pointer_cast<PProjCamera>(cam);
        _camera = cam_pnt;
        //_scene->set_cam_index(cam_id);
    }
    std::shared_ptr<PProjCamera> get_main_camera() {return _camera;}
    void set_main_light(const std::size_t light_id)
    {
        std::shared_ptr<InvisibleObject> light = _scene->get_light(light_id);
        auto light_pnt = std::dynamic_pointer_cast<PointLight>(light);
        _light = light_pnt;
    }
    std::shared_ptr<PointLight> get_main_light() {return _light;}
    std::size_t add_object(std::shared_ptr<VisibleObject> &object)
    {
        return _scene->add_object(object);
    }
    std::size_t add_camera(const Point &center, const Point &direction, const Point &up)
    {
        return _scene->add_camera(center, direction, up);
    }
    std::size_t add_light(const Point &center, const Point &direction, const Point &up)
    {
        return _scene->add_light(center, direction, up);
    }
    void delete_object(const std::size_t &id_object)
    {
        _scene->delete_object(id_object);
    }
    void delete_camera(const std::size_t &id_camera)
    {
        if (_camera->get_id() == id_camera)
            _camera = nullptr;
        _scene->delete_camera(id_camera);
    }
    void delete_light(const std::size_t &id_light)
    {
        _scene->delete_light(id_light);
    }

    void sim_iteration(std::shared_ptr<BaseDrawer> drawer)
    {
        //std::map<std::size_t, std::vector<std::vector<double>>> intensities = _scene->calc_intensities(_light);
        _adapter->set_drawer(drawer);
        _adapter->set_camera(_camera);
        _adapter->clear();
        _scene->sim_iteration();
        std::map<std::size_t, std::vector<std::vector<double>>> intensities = calc_intensities(_light, _camera);
        std::vector<std::vector<double>> shade_buf = shade_z_buf(drawer->get_width(), drawer->get_height());
        _adapter->set_to_light(_light->get_look_at());
        _adapter->set_shade_buf(shade_buf);
        draw(intensities);
        //draw();
    }

    void draw_scene(std::shared_ptr<BaseDrawer> drawer)
    {
        //std::cout << "in draw_scene method\n";
        std::map<std::size_t, std::vector<std::vector<double>>> intensities = calc_intensities(_light, _camera);
        std::vector<std::vector<double>> shade_buf = shade_z_buf(drawer->get_width(), drawer->get_height());
        _adapter->set_shade_buf(shade_buf);
        _adapter->set_drawer(drawer);
        _adapter->set_camera(_camera);
        _adapter->set_to_light(_light->get_look_at());
        draw(intensities);
        //draw();
    }

    void clear_graphics_scene(std::shared_ptr<BaseDrawer> drawer)
    {
        //_scene->_visible_objects.clear();
        //auto drawer = _adapter->get_drawer();
        //std::cout << "clearing graphics scene\n";
        drawer->clear();
    }

    void clear_scene()
    {
        _scene->_visible_objects.clear();
    }


private:
    std::shared_ptr<PProjCamera> _camera;
    std::shared_ptr<PointLight> _light;
    std::shared_ptr<Scene> _scene;
    std::shared_ptr<DrawAdapter> _adapter;
    Matrix _to_light;
    int _sim_flag;

    void draw(std::map<std::size_t, std::vector<std::vector<double>>> &intensities)
    {
        //std::cout << "in draw method\n";
        for (const auto &kv : _scene->_visible_objects)
        {
            auto i = intensities[kv.first];
            auto obj_ptr = std::dynamic_pointer_cast<Object>(kv.second);
            //std::cout << obj_ptr->get_mass() << std::endl;
            _adapter->set_adaptee(obj_ptr);
            _adapter->set_adaptee_intensities(i);
            //std::cout << i << std::endl;
            _adapter->request();
        }
        _adapter->draw();
    }

    std::vector<std::vector<double>> shade_z_buf(size_t width, size_t height)
    {
        std::vector<std::vector<double>> shade_buf;
        for (size_t i = 0; i < height; i++)
        {
            shade_buf.push_back({});
            for (size_t j = 0; j < width; j++)
            {
                shade_buf[i].push_back(std::numeric_limits<double>::max());
            }
        }
        for (const auto &[id, obj] : _scene->_visible_objects)
        {
            auto obj_ptr = std::dynamic_pointer_cast<Object>(obj);
            auto spheres = obj_ptr->get_spheres();
            for (const auto &sphere: spheres)
            {
                auto faces = sphere->get_faces();
                auto points = sphere->get_points();
                for (const auto &face : faces)
                {
                    //Возможно нужно исправить!!!!!!!!!!!!!!!!!!!!!!
                    Point s_center = sphere->get_center();
                    Point l_vec = _light->get_direction();
                    Point p = points[face[0]];
                    Point n_vec = p - s_center;
                    double scalar = l_vec.get_x() * n_vec.get_x() + l_vec.get_y() * n_vec.get_y() + l_vec.get_z() * n_vec.get_z();
                    if (scalar < 0)
                        continue;
                    if (face.size() == 3)
                    {
                        Point t0 = _light->get_projection(points[face[0]]);
                        Point t1 = _light->get_projection(points[face[1]]);
                        Point t2 = _light->get_projection(points[face[2]]);
                        triangle(shade_buf, t0, t1, t2, width, height);
                    }
                    else if (face.size() == 4)
                    {
                        Point t0 = _light->get_projection(points[face[1]]);
                        Point t1 = _light->get_projection(points[face[2]]);
                        Point t2 = _light->get_projection(points[face[3]]);
                        triangle(shade_buf, t0, t1, t2, width, height);
                        t0 = _light->get_projection(points[face[0]]);
                        t1 = _light->get_projection(points[face[1]]);
                        t2 = _light->get_projection(points[face[3]]);
                        triangle(shade_buf, t0, t1, t2, width, height);
                    }

                }
            }
        }
        return shade_buf;
    }

    void triangle(std::vector<std::vector<double>> &shade_buf, Point &p0, Point &p1, Point &p2, size_t &width, size_t &height) {
        Pixel t0 = Pixel(p0);
        Pixel t1 = Pixel(p1);
        Pixel t2 = Pixel(p2);
        t0.set_x(t0.get_x() + width / 2);
        t0.set_y(t0.get_y() + height / 2);
        t1.set_x(t1.get_x() + width / 2);
        t1.set_y(t1.get_y() + height / 2);
        t2.set_x(t2.get_x() + width / 2);
        t2.set_y(t2.get_y() + height / 2);
        if (t0.get_y() == t1.get_y() && t0.get_y() == t2.get_y())
        {
            if (t0.get_x() > t1.get_x()) std::swap(t0, t1);
            if (t0.get_x() > t2.get_x()) std::swap(t0, t2);
            if (t1.get_x() > t2.get_x()) std::swap(t1, t2);
            double dz1 = (t1.get_z() - t0.get_z()) / (t1.get_x() - t0.get_x() + 1);
            double dz2 = (t2.get_z() - t1.get_z()) / (t2.get_x() - t1.get_x() + 1);
            double z = t0.get_z();
            for (int j = t0.get_x(); j <= t1.get_x(); j++) {
                if (t0.get_y() >= shade_buf.size() || t0.get_y() < 0 || j < 0 || j >= shade_buf[0].size())
                    continue;
                if (shade_buf[t0.get_y()][j] > z) {
                    shade_buf[t0.get_y()][j] = z;
                }
                z += dz1;
            }
            for (int j = t1.get_x(); j <= t2.get_x(); j++) {
                if (t0.get_y() >= shade_buf.size() || t0.get_y() < 0 || j < 0 || j >= shade_buf[0].size())
                    continue;
                if (shade_buf[t0.get_y()][j] > z) {
                    shade_buf[t0.get_y()][j] = z;
                }
                z += dz2;
            }
            return;
        }
        if (t0.get_y() > t1.get_y()) std::swap(t0, t1);
        if (t0.get_y() > t2.get_y()) std::swap(t0, t2);
        if (t1.get_y() > t2.get_y()) std::swap(t1, t2);

        //Расчет инкрементов глубины и интенсивности при движении по оси y, двигаемся снизу вверх, от t0 к t1
        double dz_y_0_1 = (t1.get_z() - t0.get_z()) / (t1.get_y() - t0.get_y() + 1); //Между t0 и t1
        double dz_y_0_2 = (t2.get_z() - t0.get_z()) / (t2.get_y() - t0.get_y() + 1); //Между t0 и t2
        double z0 = t0.get_z(); //начальное значение z
        double z_0_1 = z0; //значение z на отрезке 0-1
        double z_0_2 = z0; //значение z на отрезке 0-2
        int total_height = t2.get_y() - t0.get_y();
        int segment_height = t1.get_y() - t0.get_y() + 1;
        for (int y = t0.get_y(); y <= t1.get_y(); y++) {
            double alpha = (double)(y - t0.get_y()) / total_height;
            double beta  = (double)(y - t0.get_y()) / segment_height; // be careful with divisions by zero
            Pixel p_0_2 = t0 + (t2 - t0) * alpha; //Точка на отрезке 0-2
            Pixel p_0_1 = t0 + (t1 - t0) * beta; //Точка на отрезке 0-1
            //double dz_0_2 = dz_y_0_2;
            //double dz_0_1 = dz_y_0_1;
            if (p_0_2.get_x() > p_0_1.get_x())
            {
                double z_cur = z_0_1;
                double dz_x = (z_0_2 - z_0_1) / (p_0_2.get_x() - p_0_1.get_x() + 1);
                for (int j = p_0_1.get_x(); j <= p_0_2.get_x(); j++) {
                    if (y >= shade_buf.size() || y < 0 || j < 0 || j >= shade_buf[0].size())
                        continue;
                    if (shade_buf[y][j] > z_cur) {
                        shade_buf[y][j] = z_cur;
                    }
                    z_cur += dz_x;
                }
            }
            else
            {
                double z_cur = z_0_2;
                double dz_x = (z_0_1 - z_0_2) / (p_0_1.get_x() - p_0_2.get_x() + 1);
                for (int j = p_0_2.get_x(); j <= p_0_1.get_x(); j++) {
                    if (y >= shade_buf.size() || y < 0 || j < 0 || j >= shade_buf[0].size())
                        continue;
                    if (shade_buf[y][j] > z_cur) {
                        shade_buf[y][j] = z_cur;
                    }
                    z_cur += dz_x;
                }
            }
            z_0_1 += dz_y_0_1;
            z_0_2 += dz_y_0_2;
        }

        double z_1_2 = t1.get_z(); //начальное значение z на отрезке 1-2
        double dz_y_1_2 = (t2.get_z() - t1.get_z()) / (t2.get_y() - t1.get_y() + 1); //Между t1 и t2
        //z_0_2 остается тем же
        segment_height = t2.get_y() - t1.get_y() + 1;
        for (int y = t1.get_y(); y <= t2.get_y(); y++) {
            double alpha = (double)(y - t0.get_y()) / total_height;
            double beta  = (double)(y - t1.get_y()) / segment_height; // be careful with divisions by zero
            Pixel p_0_2 = t0 + (t2 - t0) * alpha; //Точка на отрезке 2-0
            Pixel p_1_2 = t1 + (t2 - t1) * beta; //Точка на отрезке 2-1
            //double dz_0_2 = dz_y_0_2;
            //double dz_0_1 = dz_y_0_1;
            if (p_0_2.get_x() > p_1_2.get_x())
            {
                double z_cur = z_1_2;
                double dz_x = (z_0_2 - z_1_2) / (p_0_2.get_x() - p_1_2.get_x() + 1);
                for (int j = p_1_2.get_x(); j <= p_0_2.get_x(); j++) {
                    if (y >= shade_buf.size() || y < 0 || j < 0 || j >= shade_buf[0].size())
                        continue;
                    if (shade_buf[y][j] >= z_cur) {
                        shade_buf[y][j] = z_cur;
                    }
                    z_cur += dz_x;
                }
            }
            else
            {
                double z_cur = z_0_2;
                double dz_x = (z_1_2 - z_0_2) / (p_1_2.get_x() - p_0_2.get_x() + 1);
                for (int j = p_0_2.get_x(); j <= p_1_2.get_x(); j++) {
                    if (y >= shade_buf.size() || y < 0 || j < 0 || j >= shade_buf[0].size())
                        continue;
                    if (shade_buf[y][j] >= z_cur) {
                        shade_buf[y][j] = z_cur;
                    }
                    z_cur += dz_x;
                }
            }
            z_1_2 += dz_y_1_2;
            z_0_2 += dz_y_0_2;
        }
    }


    std::map<std::size_t, std::vector<std::vector<double>>> calc_intensities(const std::shared_ptr<PointLight> &light, const std::shared_ptr<PProjCamera> &camera)
    {
        std::map<std::size_t, std::vector<std::vector<double>>> intensities;
        auto l_center = light->get_center();
        auto c_center = camera->get_center();
        for (const auto &[id, obj]: _scene->_visible_objects)
        {
            intensities[id] = {};
            auto object = std::static_pointer_cast<Object>(obj);
            auto spheres = object->get_spheres();
            for (int i = 0; i < spheres.size(); i++)
            {
                auto s = spheres[i];
                auto s_center = s->get_center();
                auto points = s->get_points();
                std::vector<double> sphere_intensities;
                for (int i = 0; i < points.size(); i++)
                {
                    auto p = points[i];
                    //вектор нормали к сфере в данной точке
                    auto n_vec = p - s_center;
                    double n_mod = pow(pow(n_vec.get_x(), 2) + pow(n_vec.get_y(), 2) + pow(n_vec.get_z(), 2), 0.5);
                    //вектор от точки до источника света
                    auto l_vec = l_center - p;
                    double l_mod = pow(pow(l_vec.get_x(), 2) + pow(l_vec.get_y(), 2) + pow(l_vec.get_z(), 2), 0.5);
                    //вектор от точки до камеры
                    auto c_vec = c_center - p;
                    double c_mod = pow(pow(c_vec.get_x(), 2) + pow(c_vec.get_y(), 2) + pow(c_vec.get_z(), 2), 0.5);
                    //нормализованный вектор нормали
                    auto n_norm = n_vec;
                    n_norm.normalize();
                    //вектор падающего луча
                    auto f_vec = l_vec * (-1);
                    //вектор отражения
                    double scalar_f_n = f_vec.get_x() * n_norm.get_x() + f_vec.get_y() * n_norm.get_y() + f_vec.get_z() * n_norm.get_z();
                    auto r_vec = f_vec - n_norm * 2 * scalar_f_n;
                    double r_mod = pow(pow(r_vec.get_x(), 2) + pow(r_vec.get_y(), 2) + pow(r_vec.get_z(), 2), 0.5);
                    //double dist = pow(pow(p.get_x() - l_center.get_x(), 2) + pow(p.get_y() - l_center.get_y(), 2) + pow(p.get_z() - l_center.get_z(), 2), 0.5);
                    double cos_teta = (n_vec.get_x() * l_vec.get_x() + n_vec.get_y() * l_vec.get_y() + n_vec.get_z() * l_vec.get_z()) / (n_mod * l_mod);
                    double cos_alpha = (r_vec.get_x() * c_vec.get_x() + r_vec.get_y() * c_vec.get_y() + r_vec.get_z() * c_vec.get_z()) / (r_mod * c_mod);
                    if (cos_alpha < 0)
                        cos_alpha = 0;
                    if (cos_teta < 0)
                        cos_teta = 0;
                    double intensity = Ia * ka + Il * (kd * cos_teta + ks * pow(cos_alpha, ndeg));
                    sphere_intensities.push_back(intensity);
                    //std::cout << "I " << intensity << std::endl;
                }
                intensities[id].push_back(sphere_intensities);
            }
        }
        return intensities;
    }
};

#endif // SCENEMANAGER_H
