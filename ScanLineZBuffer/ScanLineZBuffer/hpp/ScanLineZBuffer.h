//
//  ScanLineZBuffer.h
//  ScanLineZBuffer
//
//  Created by yy on 2017/12/23.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#ifndef ScanLineZBuffer_h
#define ScanLineZBuffer_h
//#include <libiomp/omp.h>
#include <ctime>
#include "Model.h"
#include <list>
#define _Infinity 0x7F800000
struct Edge{
    float x;   //边上端点的x坐标
    float dxy;  //扫描线向下走过一个像素，点的x增量：-x/y
    int dy;    //边跨越的扫描线数
    
    float z;   //端点处多边形所在平面的深度值
    float dzx; //扫描线向右走过一个像素时，点的深度增量: z/x
    float dzy; //扫描线向下走过一个像素时，点的深度增量: -z/y
    
    int pid;   //所属的多边形编号
};

struct Polygon{
    int pid;          //多边形编号
    float a, b, c, d; //多边形所在平面方程
    int dy;           //多边形跨越的扫描线数
    Float3 color;     //多边形颜色
};

struct ActiveEdge{
    float x;  //交点的x坐标
    float dxy; //扫描线向下走过一个像素，点的x增量：-x/y
    float dy; //边跨越的扫描线数,随着扫描线下移减1
    
    float z;   //端点处多边形所在平面的深度值
    float dzx; //扫描线向右走过一个像素时，面的深度增量: z/x
    float dzy; //扫描线向下走过一个像素时，面的深度增量: -z/y
    
    int pid;   //所属的多边形编号
};

struct ActivePolygon{
    int pid;          //多边形编号
    float a, b, c, d; //多边形所在平面方程
    int dy;           //多边形跨越的扫描线数
    Float3 color;     //多边形颜色
    bool flag;        //是否进入、离开
};

class ScanLineZBuffer{
private:
    int width, height;
    float *zBuffer;//保存扫描线的z值
    vector<list<Edge>> edgeTable;      //边表
    vector<list<Polygon>> polygonTable;//多边形表
    vector<ActiveEdge> activeEdgeTable;//活化边表
    vector<ActivePolygon> activePolygonTable;//活化多边形表
    void Init(Model& model);
    //void Update();//更新活化边表、活化多边形表
    void Release();
public:
    int **frameBuffer;//保存多边形索引
    ScanLineZBuffer(){};
    ScanLineZBuffer(int width, int height);
    ~ScanLineZBuffer();
    void setSize(int width, int height);
    void getSize(int& width, int& height);
    void Execute(Model& model);
};

#endif /* ScanLineZBuffer_h */
