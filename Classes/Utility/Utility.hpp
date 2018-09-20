//
//  Utility.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/07/16.
//

#ifndef Utility_hpp
#define Utility_hpp

namespace project {
    /**
     *
     */
    class Utility {
    public:
        static int32_t getRandomValue(int32_t minimumValue, int32_t maximumValue);
        static float getRandomValue(float minimumValue, float maximumValue);
        
        template <typename T>
        static T getRandomValue(std::vector<T> valueAreaVector);
        
        
        
    };
}

#endif /* Utility_hpp */
