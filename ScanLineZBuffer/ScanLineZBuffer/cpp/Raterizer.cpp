//
//  Raterizer.cpp
//  ScanLineZBuffer
//
//  Created by yy on 2017/12/24.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#include "Rasterizer.h"
#include <math.h>
#include <iostream>
using namespace std;

Rasterizer::Rasterizer(int width, int height){
    this->width = width;
    this->height = height;
    this->scaleRation = 0.8;
}

Rasterizer::Rasterizer(int width, int height, float scaleRation){
    this->width = width;
    this->height = height;
    this->scaleRation = scaleRation;
}

void Rasterizer::getSize(int &width, int &height){
    width = this->width;
    height = this->height;
}

void Rasterizer::Execute(Model &model){
    Float3 min_xyz(__FLT_MAX__, __FLT_MAX__, __FLT_MAX__);
    Float3 max_xyz(-__FLT_MAX__, -__FLT_MAX__, -__FLT_MAX__);
    
    Float3 tmp;
    for(int i = 0; i < model.vertexList.size(); ++i){
        tmp = model.vertexList[i].pos;
        min_xyz.x = min(min_xyz.x, tmp.x);
        min_xyz.y = min(min_xyz.y, tmp.y);
        min_xyz.z = min(min_xyz.z, tmp.z);
        
        max_xyz.x = max(max_xyz.x, tmp.x);
        max_xyz.y = max(max_xyz.y, tmp.y);
        max_xyz.z = max(max_xyz.z, tmp.z);
    }
    
    float maxWidth = max_xyz.x - min_xyz.x;
    float maxHeight = max_xyz.y - min_xyz.y;
    float scale = min(width, height) / max(maxWidth, maxHeight);//投影时的比例
    scale = scale * scaleRation;//避免画面被占满
    
    Float3 mid_xyz = (min_xyz + max_xyz) / 2;
    for(int i = 0; i < model.vertexList.size(); ++i){
        model.vertexList[i].pos = (model.vertexList[i].pos - mid_xyz) * scale;
        model.vertexList[i].pos = model.vertexList[i].pos + Float3(width/2, height/2, 0);
    }
    
    cout << "Rasterizer successfully." << endl;
}
