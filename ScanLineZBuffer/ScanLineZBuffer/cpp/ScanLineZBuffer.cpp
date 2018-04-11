//
//  ScanLineZBuffer.cpp
//  ScanLineZBuffer
//
//  Created by yy on 2017/12/23.
//  Copyright © 2017年 Lynn. All rights reserved.
//
#include "ScanLineZBuffer.h"
#include <iostream>
ScanLineZBuffer::ScanLineZBuffer(int width, int height){
    setSize(width, height);
}

ScanLineZBuffer::~ScanLineZBuffer(){
    Release();
}

void ScanLineZBuffer::Release(){
    if (zBuffer != NULL)
    {
        delete[] zBuffer;
        zBuffer = NULL;
    }
    
    if (frameBuffer != NULL)
    {
        for (int i = 0; i < height; ++i)
        {
            delete[] frameBuffer[i];
            frameBuffer[i] = NULL;
        }
    }
    delete[] frameBuffer;
    frameBuffer = NULL;
}


void ScanLineZBuffer::setSize(int width, int height){
    if(width <= 0 || height <= 0){
        //cout << "The size must bigger than zero." << endl;
        return;
    }
    this->width = width;
    this->height = height;
    zBuffer = new float[width];
    frameBuffer = new int*[height];
    for(int i = 0; i < height; ++i){
        frameBuffer[i] = new int[width];
    }
}

void ScanLineZBuffer::getSize(int &width, int &height){
    width = this->width;
    height = this->height;
}

void ScanLineZBuffer::Init(Model &model){
    edgeTable.clear();
    polygonTable.clear();
    
    edgeTable.resize(height);   //初始化边表的高度
    polygonTable.resize(height);//初始化多边形表的高度
    
    //omp_lock_t mylock;
    //omp_init_lock(&mylock);
    
    vector<Face>& faces = model.faceList;
    vector<Vertex>& vertexes = model.vertexList;
    
    for(int i = 0; i < faces.size(); ++i){//遍历面->遍历多边形
        Vertex v1, v2, vHigh;
        v1 = vertexes[faces[i].vertexIDX[0]];

        Polygon newPolygon;//添加多边形
        newPolygon.a = faces[i].normal.x;
        newPolygon.b = faces[i].normal.y;
        newPolygon.c = faces[i].normal.z;
        newPolygon.d = -(newPolygon.a * v1.pos.x + newPolygon.b * v1.pos.y + newPolygon.c * v1.pos.z);
        newPolygon.color = faces[i].color;
        
        float vYMax = v1.pos.y, vYmin = v1.pos.y;
        int vIdxSize = (int)faces[i].vertexIDX.size();
        for(int j = 1; j <= vIdxSize; ++j){
            v2 = vertexes[faces[i].vertexIDX[j % vIdxSize]];
            
            if(vYMax < v2.pos.y)//找到多边形y值最大点
                vYMax = v2.pos.y;
            
            if(vYmin > v2.pos.y)//找到多边形y值最小点
                vYmin = v2.pos.y;
            /*
            if(v1.pos.y == v2.pos.y){
                v1 = v2;
                continue;
            }*/
            
            Edge newEdge;//添加边
            if(newPolygon.c != 0){
                newEdge.dxy = -(v1.pos.x - v2.pos.x) / (v1.pos.y - v2.pos.y);
                newEdge.dy = abs((int)v1.pos.y - (int)v2.pos.y);
                newEdge.dzx = - newPolygon.a / newPolygon.c;
                newEdge.dzy = newPolygon.b / newPolygon.c;
            }
            else{
                newEdge.dxy = -(v1.pos.x - v2.pos.x) / (v1.pos.y - v2.pos.y);
                newEdge.dy = abs((int)v1.pos.y - (int)v2.pos.y);
                newEdge.dzx = _Infinity;
                newEdge.dzy = _Infinity;
            }
            
            if(v1.pos.y > v2.pos.y)
                vHigh = v1;
            else if(v1.pos.y < v2.pos.y)
                vHigh = v2;
            else{
                if(v1.pos.x > v2.pos.x)//这一步在扫描平行于x轴的边时很有用
                    vHigh = v1;
                else if(v1.pos.x < v2.pos.x)
                    vHigh = v2;
                else{
                    if(v1.pos.z > v2.pos.z) vHigh = v1;
                    else vHigh = v2;
                }
            }
            
            newEdge.x = vHigh.pos.x;
            newEdge.z = vHigh.pos.z;
            newEdge.pid = i;
            //omp_set_lock(&mylock);
            if(newEdge.dy != 0)
                edgeTable[(int)(vHigh.pos.y)].push_back(newEdge);
            //omp_unset_lock(&mylock);
            v1 = v2;
        }
        
        newPolygon.dy = (int)vYMax - (int)vYmin;
        newPolygon.pid = i;
        //omp_set_lock(&mylock);
        if(newPolygon.dy != 0)
            polygonTable[(int)vYMax].push_back(newPolygon);
        //omp_unset_lock(&mylock);
    }
    //omp_destroy_lock(&mylock);
    cout << "Table init successfully." << endl;
}

bool CompareActiveEdge(const ActiveEdge& ae1, const ActiveEdge& ae2){
    return (ae1.x < ae2.x);
}

void ScanLineZBuffer::Execute(Model& model){
    clock_t start_t = clock();
    Init(model);

    activeEdgeTable.clear();
    activePolygonTable.clear();
    
    for(int currHeight = height - 1; currHeight >= 0; --currHeight){
        fill(zBuffer, zBuffer + width, -__FLT_MAX__);
        memset(frameBuffer[currHeight], -1, sizeof(int) * width);
        
        //加入新的活多边形
        if(polygonTable[currHeight].size() != 0){
            for(list<Polygon>::iterator polyItr = polygonTable[currHeight].begin(); polyItr != polygonTable[currHeight].end();++polyItr){
                ActivePolygon newActivePoly;
                newActivePoly.pid = (*polyItr).pid;
                newActivePoly.a = (*polyItr).a;
                newActivePoly.b = (*polyItr).b;
                newActivePoly.c = (*polyItr).c;
                newActivePoly.d = (*polyItr).d;
                newActivePoly.dy = (*polyItr).dy;
                newActivePoly.color = (*polyItr).color;
                newActivePoly.flag = false;
                activePolygonTable.push_back(newActivePoly);
            }
        }
        
        //加入新的活边
        if(edgeTable[currHeight].size() != 0){
            for(list<Edge>::iterator edgeItr = edgeTable[currHeight].begin(); edgeItr != edgeTable[currHeight].end(); ++edgeItr){
                ActiveEdge newActiveEdge;
                newActiveEdge.x = (*edgeItr).x;
                newActiveEdge.dxy = (*edgeItr).dxy;
                newActiveEdge.dy = (*edgeItr).dy;
                newActiveEdge.z = (*edgeItr).z;
                newActiveEdge.dzx = (*edgeItr).dzx;
                newActiveEdge.dzy = (*edgeItr).dzy;
                newActiveEdge.pid = (*edgeItr).pid;
                activeEdgeTable.push_back(newActiveEdge);
            }
        }
        
        if(activeEdgeTable.size() <= 0 || activePolygonTable.size() <= 0){
            continue;
        }
        
        //将活边表按照x的值排序
        sort(activeEdgeTable.begin(), activeEdgeTable.end(), CompareActiveEdge);

        //存储扫描线交点的x值
        vector<float> crosses;
        for(int c = 0; c < activeEdgeTable.size(); ++c){
            if(crosses.size()  == 0)
                crosses.push_back(activeEdgeTable[c].x);
            
            if(crosses[crosses.size() - 1] != activeEdgeTable[c].x)
                crosses.push_back(activeEdgeTable[c].x);
        }
        
        //更新缓存
        int i = 0;
        for(int j = 0; j < activeEdgeTable.size(); ++j){
            if(i >= crosses.size() - 1)
                break;
            
            if(activeEdgeTable[j].x == crosses[i]){
                for(int k = 0; k < activePolygonTable.size(); ++k){
                    if(activePolygonTable[k].pid == activeEdgeTable[j].pid){
                        //if(currHeight == 332 && activeEdgeTable[j].pid == 129){
                            //cout << "Polygon.pid = 129, flag = " << activePolygonTable[k].flag << endl;
                        //    cout << "i = " << i << "; crosses[i] = " << crosses[i] << endl;
                        //}
                        activePolygonTable[k].flag = !activePolygonTable[k].flag;
                        break;
                    }
                }
            }
            else{
                vector<ActivePolygon> truePolygon;
                for(int k = 0; k < activePolygonTable.size(); ++k){
                    if(activePolygonTable[k].flag == true)
                        truePolygon.push_back(activePolygonTable[k]);
                }
                
                int pidTmp = -1;
                float zMax = -__FLT_MAX__, zTmp;
                
                for(int n = 0; n < truePolygon.size(); ++n){
                    if(truePolygon[n].c == 0)
                        continue;
                    float xTmp = (crosses[i] + crosses[i + 1])/2;
                    float yTmp = currHeight;
                    zTmp = -(truePolygon[n].a * xTmp + truePolygon[n].b * yTmp + truePolygon[n].d)/ truePolygon[n].c;
                    
                    if(zMax < zTmp){
                        zMax = zTmp;
                        pidTmp = truePolygon[n].pid;
                    }
                }
                
                if(pidTmp != -1){
                    for(int scan = (int)crosses[i]; scan < (int)crosses[i + 1]; ++scan){
                        frameBuffer[currHeight][scan] = pidTmp;
                    }
                }
                
                --j;
                ++i;
            }
        }
        
        for(int t = 0; t < activePolygonTable.size(); ++t)
            activePolygonTable[t].flag = false;

        //更新活多边形表
        for(vector<ActivePolygon>::iterator itr = activePolygonTable.begin(); itr != activePolygonTable.end(); ){
            --(*itr).dy;
            if(((*itr).dy <= 0))
                activePolygonTable.erase(itr);
            else
                ++itr;
        }
        
        //更新活边表
        for(vector<ActiveEdge>::iterator itr = activeEdgeTable.begin(); itr != activeEdgeTable.end(); ){
            --(*itr).dy;
            if((*itr).dy <= 0)
                activeEdgeTable.erase(itr);
            else{
                (*itr).x += (*itr).dxy;
                (*itr).z += (*itr).dzx * (*itr).dxy + (*itr).dzy;
                ++itr;
            }
        }
    }
   cout << "耗时:" << float((clock() - start_t))/1000000 << "s" << endl;
 
}
