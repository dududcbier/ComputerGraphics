//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"
#include "quad.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "camera.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>


// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;	
    return t;
}

int parseRenderMode(const YAML::Node& node)
{

    std::string s = "";
    node.GetScalar(s);

    if (s == "zbuffer") {
        cout << "Render Mode: 1 (zbuffer)" << endl; 
        return 1;
    }

    if (s == "normal") {
        cout << "Render Mode: 2 (normal)" << endl; 
        return 2; 
    }
    
    if (s == "phong") {
        cout << "Render Mode: 0 (Phong)" << endl; 
        return 0; 
    }

    if (s == "gooch") {
        cout << "Render Mode: 3 (Gooch)" << endl; 
        return 3; 
    }
    
    cout << "Render Mode: 0 (Phong)" << endl;
    return 0;
}

int parseShadow(const YAML::Node& node)
{

    std::string s = "";
    node.GetScalar(s);

    if (s == "true") {
        cout << "Shadows: True" << endl; 
        return 1;
    }

    if (s == "false") {
        cout << "Shadows: False" << endl; 
        return 0; 
    }
	
	cout << "Shadows: True" << endl; 
	return 1;
}

int parseMaxRecursionDepth(const YAML::Node& node){

    int x = node;
    cout << "MaxRecursionDepth: " << x << endl;
    return x;

}

int parseSuperSampling(const YAML::Node& node){
	
	int x;
	node["factor"] >> x;
	cout << "SuperSampling factor: " << x << endl;
    return x;
}

Camera parseCamera(const YAML::Node& node){
	Camera cam;
	Triple t;
	
	node["eye"] >> t;
	cam.setEye(t);
	
	node["center"] >> t;
	cam.setCenter(t);
	
	node["up"] >> t;
	cam.setUpVector(t);
	
	unsigned int x;
	node["viewSize"][0] >> x;
	cam.setWidth(x);
	
	unsigned int y;
	node["viewSize"][1] >> y;
	cam.setHeight(y);
	
	return cam;
}

void Raytracer::parseGoochParameters(const YAML::Node& node){
    double x;

    try {
        node["b"] >> x;
        scene->setB(x);
    }

    catch(YAML::KeyNotFound e){
        scene->setB(0.5);
    }

    try {
        node["y"] >> x;
        scene->setY(x);
    }
    catch(YAML::KeyNotFound e){
        scene->setY(0.5);
    }

    try {
        node["alpha"] >> x;
        scene->setAlpha(x);
    }
    catch(YAML::KeyNotFound e){
        scene->setAlpha(0.5);
    }

    try {
        node["beta"] >> x;
        scene->setBeta(x); 
    }
    catch(YAML::KeyNotFound e){
        scene->setBeta(0.5);
    }   
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{

    Material *m = new Material();
    
    try {
        std::string textureName;
		node["texture"] >> textureName;
        m->texture = new Image(textureName.c_str());
	}
	
	catch(const YAML::KeyNotFound e){
		m->texture = NULL;
	}
	
    try { 
        node["color"] >> m->color;
    }

    catch(const YAML::KeyNotFound e){
        m->color = Color(0, 0, 0);
    }
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;
    return m;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType;

    if (objectType == "sphere") {
        Point pos;
        node["position"] >> pos;

        double angle;
        try {
            node["angle"] >> angle;
        }
        catch (const YAML::KeyNotFound e){
            angle = 0;
        }

        double r;
        Triple rVec;
        try {
            node["radius"] >> r;
            rVec = Triple(0,0,0);
        }
        catch (const YAML::InvalidScalar e) {
            node["radius"][0] >> r;
            rVec = parseTriple(node["radius"][1]);
        }

        Sphere *sphere = new Sphere(pos,r,angle,rVec);		
        returnObject = sphere;
    }

    if (objectType == "triangle") { // Triangles are defined by three different points
        Point p1, p2, p3;
        node["p1"] >> p1;
        node["p2"] >> p2;
        node["p3"] >> p3;
        Triangle *triangle = new Triangle(p1, p2, p3);     
        returnObject = triangle;
    }
    
    if (objectType == "plane") { // Planes are defined by a point and 2 linearly independent vectors
        Point p;
        Vector v1, v2;
        node["p"] >> p;
        node["v1"] >> v1;
        node["v2"] >> v2;
        Plane *plane = new Plane(p, v1, v2);     
        returnObject = plane;
    }

    if (objectType == "quad") { // Planes are defined by a point and 2 linearly independent vectors
        Point p1, p2, p3, p4;
        node["p1"] >> p1;
        node["p2"] >> p2;
        node["p2"] >> p3;
        node["p3"] >> p4;
        Quad *quad = new Quad(p1, p2, p3, p4);     
        returnObject = quad;
    }

    if (returnObject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);
        if (parser) {
            YAML::Node doc;
            parser.GetNextDocument(doc);

            // RenderMode
            try {
                scene->setRenderMode(parseRenderMode(doc["RenderMode"]));
            }
            catch(const YAML::KeyNotFound e){
                scene->setRenderMode(0);
            }

            if (scene->renderMode == 3) {
                parseGoochParameters(doc["GoochParameters"]);
            }
            
            //Shadows 
            try {
                scene->setShadow(parseShadow(doc["Shadows"]));
            }
            catch(const YAML::KeyNotFound e){
                scene->setShadow(0);
            }

            //Max Recursion Depth
            try {
                scene->setMaxRecursionDepth(parseMaxRecursionDepth(doc["MaxRecursionDepth"]));
            }
            catch(const YAML::KeyNotFound e){
                scene->setMaxRecursionDepth(0);
            }
            
            // Super Sampling
            
            try {
                scene->setSuperSamplingFactor(parseSuperSampling(doc["SuperSampling"]));
            }
            catch(const YAML::KeyNotFound e){
                scene->setSuperSamplingFactor(1);
            }
            
            // Camera
            
            try {
                scene->setCamera(parseCamera(doc["Camera"]));
            }
            catch(const YAML::KeyNotFound e){ // If Camera doesn't exist in the yaml file, then that means Eye must exist. The other values are set to default.
				cout << "No camera defined" << endl;
				Camera cam;
				cam.setEye(parseTriple(doc["Eye"]));
				cam.setCenter(Triple(200,200,0));
				cam.setUpVector(Triple(0, 1, 0));
				cam.setHeight(400);
				cam.setWidth(400);
				
				scene->setCamera(cam);
            }				

            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }

            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj) {
                    scene->addObject(obj);
                } else {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                scene->addLight(parseLight(*it));
            }
        }
        if (parser) {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
	std::cout << "Width: " << scene->camera.getWidth() << " Height: " << scene->camera.getHeight() << std::endl;
    Image img(scene->camera.getWidth(),scene->camera.getHeight());
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}
