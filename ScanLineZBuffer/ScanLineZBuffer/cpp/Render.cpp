//
//  Render.cpp
//  ScanLineZBuffer
//
//  Created by yy on 2017/12/24.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#include "Render.h"
#include "Float3.h"
#include "Model.h"
#include <iostream>
using namespace std;
Render::Render(int winW, int winH){    
    lightPos = Float3(winW * 1.0 / 2, winH * 1.0 / 2, winW * 1.0 / 2);
    lightColor = Float3(0.3, 0.3, 0.3);
}

void Render::Execute(Model &model){
    for(int i = 0; i < model.faceList.size(); ++i){
        Face& tmpF = model.faceList[i];
        Float3 tmpPos = model.vertexList[tmpF.vertexIDX[0]].pos;
        for(int j = 1; j < tmpF.vertexIDX.size(); ++j){
            tmpPos += model.vertexList[tmpF.vertexIDX[j]].pos;
        }
        tmpPos /= tmpF.vertexIDX.size();//面的中心坐标
        
        tmpF.color = ambientColor;
        Float3 lightDir = normalize(lightPos - tmpPos);
        float cos = dot(lightDir, tmpF.normal);
        if(cos > 0)//光照模型：I = L * M * cos(Theta);
            tmpF.color += albedo * cos * lightColor;
        
        //颜色约束
        if(tmpF.color.r > 1.0f) tmpF.color.r = 1.0f;
        if(tmpF.color.g > 1.0f) tmpF.color.g = 1.0f;
        if(tmpF.color.b > 1.0f) tmpF.color.b = 1.0f;
        if(tmpF.color.r < 0.0f) tmpF.color.r = 0.0f;
        if(tmpF.color.g < 0.0f) tmpF.color.g = 0.0f;
        if(tmpF.color.b < 0.0f) tmpF.color.b = 0.0f;
    }
    
    cout << "Render successfully." << endl;
}

/*
void Render::Execute(Model &model){
    for(int i = 0; i < model.vertexList.size(); ++i){
        Vertex& tmpV = model.vertexList[i];
        Float3 lightDir = normalize(lightPos - tmpV.pos);
        float cos = dot(lightDir, tmpV.normal);
        tmpV.color = ambientColor;
        if(cos > 0) //光照模型：I = L * M * cos(Theta);
            tmpV.color += albedo * cos * lightColor;
    }
    
    for(int i = 0; i < model.faceList.size(); ++i){
        Face& tmpF = model.faceList[i];
        for(int j = 0; j < tmpF.vertexIDX.size(); ++j){
            tmpF.color += model.vertexList[j].color;
        }
        tmpF.color /= tmpF.vertexIDX.size();
        
        //颜色约束
        if(tmpF.color.r > 1.0f) tmpF.color.r = 1.0f;
        if(tmpF.color.g > 1.0f) tmpF.color.g = 1.0f;
        if(tmpF.color.b > 1.0f) tmpF.color.b = 1.0f;
        if(tmpF.color.r < 0.0f) tmpF.color.r = 0.0f;
        if(tmpF.color.g < 0.0f) tmpF.color.g = 0.0f;
        if(tmpF.color.b < 0.0f) tmpF.color.b = 0.0f;
    }
    
    cout << "Render successfully." << endl;
}*/

