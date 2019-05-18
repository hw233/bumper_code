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

/**
 * @property {CanvasRenderingContext2D|WebGLRenderingContext} _renderContext - main Canvas 2D/3D Context of game engine
 * @private
 */
cc._renderContext = null;
cc._supportRender = false;

/**
 * @property {HTMLCanvasElement} _canvas - Main canvas of game engine
 * @private
 */
cc._canvas = null;

/**
 * @property {HTMLDivElement} container - The element contains the game canvas
 */
cc.container = null;
cc._gameDiv = null;

require('./cocos2d/core/utils');
require('./cocos2d/core/platform/CCSys');

// INIT ENGINE

var _engineInitCalled = false,
    _engineLoadedCallback = null;

cc._engineLoaded = false;

function _determineRenderType(config) {
    var CONFIG_KEY = cc.game.CONFIG_KEY,
        userRenderMode = parseInt(config[CONFIG_KEY.renderMode]) || 0;

    // Adjust RenderType
    if (isNaN(userRenderMode) || userRenderMode > 2 || userRenderMode < 0)
        config[CONFIG_KEY.renderMode] = 0;

    // Determine RenderType
    cc._renderType = cc.game.RENDER_TYPE_CANVAS;
    cc._supportRender = false;

    if (userRenderMode === 0) {
        if (cc.sys.capabilities['opengl']) {
            cc._renderType = cc.game.RENDER_TYPE_WEBGL;
            cc._supportRender = true;
        }
        else if (cc.sys.capabilities['canvas']) {
            cc._renderType = cc.game.RENDER_TYPE_CANVAS;
            cc._supportRender = true;
        }
    }
    else if (userRenderMode === 1 && cc.sys.capabilities['canvas']) {
        cc._renderType = cc.game.RENDER_TYPE_CANVAS;
        cc._supportRender = true;
    }
    else if (userRenderMode === 2 && cc.sys.capabilities['opengl']) {
        cc._renderType = cc.game.RENDER_TYPE_WEBGL;
        cc._supportRender = true;
    }
}

function _afterEngineLoaded() {
    cc._engineLoaded = true;
    console.log('Cocos Creator v' + cc.ENGINE_VERSION);
    if (_engineLoadedCallback) _engineLoadedCallback();
}

function _windowLoaded () {
    window.removeEventListener('load', _windowLoaded, false);
    _afterEngineLoaded();
}

cc.initEngine = function (config, cb) {
    if (_engineInitCalled) {
        var previousCallback = _engineLoadedCallback;
        _engineLoadedCallback = function () {
            previousCallback && previousCallback();
            cb && cb();
        };
        return;
    }

    _engineLoadedCallback = cb;

    // Config uninitialized and given, initialize with it
    if (!cc.game.config && config) {
        cc.game.config = config;
    }
    // No config given and no config set before, load it
    else if (!cc.game.config) {
        cc.game._loadConfig();
    }
    config = cc.game.config;

    _determineRenderType(config);

    document.body ? _afterEngineLoaded() : window.addEventListener('load', _windowLoaded, false);
    _engineInitCalled = true;
};
