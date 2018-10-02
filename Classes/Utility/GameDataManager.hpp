//
//  GameDataManager.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/01.
//

#ifndef GameDataManager_hpp
#define GameDataManager_hpp

/**
 *  @brief
 */
class GameDataManager {
private:
    GameDataManager();
    virtual ~GameDataManager();
    
public:
    static GameDataManager* getInstance();
    
};

#endif /* GameDataManager_hpp */
