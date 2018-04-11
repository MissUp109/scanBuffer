//
//  Model.h
//  ScanLineZBuffer
//
//  Created by yy on 2017/12/23.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#ifndef Model_h
#define Model_h
#include <string>
#include <vector>
#include "Float3.h"
using namespace std;

class Vertex {
public:
    Float3 pos;
    Float3 color;
    //Float3 normal;
    //vector<Float3> normals;//公共顶点法向量不只一个
    Vertex();//对color初始化
};

class Face {
public:
    vector<int> vertexIDX;
    vector<int> normalIDX;
    Float3 color;
    Float3 normal;
    Face();//对color初始化
};

class Model {
public:
    vector<Vertex> vertexList;
    vector<Float3> normalList;
    vector<Face>   faceList;
    Model(){};
    //bool Test(const string& filePath);
    bool Load(const string& filePath);
};


#endif /* Model_h */
