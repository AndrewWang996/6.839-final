/*********************************************************************************
*  Copyright(C) 2016: Marco Livesu                                               *
*  All rights reserved.                                                          *
*                                                                                *
*  This file is part of CinoLib                                                  *
*                                                                                *
*  CinoLib is dual-licensed:                                                     *
*                                                                                *
*   - For non-commercial use you can redistribute it and/or modify it under the  *
*     terms of the GNU General Public License as published by the Free Software  *
*     Foundation; either version 3 of the License, or (at your option) any later *
*     version.                                                                   *
*                                                                                *
*   - If you wish to use it as part of a commercial software, a proper agreement *
*     with the Author(s) must be reached, based on a proper licensing contract.  *
*                                                                                *
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE       *
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.     *
*                                                                                *
*  Author(s):                                                                    *
*                                                                                *
*     Marco Livesu (marco.livesu@gmail.com)                                      *
*     http://pers.ge.imati.cnr.it/livesu/                                        *
*                                                                                *
*     Italian National Research Council (CNR)                                    *
*     Institute for Applied Mathematics and Information Technologies (IMATI)     *
*     Via de Marini, 6                                                           *
*     16149 Genoa,                                                               *
*     Italy                                                                      *
**********************************************************************************/
#ifndef CINO_GLCANVAS_H
#define CINO_GLCANVAS_H

#ifdef CINOLIB_USES_OPENGL
#ifdef CINOLIB_USES_QT

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

#include <vector>
#include <string>
#include <functional>
#include <QGLWidget>
#include <QColor>
#include <QMenu>
#include <cinolib/drawable_object.h>
#include <cinolib/bbox.h>
#include <cinolib/color.h>
#include <cinolib/geometry/vec2.h>
#include <cinolib/geometry/vec3.h>
#include <cinolib/symbols.h>

namespace cinolib
{

typedef struct
{
    vec2i       p2d = vec2i(-1,-1);       // pos 2D
    vec3d       p3d;                      // if p2d points offscreen (i.e. p.x<0) a projection of p3d will be used instead
    uint        disk_size = 5;            // disk radius (in pixels). set zero if don't want a disk to appear
    uint        font_size = 10;           // font size;
    std::string label     = "";           // text to render. empty string if don't want any text to appear
    Color       color     = Color::RED(); // color, for both text and disk
}
Marker;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

typedef struct
{
    bool     mouse_pressed = false;        // true if mouse is pressed
    double   radius        = 0.5;          // trackball radius
    vec3d    pivot         = vec3d(0,0,0); // trackball rotation origin
    QPoint   last_click_2d;                // window coords
    vec3d    last_click_3d;                // world  coords
    GLdouble projection[16];               // openGL projection matrix
    GLdouble modelview[16];                // openGL modelview  matrix
    float    z_near        = 0.1;          // front clipping plane
    float    z_far         = 1.0;          // back  clipping plane
    vec3d    scene_center  = vec3d(0,0,0); // scene_center
    float    scene_size    = 1.0;          // scene_size
    bool     mode_2d       = false;        // disable rotations
}
Trackball;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

class GLcanvas : public QGLWidget
{
    public:

        explicit GLcanvas(QWidget * parent = 0);
        ~GLcanvas();

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void updateGL();
        void updateGL_strict();

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void set_2d_mode(const bool b);

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void animation_start(const int period_ms = 30);
        void animation_stop();
        void animation_orbit(const vec3d & axis = vec3d(0,1,0), const uint step = 2); // step is expressed in degrees

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        int height_retina() const;
        int width_retina()  const;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void draw_helper(QPainter & painter);
        void draw_axis();

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void set_clear_color(const QColor & c);
        void fit_scene();

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        bool read_Z_buffer(const vec2i & p2d, GLfloat & depth);
        bool depth_test   (const vec3d & p3d);
        bool project      (const vec3d & p3d, vec2i & p2d, GLdouble & depth);
        bool unproject    (const vec2i & p2d, vec3d & p3d);

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void push_obj(const DrawableObject * obj, bool refit_scene = true);
        bool pop(const DrawableObject * obj);
        bool pop_first_occurrence_of(int type);
        bool pop_all_occurrences_of (int type);

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void push_marker(const Marker * m);
        void push_marker(const vec3d & p, const std::string & label, const Color & c = Color::RED(), const uint font_size = 10, const uint disk_size = 5);
        void push_marker(const vec2i & p, const std::string & label, const Color & c = Color::RED(), const uint font_size = 10, const uint disk_size = 5);
        void pop_marker();
        void pop_all_markers();

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        std::string serialize_POV() const;
        void        deserialize_POV(const std::string & s);

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void keyPressEvent        (QKeyEvent   *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
        void mousePressEvent      (QMouseEvent *event);
        void mouseReleaseEvent    (QMouseEvent *event);
        void mouseMoveEvent       (QMouseEvent *event);
        void wheelEvent           (QWheelEvent *event);
        void timerEvent           (QTimerEvent *event);

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        // these callbacks can be used to execute external code when mouse and keyboard
        // events occurr (e.g. for picking, drawing). If defined, they will be called
        // **before** the internal event handlers
        std::function<void(GLcanvas *canvas, QKeyEvent   *event)> callback_key_press;
        std::function<void(GLcanvas *canvas, QMouseEvent *event)> callback_mouse_double_click;
        std::function<void(GLcanvas *canvas, QMouseEvent *event)> callback_mouse_press;
        std::function<void(GLcanvas *canvas, QMouseEvent *event)> callback_mouse_release;
        std::function<void(GLcanvas *canvas, QMouseEvent *event)> callback_mouse_move;
        std::function<void(GLcanvas *canvas, QWheelEvent *event)> callback_wheel;
        std::function<void(GLcanvas *canvas, QTimerEvent *event)> callback_timer;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    protected:

        void make_popup_menu();

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void draw_marker(QPainter & painter, const Marker & t);
        void draw_text  (QPainter & painter, const vec2i & pos, const std::string & text, const uint font_size = 9, const Color & c = Color::BLACK());
        void draw_disk  (QPainter & painter, const vec2i & center, const uint radius, const Color & c = Color::BLACK());

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void rotate(const vec3d & axis, const double angle);
        void translate(const vec3d & trans);
        void zoom(double d);

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void reset_trackball();
        void set_scene_center(const vec3d & new_center, const double dist_from_camera, const bool pivot_at_center);
        void set_rotation_pivot(const vec3d & new_pivot);
        void update_projection_matrix(void);
        void map_to_sphere(const QPoint & p2d, vec3d & p3d) const;

    private:

        int       timer_id;
        QColor    clear_color;
        Trackball trackball;
        QMenu    *popup;
        bool      show_helper;
        bool      show_axis;
        bool      show_pivot;

        std::vector<const DrawableObject*> objects;
        std::vector<const Marker*>         markers;
};

}

#ifndef  CINO_STATIC_LIB
#include "glcanvas.cpp"
#endif

#endif // #ifdef CINOLIB_USES_QT
#endif // #ifdef CINOLIB_USES_OPENGL

#endif // CINO_GLCANVAS_H
