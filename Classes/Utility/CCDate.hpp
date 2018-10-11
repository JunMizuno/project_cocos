//
//  CCDate.hpp
//  project_cocos
//
//  Created by 水野淳 on 2018/10/04.
//

#ifndef CCDate_h
#define CCDate_h

#include <stdio.h>
#include <ctime>
#include <cstdio>
#include <cstring>
#include "string.h"

/**
 *  @brief CCDate
 */
class CCDate {
protected:
    time_t time_;
    struct tm tm_;
    
public:
    /**
     *  @brief コンストラクタ
     */
    CCDate() {
        time_ = ::time(NULL);
        ::localtime_r(&time_, &tm_);
    }
    
    CCDate(const CCDate& _dt) {
        time_ = _dt.time_;
        tm_ = _dt.tm_;
    }
    
    CCDate(const struct tm& _tm_val) {
        tm_ = _tm_val;
        time_ = ::mktime(&tm_);
    }
    
    explicit CCDate(const time_t _tt) {
        time_ = _tt;
        ::localtime_r(&time_, &tm_);
    }
    
    /**
     *  @brief オペレーターのオーバーライド
     */
    inline CCDate& operator = (const CCDate& _dt) {
        if (this != &_dt) {
            this->time_ = _dt.time_;
            this->tm_ = _dt.tm_;
        }
        
        return *this;
    }
    
    inline CCDate operator - (const CCDate& _dt) const {
        return CCDate(this->time_ - _dt.time_);
    }
    
    inline CCDate operator - (const CCDate& _dt) {
        return CCDate(this->time_ - _dt.time_);
    }
    
    inline CCDate operator + (const CCDate& _dt) const {
        return CCDate(this->time_ + _dt.time_);
    }
    
    inline CCDate operator + (const CCDate& _dt) {
        return CCDate(this->time_ + _dt.time_);
    }
    
    inline bool operator < (const CCDate& _dt) const {
        return this->time_ < _dt.time_;
    }
    
    inline bool operator < (const CCDate& _dt) {
        return this->time_ < _dt.time_;
    }
    
    inline bool operator <= (const CCDate& _dt) const {
        return *this < _dt || *this == _dt;
    }
    
    inline bool operator <= (const CCDate& _dt) {
        return *this < _dt || *this == _dt;
    }

    inline bool operator > (const CCDate& _dt) const {
        return this->time_ > _dt.time_;
    }
    
    inline bool operator > (const CCDate& _dt) {
        return this->time_ > _dt.time_;
    }

    inline bool operator >= (const CCDate& _dt) const {
        return *this > _dt || *this == _dt;
    }
    
    inline bool operator >= (const CCDate& _dt) {
        return *this > _dt || *this == _dt;
    }

    inline bool operator == (const CCDate& _dt) const {
        return this->time_ == _dt.time_;
    }
    
    inline bool operator == (const CCDate& _dt) {
        return this->time_ == _dt.time_;
    }

    inline bool operator != (const CCDate& _dt) const  {
        return this->time_ != _dt.time_;
    }
    
    inline bool operator != (const CCDate& _dt) {
        return this->time_ != _dt.time_;
    }
    
    /**
     *  @brief 各数値を取得
     */
    inline int32_t year() const {
        return this->tm_.tm_year + 1900;
    }
    
    inline void year(const int32_t _nyear) {
        this->tm_.tm_year = _nyear - 1900;
        this->time_ = ::mktime(&this->tm_);
    }
    
    inline int32_t month() const {
        return this->tm_.tm_mon + 1;
    }
    
    inline void month(const int32_t _nmon) {
        this->tm_.tm_mon = _nmon - 1;
        this->time_ = ::mktime(&this->tm_);
    }
    
    inline int32_t wday() const {
        return this->tm_.tm_wday;
    }
    
    inline int32_t mday() const {
        return this->tm_.tm_mday;
    }
    
    inline void mday(const int32_t _nday) {
        this->tm_.tm_mday = _nday;
        this->time_ = ::mktime(&this->tm_);
    }
    
    inline int32_t hour() const {
        return this->tm_.tm_hour;
    }
    
    inline void hour(const int32_t _nhour) {
        this->tm_.tm_hour = _nhour;
        this->time_ = ::mktime(&this->tm_);
    }
    
    inline int32_t minute() const {
        return this->tm_.tm_min;
    }
    
    inline void minute(const int32_t _nminute) {
        this->tm_.tm_min = _nminute;
        this->time_ = ::mktime(&this->tm_);
    }
    
    inline int32_t second() const {
        return this->tm_.tm_sec;
    }
    
    inline void second(const int32_t _nsecond) {
        this->tm_.tm_sec = _nsecond;
        this->time_ = ::mktime(&this->tm_);
    }
    
    /**
     *  @brief 各種ゲッター
     */
    inline time_t time() const {
        return this->time_;
    }

    time_t date() const {
        struct tm stm;
        ::localtime_r(&this->time_, &stm);
        stm.tm_hour = stm.tm_min = stm.tm_sec = 0;
        return ::mktime(&stm);
    }
    
    inline char* to_char(char* _pStr, int32_t _length) {
        if (_length < 20) {
            return _pStr;
        }
        
        ::strftime(_pStr, _length, "%Y-%m-%d %H:%M:%S", &this->tm_);
        
        // "YYYY-MM-DD hh:mm::ss" = 19
        if (_length > 20) {
            _length = 20;
        }
        
        _pStr[_length-1] = '\0';
        
        return _pStr;
    }
    
    inline char* to_str_mmddhhmi(char* _pStr, int32_t _length) {
        if (_length < 12) {
            return _pStr;
        }
        
        ::strftime(_pStr, _length, "%m/%d %H:%M", &this->tm_);
        
        // "MM/DD hh:mm" = 11
        if (_length > 12) {
            _length = 12;
        }
        
        _pStr[_length-1] = '\0';
        
        return _pStr;
    }
    
    inline char* date_to_str(char* _pStr, int32_t _length) {
        if (_length < 11) {
            return _pStr;
        }
        
        ::strftime(_pStr, _length, "%Y-%m-%d", &this->tm_);
        
        // "YYYY-MM-DD" = 10
        if (_length > 11) {
            _length = 11;
        }
        
        _pStr[_length-1] = '\0';
        
        return _pStr;
    }
    
    inline char* time_to_str(char* _pStr, int32_t _length) {
        if (_length < 9) {
            return _pStr;
        }
        
        ::strftime(_pStr, _length, "%H:%M:%S", &this->tm_);
        
        // "HH:MM:SS" = 8
        if (_length > 9) {
            _length = 9;
        }
        
        _pStr[_length-1] = '\0';
        
        return _pStr;
    }
    
    /**
     *  @brief 文字列をtime_t型にコンバートして返す
     *  @param const char* _pStr:"YYYY-MM-DD hh:mm:ss"の形のもの
     */
    inline time_t str_to_datetime(const char* _pStr) {
        if (_pStr == 0) {
            return 0;
        }
        
        ::memset(&this->tm_, 0, sizeof(this->tm_));
        ::strptime(_pStr, "%Y-%m-%d %H:%M:%S", &this->tm_);
        this->time_ = ::mktime(&this->tm_);
        
        return this->time_;
    }
    
    /**
     *  @brief time_t型の数値を文字列にコンバートして返す
     *  @return "YYYY-MM-DD hh:mm:ss"にして返す
     */
    inline std::string datetime_to_str(const time_t& _time) {
        tm* pDate = ::localtime(&_time);
        if (pDate == nullptr) {
            pDate = ::localtime(&this->time_);
        }
        
        char* pTime_str;
        int32_t str_length = 20;
        pTime_str = (char*)malloc(str_length * sizeof(char));
        
        strftime(pTime_str, str_length, "%Y-%m-%d %H:%M:%S", pDate);
        std::string retStr = pTime_str;
        
        free(pTime_str);
        
        return std::string(pTime_str);
    }
    
    /**
     *  @brief 文字列をtime_t型にコンバートして返す
     *  @param const char* _pStr:"hh:mm:ss"の形のもの
     */
    inline time_t str_to_time(const char* _pStr) {
        if (_pStr == 0) {
            return 0;
        }
        
        this->time_ = 0;
        ::localtime_r(&this->time_, &this->tm_);
        ::strptime(_pStr, "%H:%M:%S", &this->tm_);
        this->time_ = ::mktime(&this->tm_);
        
        return this->time_;
    }

    /**
     *  @brief 文字列をtime_t型にコンバートして返す
     *  @param const char* _pStr:"YYYY:MM:DD"の形のもの
     */
    inline time_t str_to_date(const char* _pStr) {
        if (_pStr == 0) {
            return 0;
        }
        
        ::memset(&this->tm_, 0, sizeof(this->tm_));
        ::strptime(_pStr, "%Y-%m-%d", &this->tm_);
        this->time_ = ::mktime(&this->tm_);
        
        return this->time_;
    }
    
    /**
     *  @brief 現在の日付で更新する
     */
    inline time_t updateByCurrentDate() {
        this->time_ = ::time(NULL);
        ::localtime_r(&this->time_, &this->tm_);
        
        return this->time_;
    }
    
    /**
     *  @brief 指定した日付で更新する
     */
    inline time_t updateBySpecifiedDate(time_t _dtime) {
        this->time_ = _dtime;
        ::localtime_r(&this->time_, &this->tm_);
        
        return this->time_;
    }
};

#endif /* CCDate_h */
