//
//  Model.cpp
//  ScanLineZBuffer
//
//  Created by yy on 2017/12/23.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include "myString.h"
using namespace std;

Vertex::Vertex(){
    pos = Float3(0.0f, 0.0f, 0.0f);
    color = Float3(0.0f, 0.0f, 0.0f);
    //normal = Float3(0.0f, 0.0f, 0.0f);
}

Face::Face(){
    color = Float3(0.0f, 0.0f, 0.0f);
    normal = Float3(0.0f, 0.0f, 0.0f);
}

bool Model::Load(const string &filePath){
    ifstream file(filePath);
    if(!file.is_open()){
        cout << "Can't open the file." << endl;
        return false;
    }
    string line;
    while(getline(file, line)){//注意：顶点和法向量数据要在面数据前
        string type;
        stringstream stringPos;
        if(line.substr(0, 2) == "v "){//model.vertexList
            Vertex v;
            stringPos << line;
            stringPos >> type >> v.pos.x >> v.pos.y >> v.pos.z;
            vertexList.push_back(v);
        }
        else if(line.substr(0, 2) == "vn"){//model.normalList
            Float3 vn;
            stringPos << line;
            stringPos >> type >> vn.x >> vn.y >> vn.z;
            normalList.push_back(vn);
        }
        else if(line.substr(0, 2) == "f "){//model.faceList
            Face f;
            vector<string> strArr = Split(line, " ");
            
            for(int i = 1; i < strArr.size(); ++i){
                if(strArr[i].npos != strArr[i].find('/')){
                    vector<string> tmpArr = Split(strArr[i], "/");
                    if(tmpArr.size() != 3){
                        cout << "There is something wrong with the date of face." << endl;
                        return false;
                    }
                    
                    //stringstream stringVIdx;
                    int vIdx = atoi(tmpArr[0].c_str());//顶点索引：f.vertexIDX
                    f.vertexIDX.push_back(--vIdx);//obj的索引是从1开始

                    int nIdx = atoi(tmpArr[2].c_str());//法向量索引：f.normalIDX
                    f.normalIDX.push_back(--nIdx);//obj的索引是从1开始
                    
                    //将顶点与法向量对应(为了后面计算光照方便)
                    //vertexList[vIdx].normal = normalList[nIdx];
                }
            }
            
            for(int i = 0; i < f.normalIDX.size(); ++i)//面法向量：f.normal
                f.normal += normalList[f.normalIDX[i]];
            f.normal = normalize(f.normal);
            
            faceList.push_back(f);
        }
    }
    
    file.close();
    cout << "Load model successfully." << endl;
    return true;
}

