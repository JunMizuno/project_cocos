//
//  FirstScene.hpp
//  project_cocos-mobile
//
//  Created by Jun Mizuno on 2018/07/06.
//

#ifndef FirstScene_hpp
#define FirstScene_hpp

namespace project {
    /**
     *  @brief
     */
    class FirstScene final : public cocos2d::Layer {
    private:
        FirstScene() = default;
        virtual ~FirstScene() = default;
        
        static FirstScene* create();
        
        bool init() override;
        
    public:
        static cocos2d::Scene* createScene();
    };
}

#endif /* FirstScene_hpp */
