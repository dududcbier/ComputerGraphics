#include "mainwindow.h"

#include <iostream>
#include <QDebug>
#include <QScreen>
#include <math.h>
#include <QTimer>

#define PI 3.14159265

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
    squareVAO.destroy();

    // free the pointer of the shading program
    delete m_shaderProgram;
    delete colors_buffer;
    delete coordinates;
    delete blur_shaderProgram;

    delete texture_buffer;
    delete texture;

    delete square_buffer;
    delete pointerForblur1;
    delete pointerForblur2;

}

// Initialize all your OpenGL objects here
void MainWindow::initialize(){
    rotate = false;
    t = 0;

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

    // Initialize the shaders
    blur_shaderProgram = new QOpenGLShaderProgram(this);
    // Use the ":" to load from the resources files (i.e. from the resources.qrc)
    blur_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/blur_vertex.glsl");
    blur_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/blur_fragment.glsl");
    blur_shaderProgram->link();

    // Shaders are initialized
    // You can retrieve the locations of the uniforms from here


    // Initialize your objects and buffers

    //OBJModel cube = OBJModel(":/models/cube.obj");

    OBJModel sphere = OBJModel(":/models/sphere.obj");
    qDebug() << "Object read";
    QVector<QVector3D> vertices = sphere.vertices;
    QVector<QVector2D> textureCoordinates = sphere.texcoords;

    nVertices = vertices.length();

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
    
    // Coordinates

    coordinates = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    coordinates->create();
    coordinates->bind();

    coordinates->allocate(vertices.data(), vertices.length() * sizeof(QVector3D));
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	
    // Color Buffer

	colors_buffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	colors_buffer->create();
    colors_buffer->bind();

    colors_buffer->allocate(colors.data(), colors.length() * sizeof(QVector3D));
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

    // Texture Buffer

    texture_buffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    texture_buffer->create();
    texture_buffer->bind();

    texture_buffer->allocate(textureCoordinates.data(), textureCoordinates.length() * sizeof(QVector2D));
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,0);

    // Simple Square VBO

    squareVAO.create();
    squareVAO.bind();

    // Create a simple square VAO for rendering a plain texture
    QVector<QVector3D> sq;
    sq.append(QVector3D(-1.0f,-1.0f,0.0f));
    sq.append(QVector3D(1.0f,1.0f,0.0f));
    sq.append(QVector3D(-1.0f,1.0f,0.0f));

    sq.append(QVector3D(-1.0f,-1.0f,0.0f));
    sq.append(QVector3D(1.0f,-1.0f,0.0f));
    sq.append(QVector3D(1.0f,1.0f,0.0f));

    square_buffer = new  QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    square_buffer->create();
    square_buffer->bind();
    square_buffer->allocate(sq.data(),sq.size() * sizeof(sq[0]));

    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,0);

    squareVAO.release();

    // Enabling all vertex attrib
    // Note that the normal buffer was not created on this version
    // and therefore doesn't need to be enabled

    for (int index = 0; index <= 3; index++)
        glEnableVertexAttribArray(index);

    pointerForblur1 = new QOpenGLFramebufferObject(400,400);
    pointerForblur2 = new QOpenGLFramebufferObject(400,400);

    pointerForblur1->setAttachment(QOpenGLFramebufferObject::Depth);
    pointerForblur2->setAttachment(QOpenGLFramebufferObject::Depth);

    // Texture

    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,  QOpenGLTexture::Linear);
    texture->setData(QImage(QString(":/textures/texture.png")).mirrored());
		
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

    model.setToIdentity();

    projection.perspective(90.0, (float)width()/height(), 50, 2000);
    view.translate(-100,-100, -1300);

    qDebug() << "End of function initialize()";
}

void MainWindow::renderSphere(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos)
{
    model.setToIdentity();
    model.translate(pos);

    // Phong Shading was not implemented correctly and was disabled in this assignment
    //m_shaderProgram->setUniformValue("lightpos", lightpos);
    // m_shaderProgram->setUniformValue("materialColor", color);

    //model.setColumn(3,QVector4D(pos,1));
    // m_shaderProgram->setUniformValue("intensity", material);
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
    QVector3D lightpos = QVector3D(-200,1200,1500);

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
    blur_shaderProgram->bind();
    object.bind();
    squareVAO.bind();

    // Bind the texture
    texture->bind(GL_TEXTURE0);
    
    // Rendering can be done here
    // Any transformation you whish to do or setting a uniform
    // should be done before any call to glDraw
    m_shaderProgram->setUniformValue("v", view);
    m_shaderProgram->setUniformValue("p", projection);

    //glDrawArrays(GL_TRIANGLES, 0, nVertices);

    // OpenGl assignment 1, part 2:
    // To render the scene from the raytracer:
    // Make sure your camera is positioned at (200,200,1000) in world coordinates
    // before calling the render function.
    // Make sure that the light position is fixed relative to the world.
    // (You should be able to see the dark side of the scene when rotation)

    //renderRaytracerScene();

    renderAnimatedScene();

    // relases the current shaderprogram (to bind an use another shaderprogram for example)
    m_shaderProgram->release();
}

void MainWindow::renderAnimatedScene()
{

    // Sun
    renderPlanet(0, 1, 0, QVector3D(145, 160, 300), 2.9);

    // Planet 1
    renderPlanet(300, 2, 1.8, QVector3D(145, 160, 300), .5);

    // Planet 2
    renderPlanet(400, 4, 1.4, QVector3D(145, 160, 300), 0.9);

    // Planet 3
    renderPlanet(600, 3, 1.1, QVector3D(145, 160, 300), 1.3);

    // Planet 4
    renderPlanet(820, 5, 0.4, QVector3D(145, 160, 300), 1.6);
    
    t++;
    renderLater();
}
void MainWindow::renderPlanet(float centerDistance, float speedArroundSelf, float speedAroundCenter, QVector3D originalPos, float size){

    model.setToIdentity();

    // Using x = r * cos(a) + x_0 and y = r * sin(a) + y_0
    // Angles are in radians
    qreal deltaX = centerDistance * cos(t * speedAroundCenter * PI / 180) + originalPos.x();
    qreal deltaY = centerDistance * sin(t * speedAroundCenter * PI / 180) + originalPos.y();

    // Moving around the sun
    model.translate(deltaX, deltaY, originalPos.z());

    // Moving around itself
    model.rotate(t * speedArroundSelf, 0, 0 ,1);

    // Sizing the planet
    model.scale(size);

    m_shaderProgram->setUniformValue("m", model);

    glDrawArrays(GL_TRIANGLES, 0, nVertices);

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
        model.rotate(ev->x() - lastX, 1, 0, 0);
        model.rotate(ev->y() - lastY, 0, 1, 0);
        lastX = ev->x();
        lastY = ev->y();

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
