//
//  MyTJConnectListener.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/09/20.
//

#ifndef MyTJConnectListener_hpp
#define MyTJConnectListener_hpp

#include "cocos2d.h"
#include "Tapjoy/TapjoyCpp.h"

class MyTJConnectListener : public tapjoy::TJConnectListener {
public:
    virtual void onConnectSuccess() override;
    virtual void onConnectFailure() override;
};

#endif /* MyTJConnectListener_hpp */
