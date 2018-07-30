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
     *  @brief ランダム値を取得
     */
    int32_t Utility::getRandomValue(int32_t minimumValue, int32_t maximumValue) {
        int32_t rndValue = 0;
        
        // 確率
        {            
            std::random_device seed;
            std::mt19937 rng(seed());
            std::uniform_int_distribution<> rndArea(minimumValue, maximumValue);
            
            rndValue = rndArea(rng);
         }
        
        return rndValue;
    }
    
    /**
     *  @brief ランダム値を取得
     */
    float Utility::getRandomValue(float minimumValue, float maximumValue) {
        float rndValue = 0.0f;
        
        // 確率
        {
            std::random_device seed;
            std::mt19937 rng(seed());
            std::uniform_real_distribution<> rndArea(minimumValue, maximumValue);
            
            rndValue = rndArea(rng);
        }
        
        return rndValue;
    }
    
    /**
     *  @brief ランダム値を取得
     */
    template <typename T>
    T Utility::getRandomValue(std::vector<T> valueAreaVector) {
        T rndValue;
        
        // 確率
        {
            std::vector<T> prob = valueAreaVector;
            
            std::random_device seed;
            std::mt19937 rng(seed());
            std::discrete_distribution<T> dst;
            dst.param(decltype(dst.param())(prob.begin(), prob.end()));
            
            T rndValue = dst(rng);
        }
        
        return rndValue;
    }
}
