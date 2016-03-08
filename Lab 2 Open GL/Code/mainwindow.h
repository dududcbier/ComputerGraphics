#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "openglwindow.h"

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QKeyEvent>
#include <QMouseEvent>

#include <QTimer>

// Math includes
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix3x3>
#include <QMatrix4x4>


#include "objmodel.h"

/* MainWindow is the window used to display the application
 * Do all your changes to the code in this class (and the mainwindow.cpp file)
 */
class MainWindow : public OpenGLWindow
{

public:
    MainWindow();
    ~MainWindow();
    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

protected:
    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent * ev);
    void keyReleaseEvent(QKeyEvent * ev);

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent * ev);
    void mouseMoveEvent(QMouseEvent * ev);
    void mousePressEvent(QMouseEvent * ev);
    void mouseReleaseEvent(QMouseEvent * ev);
    void wheelEvent(QWheelEvent * ev);

private:
    void renderSphere(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos);
    void renderRaytracerScene();
    int nVertices;
    QMatrix4x4 model, view, projection;
    int lastX, lastY; // Mouse position
    unsigned int t;
    bool rotate;

    // Declare your VAO and VBO variables here
	QOpenGLVertexArrayObject object;
	QOpenGLBuffer *coordinates;
	QOpenGLBuffer *colors_buffer;

    // Members for the shader and uniform variables
    QOpenGLShaderProgram *m_shaderProgram;

    // Declare other private variables here if you need them
    QOpenGLBuffer *texture_buffer;
    QOpenGLTexture *texture;

    // Animation functions
    void startAnimatedScene();
    void renderPlanet(float centerDistance, float speedArroundSelf, float speedAroundCenter, QVector3D originalPos, float size);

public slots:
    void renderAnimatedScene();

};

#endif // MAINWINDOW_H
