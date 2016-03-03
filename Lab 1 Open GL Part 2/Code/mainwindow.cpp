#include "mainwindow.h"
#include <math.h>
#include <iostream>
#include <QDebug>
#include <QScreen>

MainWindow::MainWindow()
    : m_shaderProgram(0)
{
    qDebug() << "Constructor of MainWindow";

    // NOTE: OpenGL functions are not yet enabled when the constructor is called
}

MainWindow::~MainWindow()
{
    qDebug() << "Desctructor of MainWindow";

    // Freeing all your used resources here

    // Destroys buffers first, before destroying the VertexArrayObject
    object.destroy();

    // freeing the pointer of the shading program
    delete m_shaderProgram;
    delete normal_buffer;
    delete coordinates;

}

// Initialize all your OpenGL objects here
void MainWindow::initialize()
{
    rotate = false;
    qDebug() << "MainWindow::initialize()";
    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << "Using OpenGL" << qPrintable(glVersion);

    // Initialize the shaders
    m_shaderProgram = new QOpenGLShaderProgram(this);
    // Use the ":" to load from the resources files (i.e. from the resources.qrc)
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.glsl");
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.glsl");
    m_shaderProgram->link();

    // Shaders are initialized
    // Initialize objects and buffers

    OBJModel sphere = OBJModel(":/models/sphere.obj");
    QVector<QVector3D> vertices = sphere.vertices;

    nVertices = vertices.length();

    QVector<QVector3D> normals = sphere.normals;

    QVector<QVector3D> colors;
    for (int i = 0; i < sphere.indices.length(); i += 3){
        QVector3D c = QVector3D((double) rand() / RAND_MAX, (double) rand() / RAND_MAX, (double) rand() / RAND_MAX);
        colors.append(c);
        colors.append(c);
        colors.append(c);
    }

    // Create your Vertex Array Object (VAO) and Vertex Buffer Objects (VBO) here.
    object.create();
    object.bind();
    
    coordinates = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    coordinates->create();
    coordinates->bind();

    coordinates->allocate(vertices.data(), vertices.length() * sizeof(QVector3D));
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	
    normal_buffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    normal_buffer->create();
    normal_buffer->bind();
    
    normal_buffer->allocate(normals.data(), normals.length() * sizeof(QVector3D));
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);

    m_shaderProgram->setUniformValue("normal", normal);
    
   // for (int index = 0; index < sphere.indices.length(); index++)
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(2);
		
	object.release();

    // Set OpenGL to use Filled triangles (can be used to render points, or lines)
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    // Enable Z-buffering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    qDebug() << "Depth Buffer size:" <<  this->format().depthBufferSize() << "bits";

    // Set the clear color to be black (color used for resetting the screen)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);



}

void MainWindow::renderSphere(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos)
{
    model.setToIdentity();
    model.translate(pos);
    m_shaderProgram->setUniformValue("lightpos", lightpos);
    m_shaderProgram->setUniformValue("materialColor", color);

    //model.setColumn(3,QVector4D(pos,1));
    m_shaderProgram->setUniformValue("intensity", material);
    m_shaderProgram->setUniformValue("m", model);

    // OpenGL assignment 1, part 2: create a function to render the sphere
    // Use OBJModel(":/models/spehere.obj") for the model
    glDrawArrays(GL_TRIANGLES, 0, nVertices);
}

/**
 * Renders a similar scene used for the raytracer:
 * 5 colored spheres with a single light
 */
void MainWindow::renderRaytracerScene()
{
    QVector3D lightpos = QVector3D(-200,600,1500);

    // Blue sphere
    renderSphere(QVector3D(90,320,100),QVector3D(0,0,1),QVector4D(0.2f,0.7f,0.5f,64),lightpos);

    // Green sphere
    renderSphere(QVector3D(210,270,300),QVector3D(0,1,0),QVector4D(0.2f,0.3f,0.5f,8),lightpos);

    // Red sphere
    renderSphere(QVector3D(290,170,150),QVector3D(1,0,0),QVector4D(0.2f,0.7f,0.8f,32),lightpos);

    // Yellow sphere
    renderSphere(QVector3D(140,220,400),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos);

    // Orange sphere
    renderSphere(QVector3D(110,130,200),QVector3D(1,0.5f,0),QVector4D(0.2f,0.8f,0.5f,32),lightpos);
}

// The render function, called when an update is requested
void MainWindow::render()
{
    // glViewport is used for specifying the resolution to render
    // Uses the window size as the resolution
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    // Clear the screen at the start of the rendering.
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Bind the shaderprogram to use it
    m_shaderProgram->bind();
    object.bind();

   // m_shaderProgram->setUniformValue("m", model);


    view.setToIdentity();
    projection.setToIdentity();

    projection.perspective(30.0, 1, 50, 1000);
    //view.rotate(-200,-200,-200);
    view.translate(-200,-200,-1000);

    m_shaderProgram->setUniformValue("v", view);
    m_shaderProgram->setUniformValue("p", projection);
    renderRaytracerScene();

    // relases the current shaderprogram (to bind an use another shaderprogram for example)
    m_shaderProgram->release();
}

// Below are functions which are triggered by input events:

// Triggered by pressing a key
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    switch(ev->key()) {
    case 'A': qDebug() << "A pressed"; break;
    default:
        // ev->key() is an integer. For alpha numeric characters keys it equivalent with the char value ('A' == 65, '1' == 49)
        // Alternatively, you could use Qt Key enums, see http://doc.qt.io/qt-5/qt.html#Key-enum
        qDebug() << ev->key() << "pressed";
        break;
    }

    // Used to update the screen
    renderLater();
}

// Triggered by releasing a key
void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    switch(ev->key()) {
    case 'A': qDebug() << "A released"; break;
    default:
        qDebug() << ev->key() << "released";
        break;
    }

    renderLater();
}

// Triggered by clicking two subsequent times on any mouse button.
void MainWindow::mouseDoubleClickEvent(QMouseEvent *ev)
{
    qDebug() << "Mouse double clicked:" << ev->button();

    renderLater();
}

// Triggered when moving the mouse inside the window (even when no mouse button is clicked!)
void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    //alowing a user to rotate
    if (rotate){

        std::cout << "(" << lastX << ", " << lastY << ") ->" << "(" << ev->x() << ", " << ev->y() << ")" << std::endl;
        model.rotate(1.5, ev->x() - lastX, ev->y() - lastY, 0);

        renderLater();
    }
}

// Triggered when pressing any mouse button
void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "Mouse button pressed:" << ev->button();
    lastX = ev->x();
    lastY = ev->y();

    rotate = true;

    renderLater();
}

// Triggered when releasing any mouse button
void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << "Mouse button released" << ev->button();
    rotate = false;

    renderLater();
}

// Triggered when clicking scrolling with the scroll wheel on the mouse
void MainWindow::wheelEvent(QWheelEvent * ev)
{
    // allowing a user to zoom
    qDebug() << "Mouse wheel:" << ev->delta();
    if (ev->delta()>0)
        model.scale(1.2);
    else model.scale(0.9);
     renderLater();
}
