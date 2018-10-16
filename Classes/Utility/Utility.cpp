//
//  Utility.cpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/07/16.
//

#include "Utility.hpp"
#include "GlobalDefine.hpp"
#include "ui/CocosGUI.h"
#include "CCDate.hpp"

USING_NS_CC;

namespace project {
    namespace {
        constexpr int8_t STRING_SIZE_ONE = 1;
        constexpr int8_t STRING_SIZE_TWO = 2;
        constexpr int8_t STRING_SIZE_THREE = 3;
        constexpr int8_t STRING_SIZE_FOUR = 4;
        constexpr int32_t TIME_COUNT_OF_ONE_DAY = 86400;
    }
    
    /**
     *  @brief 擬似乱数を生成・取得
     */
    std::mt19937& Utility::getMt() {
        static std::random_device rnd;
        static std::mt19937 mt(rnd());
        return mt;
    }
    
    /**
     *  @brief ランダム値を取得
     */
    int32_t Utility::getRandomValue(int32_t _minimumValue, int32_t _maximumValue) {
        int32_t rndValue = 0;
        
        // 確率
        {            
            std::random_device seed;
            std::mt19937 rng(seed());
            std::uniform_int_distribution<> rndArea(_minimumValue, _maximumValue);
            
            rndValue = rndArea(rng);
         }
        
        return rndValue;
    }
    
    /**
     *  @brief ランダム値を取得
     */
    float Utility::getRandomValue(float _minimumValue, float _maximumValue) {
        float rndValue = 0.0f;
        
        // 確率
        {
            std::random_device seed;
            std::mt19937 rng(seed());
            std::uniform_real_distribution<> rndArea(_minimumValue, _maximumValue);
            
            rndValue = rndArea(rng);
        }
        
        return rndValue;
    }
    
    /**
     *  @brief ランダム値を取得
     */
    template <typename T>
    T Utility::getRandomValue(std::vector<T> _valueAreaVector) {
        T rndValue;
        
        // 確率
        {
            std::vector<T> prob = _valueAreaVector;
            
            std::random_device seed;
            std::mt19937 rng(seed());
            std::discrete_distribution<T> dst;
            dst.param(decltype(dst.param())(prob.begin(), prob.end()));
            
            rndValue = dst(rng);
        }
        
        return rndValue;
    }
    
    /**
     *  @brief enumを数値に変換
     */
    template <typename EnumType>
    typename std::underlying_type<EnumType>::type Utility::enumToValue(EnumType _enum) {
        return static_cast<typename std::underlying_type<EnumType>::type>(_enum);
    }
    
    /**
     *  @brief 2点間の距離を取得
     */
    float Utility::getTwoPointBetweenDistance(Vec2 _v1, Vec2 _v2) {
        return std::sqrt((_v2.x - _v1.x) * (_v2.x - _v1.x) + (_v2.y - _v1.y) * (_v2.y - _v1.y));
    }
    
    /**
     *  @brief 2点間の角度を取得
     */
    float Utility::getTowPointBetweenAngle(Vec2 _v1, Vec2 _v2) {
        return CC_RADIANS_TO_DEGREES((_v2 - _v1).getAngle());
    }
    
    /**
     *  @brief フレームを秒にして返す
     */
    float Utility::exchangeFrameToSeconds(float _frame) {
        return _frame / FPS;
    }
    
    /**
     *  @brief 現在のタイムスタンプ値を取得
     */
    int32_t Utility::getCurrentTimeStampValue() {
        return static_cast<int32_t>(time(nullptr));
    }
    
    /**
     *  @brief 現在のタイムスタンプを日付文で取得
     */
    std::string Utility::getCurrentTimeStampString() {
        return exchangeTimeStampToDateTime(time(nullptr));
    }
    
    /**
     *  @brief タイムスタンプ値を文字列に変換
     */
    std::string Utility::exchangeTimeStampToDateTime(time_t _timeStamp) {
        const size_t size = 32;
        char dateTime[size];
        
        strftime(dateTime, size, "%Y-%m-%d %H:%M:%S", localtime(&_timeStamp));
        
        return std::string(dateTime);
    }
    
    /**
     *  @brief 日付文をタイムスタンプ値に変換
     */
    time_t Utility::exchangeDateTimeToTimeStamp(const std::string& _dateTime) {
        time_t currentTimeStamp = getCurrentTimeStampValue();
        tm tm = *localtime(&currentTimeStamp);
        
        if (!strptime(_dateTime.c_str(), "%Y-%m-%d %H:%M:%S", &tm)) {
            return 0;
        }
        
        return mktime(&tm);
    }
    
    /**
     *  @brief 次の日のタイムスタンプ値を取得
     *  @details 0:00時点のもの
     */
    time_t Utility::getTommorowsTimeStamp() {
        time_t currentTimeStamp = getCurrentTimeStampValue();
        tm tm = *localtime(&currentTimeStamp);
        tm.tm_hour = 0;
        tm.tm_min = 0;
        tm.tm_sec = 0;
        return mktime(&tm) + TIME_COUNT_OF_ONE_DAY;
    }
    
    /**
     *  @brief 指定された時間から残り時間を算出
     */
    time_t Utility::getRemainingPeriodSeconds(time_t _endTimeStamp) {
        return _endTimeStamp - getCurrentTimeStampValue();
    }
    
    /**
     *  @brief タイムスタンプから日付だけを取得
     */
    time_t Utility::getDataFromTimeStamp(time_t _timeStamp) {
        tm tm = *localtime(&_timeStamp);
        tm.tm_hour = 0;
        tm.tm_min = 0;
        tm.tm_sec = 0;
        return mktime(&tm);
    }
    
    /**
     *  @brief 文字の長さを取得
     *  @details 日本語が含まれているとデフォルト関数では正確な値が取得出来ないため
     */
    int32_t Utility::getStringsCount(const std::string& _string) {
        int32_t count = 0;
        int32_t pos = 0;
        unsigned char leadString;
        int32_t charSize = 0;
        
        for (; pos < _string.size(); pos += charSize, count++) {
            leadString = _string[pos];
            
            // ASCII文字
            if (leadString < 0x80) {
                charSize = STRING_SIZE_ONE;
            }
            // 以下、それ以外の漢字などの判定
            else if (leadString < 0xe0) {
                charSize = STRING_SIZE_TWO;
            }
            else if (leadString < 0xf0) {
                charSize = STRING_SIZE_THREE;
            }
            else {
                charSize = STRING_SIZE_FOUR;
            }
        }
        
        return count;
    }
    
    /**
     *  @brief 指定した数値の文字の位置を取得
     *  @details 日本語が含まれているとデフォルト関数では正確な値が取得出来ないため
     */
    int32_t Utility::getStringsPositionFromCount(const std::string& _string, int32_t _count) {
        int32_t retPosition = 0;
        unsigned char leadString;
        int32_t charSize = 0;
        
        for (int32_t i = 0; i < _count; retPosition += charSize, i++) {
            leadString = _string[retPosition];
            
            if (leadString < 0x80) {
                charSize = STRING_SIZE_ONE;
            }
            else if (leadString < 0xe0) {
                charSize = STRING_SIZE_TWO;
            }
            else if (leadString < 0xf0) {
                charSize = STRING_SIZE_THREE;
            }
            else {
                charSize = STRING_SIZE_FOUR;
            }
        }
        
        return retPosition;
    }
    
    /**
     *  @brief 指定した文字列を入れ替える
     */
    std::string Utility::replaceString(std::string _baseString, const std::string& _targetString, const std::string& _replaceString) {
        std::string::size_type pos(_baseString.find(_targetString));
        
        while (pos != std::string::npos) {
            _baseString.replace(pos, _targetString.length(), _replaceString);
            pos = _baseString.find(_targetString, pos + _replaceString.length());
        }
        
        return _baseString;
    }
    
    /**
     *  @brief 文字列を指定した文字で区切る
     */
    std::vector<std::string> Utility::splitString(const std::string& _baseString, char _delim) {
        std::vector<std::string> retVector;
        size_t current = 0, found = 0;
        
        while ((found = _baseString.find_first_of(_delim, current)) != std::string::npos) {
            retVector.emplace_back(std::string(_baseString, current, found - current));
            current = found + 1;
        }
        retVector.emplace_back(std::string(_baseString, current, _baseString.size() - current));
        
        return retVector;
    }
    
    /**
     *  @brief jsonファイルを読み込む
     */
    json11::Json Utility::loadJsonData(const std::string& _filePath) {
        std::string jsonFile = FileUtils::getInstance()->getStringFromFile(_filePath);
        std::string error;
        
        json11::Json json = json11::Json::parse(jsonFile, error);
        if (error != "") {
            CCLOG("%s", error.c_str());
            return json11::Json();
        }
        
        return json;
    }
    
    /**
     *  @brief jsonデータを切り取る
     */
    json11::Json Utility::parseJson(const std::string& _key) {
        std::string error;
        
        json11::Json json = json11::Json::parse(_key, error);
        if (error != "") {
            CCLOG("%s", error.c_str());
            return json11::Json();
        }
        
        return json;
    }
        
    /**
     *  @brief 数値の桁数を取得
     */
    int32_t Utility::getNumberDigit(int32_t _number) {
        return log10f(_number) + 1;
    }
    
    /**
     *  @brief 重みの配列から抽選して値を返す
     */
    int32_t Utility::drawLots(const std::vector<int32_t>& _ratioVector) {
        int32_t total = std::accumulate(_ratioVector.begin(), _ratioVector.end(), 0);
        
        int32_t randomValue = (getMt()()) % total;
        for (int32_t i = 0; i < _ratioVector.size(); i++) {
            if (randomValue < _ratioVector.at(i)) {
                return _ratioVector.at(i);
            }
            
            randomValue -= _ratioVector.at(i);
        }
        
        return 0;
    }
    
    
    
    
    
    
}
