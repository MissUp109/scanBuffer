//
//  main.cpp
//  ScanLineZBuffer
//
//  Created by yy on 2017/12/23.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#include <iostream>
#include <string.h>
#include "Model.h"
#include "Render.h"
#include "Rasterizer.h"
#include "ScanLineZBuffer.h"
#include "myString.h"
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
using namespace std;

string ROOT = "/Users/leah/Desktop/CG/OBJ/";

int width = 800;
int height = 600;

Model model;
ScanLineZBuffer zBuffer;

void init(){
    string filePath = ROOT + "bunny.obj";
    model.Load(filePath);
    
    Rasterizer rasterizer(width, height);
    rasterizer.Execute(model);
    
    Render render(width, height);
    render.Execute(model);
    
    zBuffer.setSize(width, height);
    zBuffer.Execute(model);
}

void myDraw(){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    
    glBegin(GL_POINTS);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Float3 rgb(0.0f, 0.0f, 0.0f);
            int fid = zBuffer.frameBuffer[y][x];
            if (fid >= 0)
                rgb = model.faceList[fid].color;
            glColor3f(rgb.r, rgb.g, rgb.b);
            glVertex2i(x, y);
        }
    }
    glEnd();
    glFinish();
}

void getMouseDown(int button, int state, int x, int y){
    
    if(state == GLUT_DOWN){
        cout << x << " " << y << endl;
    }
}
int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(50, 80);
    glutCreateWindow("ScanLineZBuffer");
    init();
    glutDisplayFunc(myDraw);
    glutMouseFunc(getMouseDown);
    glutMainLoop();
    return 0;
}


