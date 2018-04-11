//
//  Render.h
//  ScanLineZBuffer
//
//  Created by yy on 2017/12/24.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#ifndef Render_h
#define Render_h
#include "Float3.h"
#include "Model.h"

class Render{
private:
    //int winW, winH;
    const float albedo = 0.8;
    const Float3 ambientColor = Float3(0.3, 0.3, 0.3);
    Float3 lightPos;
    Float3 lightColor;
public:
    Render(int winW, int winH);
    void Execute(Model& model);
};

#endif /* Render_h */
