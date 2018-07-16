//
//  Utility.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/07/16.
//

#include "Utility.hpp"

USING_NS_CC;

namespace project {
    namespace {
        
    }
    
    /**
     *
     */
    int32_t Utility::getRandomValue(int32_t minimumValue, int32_t maximumValue) {
        int32_t rndValue = 0;
        
        // 確率
        {            
            std::random_device seed;
            std::mt19937 rng(seed());
            std::uniform_int_distribution<> rndArea(minimumValue, maximumValue);
            
            int32_t rndValue = rndArea(rng);
            CCLOG("randValue:%d", rndValue);
        }
        
        return rndValue;
    }
    
    /**
     *
     */
    int32_t Utility::getRandomValue(std::vector<int32_t> valueAreaVector) {
        int32_t rndValue = 0;
        
        // 確率
        {
            std::vector<int32_t> prob = valueAreaVector;
            
            std::random_device seed;
            std::mt19937 rng(seed());
            std::discrete_distribution<int32_t> dst;
            dst.param(decltype(dst.param())(prob.begin(), prob.end()));
            
            int32_t rndValue = dst(rng);
            CCLOG("randValue:%d", rndValue);
        }
        
        return rndValue;
    }
}
