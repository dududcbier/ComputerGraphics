#include "mainwindow.h"

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

    // Free all your used resources here

    // Destroy buffers first, before destroying the VertexArrayObject
    object.destroy();

    // free the pointer of the shading program
    delete m_shaderProgram;
    delete colors_buffer;
    delete coordinates;

}

// Initialize all your OpenGL objects here
void MainWindow::initialize()
{
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
    // You can retrieve the locations of the uniforms from here


    // Initialize your objects and buffers

    OBJModel cube = OBJModel(":/models/cube.obj");
    QVector<QVector3D> vertices = cube.vertices;

    nVertices = vertices.length();

    QVector<QVector3D> colors;
    for (int i = 0; i < cube.indices.length(); i += 3){
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
	
	colors_buffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	colors_buffer->create();
    colors_buffer->bind();
    
    colors_buffer->allocate(colors.data(), colors.length() * sizeof(QVector3D));
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

     QVector<QVector2D> textureCoordinates;
    
    texture_buffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    texture_buffer->create();
    texture_buffer->bind();
    texture_buffer->allocate(textureCoordinates.data(), textureCoordinates.length() * sizeof(QVector2D));
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,0);

   // QOpenGLTexture(QOpenGLTexture::Target2D);

    for (int index = 0; index <= 2; index++)
		glEnableVertexAttribArray(index);
		
	object.release();

    // Set OpenGL to use Filled triangles (can be used to render points, or lines)
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    // Enable Z-buffering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    qDebug() << "Depth Buffer size:" <<  this->format().depthBufferSize() << "bits";

    // Function for culling, removing faces which don't face the camera
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    // Set the clear color to be black (color used for resetting the screen)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void MainWindow::renderSphere(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos)
{
    // OpenGL assignment 1, part 2: create a function to render the sphere
    // Use OBJModel(":/models/spehere.obj") for the model

    // you must remove these Q_UNUSED when you implement this function
    Q_UNUSED(pos)
    Q_UNUSED(color)
    Q_UNUSED(material)
    Q_UNUSED(lightpos)
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
    
    // Rendering can be done here
    // Any transformation you whish to do or setting a uniform
    // should be done before any call to glDraw*
    model.setToIdentity();
    view.setToIdentity();
    projection.setToIdentity();

    view.perspective(60.0, 1, 0.1, 2000);
    model.translate(0, 0, -4);

    m_shaderProgram->setUniformValue("m", model);
    m_shaderProgram->setUniformValue("v", view);
    m_shaderProgram->setUniformValue("p", projection);

    glDrawArrays(GL_TRIANGLES, 0, nVertices);

    // OpenGl assignment 1, part 2:
    // To render the scene from the raytracer:
    // Make sure your camera is positioned at (200,200,1000) in world coordinates
    // before calling the render function.
    // Make sure that the light position is fixed relative to the world.
    // (You should be able to see the dark side of the scene when rotation)

    // renderRaytracerScene()

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
    //qDebug() << ev->button();
    //renderLater();

    // you must remove the Q_UNUSED when you implement this function
    Q_UNUSED(ev)
}

// Triggered when pressing any mouse button
void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "Mouse button pressed:" << ev->button();
    xMouse = ev->x();
    yMouse = ev->y();

    renderLater();
}

// Triggered when releasing any mouse button
void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << "Mouse button released" << ev->button();

    renderLater();
}

// Triggered when clicking scrolling with the scroll wheel on the mouse
void MainWindow::wheelEvent(QWheelEvent * ev)
{
    // Implement something
    qDebug() << "Mouse wheel:" << ev->delta();
}
