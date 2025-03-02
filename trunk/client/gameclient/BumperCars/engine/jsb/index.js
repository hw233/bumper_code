/****************************************************************************
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
  worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
  not use Cocos Creator software for developing other software or tools that's
  used for developing games. You are not granted to publish, distribute,
  sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

'use strict';

require('../predefine');

if (!cc.ClassManager) {
    cc.ClassManager = window.ClassManager;
}

// polyfills
require('../polyfill/misc');
// str.startswith isn't supported in JavaScriptCore which is shipped with iOS8.
require('../polyfill/string');
if (!(CC_EDITOR && Editor.isMainProcess)) {
    require('../polyfill/typescript');
}

// predefine some modules for cocos
require('../cocos2d/core/platform/js');
require('../cocos2d/core/value-types');
require('../cocos2d/core/utils/find');
require('../cocos2d/core/utils/mutable-forward-iterator');
require('../cocos2d/core/event');
require('../cocos2d/core/event-manager/CCEvent');
require('../CCDebugger');

// Mark memory model
var macro = require('../cocos2d/core/platform/CCMacro');

if (window.__ENABLE_GC_FOR_NATIVE_OBJECTS__ !== undefined) {
    macro.ENABLE_GC_FOR_NATIVE_OBJECTS = window.__ENABLE_GC_FOR_NATIVE_OBJECTS__;
}

require('./jsb-game');
require('./jsb-loader');
require('./jsb-director');
require('./jsb-tex-sprite-frame');
require('./jsb-scale9sprite');
require('./jsb-label');
require('./jsb-editbox');
require('./jsb-videoplayer');
require('./jsb-webview');
require('./jsb-particle');
require('./jsb-spine');
require('./jsb-enums');
require('./jsb-event');
require('./jsb-action');
require('./jsb-etc');
require('./jsb-audio');
require('./jsb-tiledmap');
require('./jsb-box2d');
require('./jsb-dragonbones-asset');
require('./jsb-dragonbones');

require('../extends');
