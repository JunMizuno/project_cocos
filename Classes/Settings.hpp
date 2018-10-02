//
//  Settings.hpp
//  project_cocos-mobile
//
//  Created by 水野淳 on 2018/10/02.
//

#ifndef Settings_hpp
#define Settings_hpp

/**
 *  @brief バージョン情報
 */
#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define PATCH_VERSION 0

/**
 *  @brief リリースモード
 */
//#define ENABLE_RELEASE

/**
 *  @brief デバッグモード
 */
#ifndef ENABLE_RELEASE
#define ENABLE_DEBUG
#endif

#endif /* Settings_hpp */
