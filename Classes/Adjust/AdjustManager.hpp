//
//  AdjustManager.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2019/08/07.
//

#ifndef AdjustManager_hpp
#define AdjustManager_hpp

#include "cocos2d.h"

class AdjustManager {
private:
    AdjustManager();
    ~AdjustManager();

public:
    static AdjustManager* getInstance();
    
};

#endif /* AdjustManager_hpp */
