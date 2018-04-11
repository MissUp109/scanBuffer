//
//  Rasterizer.h
//  ScanLineZBuffer
//
//  Created by yy on 2017/12/24.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#ifndef Rasterizer_h
#define Rasterizer_h
#include "Model.h"
class Rasterizer{
private:
    int width, height;
    float scaleRation;
public:
    Rasterizer(int width, int height);
    Rasterizer(int width, int height, float scaleRation);
    void getSize(int& width, int& height);
    void Execute(Model& model);
};

#endif /* Rasterizer_h */
