//
//  Utility.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/07/16.
//

#ifndef Utility_hpp
#define Utility_hpp

#include "json11.hpp"

namespace project {
    /**
     *  @brief
     */
    class Utility {
    public:
        static std::mt19937& getMt();
        static int32_t getRandomValue(int32_t _minimumValue, int32_t _maximumValue);
        static float getRandomValue(float _minimumValue, float _maximumValue);
        
        template <typename T>
        static T getRandomValue(std::vector<T> _valueAreaVector);
        
        template <typename EnumType>
        static typename std::underlying_type<EnumType>::type enumToValue(EnumType _enum);
        
        static float getTwoPointBetweenDistance(cocos2d::Vec2 _v1, cocos2d::Vec2 _v2);
        static float getTowPointBetweenAngle(cocos2d::Vec2 _v1, cocos2d::Vec2 _v2);
        static float exchangeFrameToSecond(float _frame);
        static int32_t getCurrentTimeStampValue();
        static std::string getCurrentTimeStampString();
        static std::string exchangeTimeStampToDateTime(time_t _timeStamp);
        static time_t exchangeDateTimeToTimeStamp(const std::string& _dateTime);
        static time_t getTommorowsTimeStamp();
        static time_t getRemainingPeriodSeconds(time_t _endTimeStamp);
        static time_t getDataFromTimeStamp(time_t _timeStamp);
        static int32_t getStringsCount(const std::string& _string);
        static int32_t getStringsPositionFromCount(const std::string& _string, int32_t _count);
        static std::string replaceString(std::string _baseString, const std::string& _targetString, const std::string& _replaceString);
        static std::vector<std::string> splitString(const std::string& _baseString, char _delim);
        static json11::Json loadJsonData(const std::string& _filePath);
        static json11::Json parseJson(const std::string& _key);
        static void createDisableTouchArea(cocos2d::Node* _node);
        static void removeDisableTouchArea(cocos2d::Node* _node);
        static int32_t getNumberDigit(int32_t _number);
        
        
        
    };
}

#endif /* Utility_hpp */
