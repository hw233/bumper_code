/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2011-2012 cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

var PNGReader = require('./CCPNGReader');
var tiffReader = require('./CCTIFFReader');
require('../core/platform/CCSAXParser.js');
require('../compression/ZipUtils');

// ideas taken from:
//   . The ocean spray in your face [Jeff Lander]
//      http://www.double.co.nz/dust/col0798.pdf
//   . Building an Advanced Particle System [John van der Burg]
//      http://www.gamasutra.com/features/20000623/vanderburg_01.htm
//   . LOVE game engine
//      http://love2d.org/
//
//
// Radius mode support, from 71 squared
//      http://particledesigner.71squared.com/
//
// IMPORTANT: Particle Designer is supported by cocos2d, but
// 'Radius Mode' in Particle Designer uses a fixed emit rate of 30 hz. Since that can't be guarateed in cocos2d,
//  cocos2d uses a another approach, but the results are almost identical.
//


// tCCPositionType
// possible types of particle positions


/**
 * Structure that contains the values of each particle
 * @Class
 * @Construct
 * @param {cc.Vec2} [pos=cc.p(0,0)] Position of particle
 * @param {cc.Vec2} [startPos=cc.p(0,0)]
 * @param {cc.Color} [color= cc.color(0, 0, 0, 255)]
 * @param {cc.Color} [deltaColor=cc.color(0, 0, 0, 255)]
 * @param {Size} [size=0]
 * @param {Size} [deltaSize=0]
 * @param {Number} [rotation=0]
 * @param {Number} [deltaRotation=0]
 * @param {Number} [timeToLive=0]
 * @param {cc.Particle.ModeA} [modeA=]
 * @param {cc.Particle.ModeA} [modeB=]
 */
cc.Particle = function (pos, startPos, color, deltaColor, size, deltaSize, rotation, deltaRotation, timeToLive, modeA, modeB) {
    this.pos = pos || cc.v2(0, 0);
    this.startPos = startPos || cc.v2(0, 0);
    this.color = color || cc.color(0, 0, 0, 255);
    this.deltaColor = deltaColor || {r:0, g: 0, b:0, a:255};
    this.size = size || 0;
    this.deltaSize = deltaSize || 0;
    this.rotation = rotation || 0;
    this.deltaRotation = deltaRotation || 0;
    this.timeToLive = timeToLive || 0;
    this.modeA = modeA ? modeA : new cc.Particle.ModeA();
    this.modeB = modeB ? modeB : new cc.Particle.ModeB();
    this.isChangeColor = false;
    this.drawPos = cc.p(0, 0);
};

/**
 * Mode A: gravity, direction, radial accel, tangential accel
 * @Class
 * @Construct
 * @param {cc.Vec2} dir direction of particle
 * @param {Number} radialAccel
 * @param {Number} tangentialAccel
 */
cc.Particle.ModeA = function (dir, radialAccel, tangentialAccel) {
    this.dir = dir ? dir : cc.p(0,0);
    this.radialAccel = radialAccel || 0;
    this.tangentialAccel = tangentialAccel || 0;
};

/**
 * Mode B: radius mode
 * @Class
 * @Construct
 * @param {Number} angle
 * @param {Number} degreesPerSecond
 * @param {Number} radius
 * @param {Number} deltaRadius
 */
cc.Particle.ModeB = function (angle, degreesPerSecond, radius, deltaRadius) {
    this.angle = angle || 0;
    this.degreesPerSecond = degreesPerSecond || 0;
    this.radius = radius || 0;
    this.deltaRadius = deltaRadius || 0;
};

/**
 * Array of Point instances used to optimize particle updates
 */
cc.Particle.TemporaryPoints = [
    cc.p(),
    cc.p(),
    cc.p(),
    cc.p()
];

/**
 * <p>
 *     Particle System base class. <br/>
 *     Attributes of a Particle System:<br/>
 *     - emission rate of the particles<br/>
 *     - Gravity Mode (Mode A): <br/>
 *     - gravity <br/>
 *     - direction <br/>
 *     - speed +-  variance <br/>
 *     - tangential acceleration +- variance<br/>
 *     - radial acceleration +- variance<br/>
 *     - Radius Mode (Mode B):      <br/>
 *     - startRadius +- variance    <br/>
 *     - endRadius +- variance      <br/>
 *     - rotate +- variance         <br/>
 *     - Properties common to all modes: <br/>
 *     - life +- life variance      <br/>
 *     - start spin +- variance     <br/>
 *     - end spin +- variance       <br/>
 *     - start size +- variance     <br/>
 *     - end size +- variance       <br/>
 *     - start color +- variance    <br/>
 *     - end color +- variance      <br/>
 *     - life +- variance           <br/>
 *     - blending function          <br/>
 *     - texture                    <br/>
 *                                  <br/>
 *     cocos2d also supports particles generated by Particle Designer (http://particledesigner.71squared.com/).<br/>
 *     'Radius Mode' in Particle Designer uses a fixed emit rate of 30 hz. Since that can't be guarateed in cocos2d,  <br/>
 *     cocos2d uses a another approach, but the results are almost identical.<br/>
 *     cocos2d supports all the variables used by Particle Designer plus a bit more:  <br/>
 *     - spinning particles (supported when using ParticleSystem)       <br/>
 *     - tangential acceleration (Gravity mode)                               <br/>
 *     - radial acceleration (Gravity mode)                                   <br/>
 *     - radius direction (Radius mode) (Particle Designer supports outwards to inwards direction only) <br/>
 *     It is possible to customize any of the above mentioned properties in runtime. Example:   <br/>
 * </p>
 * @class _ccsg.ParticleSystem
 * @extends _ccsg.Node
 *
 * @property {Boolean}              opacityModifyRGB    - Indicate whether the alpha value modify color.
 * @property {Boolean}              active              - <@readonly> Indicate whether the particle system is activated.
 * @property {Number}               particleCount       - Current quantity of particles that are being simulated.
 * @property {Number}               duration            - How many seconds the emitter wil run. -1 means 'forever'
 * @property {cc.Vec2}             sourcePos           - Source position of the emitter.
 * @property {cc.Vec2}             posVar              - Variation of source position.
 * @property {Number}               life                - Life of each particle setter.
 * @property {Number}               lifeVar             - Variation of life.
 * @property {Number}               angle               - Angle of each particle setter.
 * @property {Number}               angleVar            - Variation of angle of each particle setter.
 * @property {Number}               startSize           - Start size in pixels of each particle.
 * @property {Number}               startSizeVar        - Variation of start size in pixels.
 * @property {Number}               endSize             - End size in pixels of each particle.
 * @property {Number}               endSizeVar          - Variation of end size in pixels.
 * @property {Number}               startSpin           - Start angle of each particle.
 * @property {Number}               startSpinVar        - Variation of start angle.
 * @property {Number}               endSpin             - End angle of each particle.
 * @property {Number}               endSpinVar          - Variation of end angle.
 * @property {cc.Vec2}             gravity             - Gravity of the emitter.
 * @property {cc.Vec2}             speed               - Speed of the emitter.
 * @property {cc.Vec2}             speedVar            - Variation of the speed.
 * @property {Number}               tangentialAccel     - Tangential acceleration of each particle. Only available in 'Gravity' mode.
 * @property {Number}               tangentialAccelVar  - Variation of the tangential acceleration.
 * @property {Number}               tangentialAccel     - Radial acceleration of each particle. Only available in 'Gravity' mode.
 * @property {Number}               tangentialAccelVar  - Variation of the radial acceleration.
 * @property {Boolean}              rotationIsDir       - Indicate whether the rotation of each particle equals to its direction. Only available in 'Gravity' mode.
 * @property {Number}               startRadius         - Starting radius of the particles. Only available in 'Radius' mode.
 * @property {Number}               startRadiusVar      - Variation of the starting radius.
 * @property {Number}               endRadius           - Ending radius of the particles. Only available in 'Radius' mode.
 * @property {Number}               endRadiusVar        - Variation of the ending radius.
 * @property {Number}               rotatePerS          - Number of degress to rotate a particle around the source pos per second. Only available in 'Radius' mode.
 * @property {Number}               rotatePerSVar       - Variation of the degress to rotate a particle around the source pos per second.
 * @property {cc.Color}             startColor          - Start color of each particle.
 * @property {cc.Color}             startColorVar       - Variation of the start color.
 * @property {cc.Color}             endColor            - Ending color of each particle.
 * @property {cc.Color}             endColorVar         - Variation of the end color.
 * @property {Number}               emissionRate        - Emission rate of the particles.
 * @property {ccsg.ParticleSystem.Mode} emitterMode       - Emitter modes: ccsg.ParticleSystem.Mode.GRAVITY: uses gravity, speed, radial and tangential acceleration; ccsg.ParticleSystem.Mode.RADIUS: uses radius movement + rotation.
 * @property {ccsg.ParticleSystem.Type} positionType      - Particles movement type: ccsg.ParticleSystem.Type.FREE | ccsg.ParticleSystem.Type.GROUPED.
 * @property {Number}               totalParticles      - Maximum particles of the system.
 * @property {Boolean}              autoRemoveOnFinish  - Indicate whether the node will be auto-removed when it has no particles left.
 * @property {Texture2D}         texture             - Texture of Particle System.
 *
 * @example
 *  emitter.radialAccel = 15;
 *  emitter.startSpin = 0;
 */
_ccsg.ParticleSystem = _ccsg.Node.extend({
    _className: "ParticleSystem",
    //***********variables*************
    _plistFile: "",
    //! time elapsed since the start of the system (in seconds)
    _elapsed: 0,
    _dontTint: false,

    // Different modes
    //! Mode A:Gravity + Tangential Accel + Radial Accel
    modeA: null,
    //! Mode B: circular movement (gravity, radial accel and tangential accel don't are not used in this mode)
    modeB: null,

    //private POINTZERO for ParticleSystem
    _pointZeroForParticle: cc.p(0, 0),

    //! Array of particles
    _particles: null,

    // color modulate
    //  BOOL colorModulate;

    //! How many particles can be emitted per second
    _emitCounter: 0,
    //!  particle idx
    _particleIdx: 0,

    _allocatedParticles: 0,

    _isActive: false,
    particleCount: 0,
    duration: 0,
    _sourcePosition: null,
    _posVar: null,
    life: 0,
    lifeVar: 0,
    angle: 0,
    angleVar: 0,
    startSize: 0,
    startSizeVar: 0,
    endSize: 0,
    endSizeVar: 0,
    _startColor: null,
    _startColorVar: null,
    _endColor: null,
    _endColorVar: null,
    startSpin: 0,
    startSpinVar: 0,
    endSpin: 0,
    endSpinVar: 0,
    emissionRate: 0,
    _totalParticles: 0,
    _texture: null,
    _blendFunc: null,
    _opacityModifyRGB: false,
    positionType: null,
    autoRemoveOnFinish: false,
    emitterMode: 0,

    _textureLoaded: null,

    /**
     * <p> return the string found by key in dict. <br/>
     *    This plist files can be create manually or with Particle Designer:<br/>
     *    http://particledesigner.71squared.com/<br/>
     * </p>
     * Constructor of ccsg.ParticleSystem
     * @param {String|Number} plistFile
     */
    ctor:function (plistFile) {
        _ccsg.Node.prototype.ctor.call(this);
        this.emitterMode = _ccsg.ParticleSystem.Mode.GRAVITY;
        this.modeA = new _ccsg.ParticleSystem.ModeA();
        this.modeB = new _ccsg.ParticleSystem.ModeB();
        this._blendFunc = {src:cc.macro.BLEND_SRC, dst:cc.macro.BLEND_DST};

        this._particles = [];
        this._sourcePosition = cc.p(0, 0);
        this._posVar = cc.p(0, 0);

        this._startColor = cc.color(255, 255, 255, 255);
        this._startColorVar = cc.color(255, 255, 255, 255);
        this._endColor = cc.color(255, 255, 255, 255);
        this._endColorVar = cc.color(255, 255, 255, 255);

        this._plistFile = "";
        this._elapsed = 0;
        this._dontTint = false;
        this._pointZeroForParticle = cc.p(0, 0);
        this._emitCounter = 0;
        this._particleIdx = 0;

        this._allocatedParticles = 0;
        this._isActive = false;
        this.particleCount = 0;
        this.duration = 0;
        this.life = 0;
        this.lifeVar = 0;
        this.angle = 0;
        this.angleVar = 0;
        this.startSize = 0;
        this.startSizeVar = 0;
        this.endSize = 0;
        this.endSizeVar = 0;

        this.startSpin = 0;
        this.startSpinVar = 0;
        this.endSpin = 0;
        this.endSpinVar = 0;
        this.emissionRate = 0;
        this._totalParticles = 0;
        this._texture = null;
        this._opacityModifyRGB = false;
        this.positionType = _ccsg.ParticleSystem.Type.FREE;
        this.autoRemoveOnFinish = false;

        this._textureLoaded = true;

        if (!plistFile || cc.js.isNumber(plistFile)) {
            var ton = plistFile || 100;
            this.initWithTotalParticles(ton);
        } else if (typeof plistFile === 'string') {
            this.initWithFile(plistFile);
        } else if (typeof plistFile === 'object') {
            this.initWithDictionary(plistFile, "");
        }
    },

    _createRenderCmd: function () {
        if(cc._renderType === cc.game.RENDER_TYPE_CANVAS)
            return new _ccsg.ParticleSystem.CanvasRenderCmd(this);
        else
            return new _ccsg.ParticleSystem.WebGLRenderCmd(this);
    },
    onEnter: function () {
        // udpate after action in run!
        this.scheduleUpdateWithPriority(1);
        _ccsg.Node.prototype.onEnter.call(this);
    },
    
    /**
     * This is a hack function for performance, it's only available on Canvas mode. <br/>
     * It's very expensive to change color on Canvas mode, so if set it to true, particle system will ignore the changing color operation.
     * @param {boolean} ignore
     */
    ignoreColor: function(ignore){
       this._dontTint = ignore;
    },

    /**
     * <p> initializes the texture with a rectangle measured Points<br/>
     * pointRect should be in Texture coordinates, not pixel coordinates
     * </p>
     * @param {Rect} pointRect
     */
    initTexCoordsWithRect:function (pointRect) {
        this._renderCmd.initTexCoordsWithRect(pointRect);
    },

    /**
     * Return ParticleSystem is active
     * @return {Boolean}
     */
    isActive:function () {
        return this._isActive;
    },

    /**
     * Quantity of particles that are being simulated at the moment
     * @return {Number}
     */
    getParticleCount:function () {
        return this.particleCount;
    },

    /**
     * Quantity of particles setter
     * @param {Number} particleCount
     */
    setParticleCount:function (particleCount) {
        this.particleCount = particleCount;
    },

    /**
     * How many seconds the emitter wil run. -1 means 'forever'
     * @return {Number}
     */
    getDuration:function () {
        return this.duration;
    },

    /**
     * set run seconds of the emitter
     * @param {Number} duration
     */
    setDuration:function (duration) {
        this.duration = duration;
    },

    /**
     * Return sourcePosition of the emitter
     * @return {cc.Vec2 | Object}
     */
    getSourcePosition:function () {
        return {x: this._sourcePosition.x, y: this._sourcePosition.y};
    },

    /**
     * sourcePosition of the emitter setter
     * @param sourcePosition
     */
    setSourcePosition: function (sourcePosition) {
        this._sourcePosition.x = sourcePosition.x;
        this._sourcePosition.y = sourcePosition.y;
    },

    /**
     * Return Position variance of the emitter
     * @return {cc.Vec2 | Object}
     */
    getPosVar:function () {
        return {x: this._posVar.x, y: this._posVar.y};
    },

    /**
     * Position variance of the emitter setter
     * @param {cc.Vec2} posVar
     */
    setPosVar: function (posVar) {
        this._posVar.x = posVar.x;
        this._posVar.y = posVar.y;
    },

    /**
     * Return life of each particle
     * @return {Number}
     */
    getLife:function () {
        return this.life;
    },

    /**
     * life of each particle setter
     * @param {Number} life
     */
    setLife:function (life) {
        this.life = life;
    },

    /**
     * Return life variance of each particle
     * @return {Number}
     */
    getLifeVar:function () {
        return this.lifeVar;
    },

    /**
     * life variance of each particle setter
     * @param {Number} lifeVar
     */
    setLifeVar:function (lifeVar) {
        this.lifeVar = lifeVar;
    },

    /**
     * Return angle of each particle
     * @return {Number}
     */
    getAngle:function () {
        return this.angle;
    },

    /**
     * angle of each particle setter
     * @param {Number} angle
     */
    setAngle:function (angle) {
        this.angle = angle;
    },

    /**
     * Return angle variance of each particle
     * @return {Number}
     */
    getAngleVar:function () {
        return this.angleVar;
    },

    /**
     * angle variance of each particle setter
     * @param angleVar
     */
    setAngleVar:function (angleVar) {
        this.angleVar = angleVar;
    },

    // mode A
    /**
     * Return Gravity of emitter
     * @return {cc.Vec2}
     */
    getGravity:function () {
        var locGravity = this.modeA.gravity;
        return cc.p(locGravity.x, locGravity.y);
    },

    /**
     * Gravity of emitter setter
     * @param {cc.Vec2} gravity
     */
    setGravity:function (gravity) {
        this.modeA.gravity = gravity;
    },

    /**
     * Return Speed of each particle
     * @return {Number}
     */
    getSpeed:function () {
        return this.modeA.speed;
    },

    /**
     * Speed of each particle setter
     * @param {Number} speed
     */
    setSpeed:function (speed) {
        this.modeA.speed = speed;
    },

    /**
     * return speed variance of each particle. Only available in 'Gravity' mode.
     * @return {Number}
     */
    getSpeedVar:function () {
        return this.modeA.speedVar;
    },

    /**
     * speed variance of each particle setter. Only available in 'Gravity' mode.
     * @param {Number} speedVar
     */
    setSpeedVar:function (speedVar) {
        this.modeA.speedVar = speedVar;
    },

    /**
     * Return tangential acceleration of each particle. Only available in 'Gravity' mode.
     * @return {Number}
     */
    getTangentialAccel:function () {
        return this.modeA.tangentialAccel;
    },

    /**
     * Tangential acceleration of each particle setter. Only available in 'Gravity' mode.
     * @param {Number} tangentialAccel
     */
    setTangentialAccel:function (tangentialAccel) {
        this.modeA.tangentialAccel = tangentialAccel;
    },

    /**
     * Return tangential acceleration variance of each particle. Only available in 'Gravity' mode.
     * @return {Number}
     */
    getTangentialAccelVar:function () {
        return this.modeA.tangentialAccelVar;
    },

    /**
     * tangential acceleration variance of each particle setter. Only available in 'Gravity' mode.
     * @param {Number} tangentialAccelVar
     */
    setTangentialAccelVar:function (tangentialAccelVar) {
        this.modeA.tangentialAccelVar = tangentialAccelVar;
    },

    /**
     * Return radial acceleration of each particle. Only available in 'Gravity' mode.
     * @return {Number}
     */
    getRadialAccel:function () {
        return this.modeA.radialAccel;
    },

    /**
     * radial acceleration of each particle setter. Only available in 'Gravity' mode.
     * @param {Number} radialAccel
     */
    setRadialAccel:function (radialAccel) {
        this.modeA.radialAccel = radialAccel;
    },

    /**
     * Return radial acceleration variance of each particle. Only available in 'Gravity' mode.
     * @return {Number}
     */
    getRadialAccelVar:function () {
        return this.modeA.radialAccelVar;
    },

    /**
     * radial acceleration variance of each particle setter. Only available in 'Gravity' mode.
     * @param {Number} radialAccelVar
     */
    setRadialAccelVar:function (radialAccelVar) {
        this.modeA.radialAccelVar = radialAccelVar;
    },

    /**
     * get the rotation of each particle to its direction Only available in 'Gravity' mode.
     * @returns {boolean}
     */
    getRotationIsDir: function(){
        return this.modeA.rotationIsDir;
    },

    /**
     * set the rotation of each particle to its direction Only available in 'Gravity' mode.
     * @param {boolean} t
     */
    setRotationIsDir: function(t){
        this.modeA.rotationIsDir = t;
    },

    // mode B
    /**
     * Return starting radius of the particles. Only available in 'Radius' mode.
     * @return {Number}
     */
    getStartRadius:function () {
        return this.modeB.startRadius;
    },

    /**
     * starting radius of the particles setter. Only available in 'Radius' mode.
     * @param {Number} startRadius
     */
    setStartRadius:function (startRadius) {
        this.modeB.startRadius = startRadius;
    },

    /**
     * Return starting radius variance of the particles. Only available in 'Radius' mode.
     * @return {Number}
     */
    getStartRadiusVar:function () {
        return this.modeB.startRadiusVar;
    },

    /**
     * starting radius variance of the particles setter. Only available in 'Radius' mode.
     * @param {Number} startRadiusVar
     */
    setStartRadiusVar:function (startRadiusVar) {
        this.modeB.startRadiusVar = startRadiusVar;
    },

    /**
     * Return ending radius of the particles. Only available in 'Radius' mode.
     * @return {Number}
     */
    getEndRadius:function () {
        return this.modeB.endRadius;
    },

    /**
     * ending radius of the particles setter. Only available in 'Radius' mode.
     * @param {Number} endRadius
     */
    setEndRadius:function (endRadius) {
        this.modeB.endRadius = endRadius;
    },

    /**
     * Return ending radius variance of the particles. Only available in 'Radius' mode.
     * @return {Number}
     */
    getEndRadiusVar:function () {
        return this.modeB.endRadiusVar;
    },

    /**
     * ending radius variance of the particles setter. Only available in 'Radius' mode.
     * @param endRadiusVar
     */
    setEndRadiusVar:function (endRadiusVar) {
        this.modeB.endRadiusVar = endRadiusVar;
    },

    /**
     * get Number of degress to rotate a particle around the source pos per second. Only available in 'Radius' mode.
     * @return {Number}
     */
    getRotatePerSecond:function () {
        return this.modeB.rotatePerSecond;
    },

    /**
     * set Number of degress to rotate a particle around the source pos per second. Only available in 'Radius' mode.
     * @param {Number} degrees
     */
    setRotatePerSecond:function (degrees) {
        this.modeB.rotatePerSecond = degrees;
    },

    /**
     * Return Variance in degrees for rotatePerSecond. Only available in 'Radius' mode.
     * @return {Number}
     */
    getRotatePerSecondVar:function () {
        return this.modeB.rotatePerSecondVar;
    },

    /**
     * Variance in degrees for rotatePerSecond setter. Only available in 'Radius' mode.
     * @param degrees
     */
    setRotatePerSecondVar:function (degrees) {
        this.modeB.rotatePerSecondVar = degrees;
    },
    //////////////////////////////////////////////////////////////////////////

    /**
     * get start size in pixels of each particle
     * @return {Number}
     */
    getStartSize:function () {
        return this.startSize;
    },

    /**
     * set start size in pixels of each particle
     * @param {Number} startSize
     */
    setStartSize:function (startSize) {
        this.startSize = startSize;
    },

    /**
     * get size variance in pixels of each particle
     * @return {Number}
     */
    getStartSizeVar:function () {
        return this.startSizeVar;
    },

    /**
     * set size variance in pixels of each particle
     * @param {Number} startSizeVar
     */
    setStartSizeVar:function (startSizeVar) {
        this.startSizeVar = startSizeVar;
    },

    /**
     * get end size in pixels of each particle
     * @return {Number}
     */
    getEndSize:function () {
        return this.endSize;
    },

    /**
     * set end size in pixels of each particle
     * @param endSize
     */
    setEndSize:function (endSize) {
        this.endSize = endSize;
    },

    /**
     * get end size variance in pixels of each particle
     * @return {Number}
     */
    getEndSizeVar:function () {
        return this.endSizeVar;
    },

    /**
     * set end size variance in pixels of each particle
     * @param {Number} endSizeVar
     */
    setEndSizeVar:function (endSizeVar) {
        this.endSizeVar = endSizeVar;
    },

    /**
     * set start color of each particle
     * @return {cc.Color}
     */
    getStartColor:function () {
        return cc.color(this._startColor.r, this._startColor.g, this._startColor.b, this._startColor.a);
    },

    /**
     * get start color of each particle
     * @param {cc.Color} startColor
     */
    setStartColor: function (startColor) {
        this._startColor.fromColor(startColor);
    },

    /**
     * get start color variance of each particle
     * @return {cc.Color}
     */
    getStartColorVar:function () {
        return cc.color(this._startColorVar.r, this._startColorVar.g, this._startColorVar.b, this._startColorVar.a);
    },

    /**
     * set start color variance of each particle
     * @param {cc.Color} startColorVar
     */
    setStartColorVar:function (startColorVar) {
        this._startColorVar.fromColor(startColorVar);
    },

    /**
     * get end color and end color variation of each particle
     * @return {cc.Color}
     */
    getEndColor:function () {
        return cc.color(this._endColor.r, this._endColor.g, this._endColor.b, this._endColor.a);
    },

    /**
     * set end color and end color variation of each particle
     * @param {cc.Color} endColor
     */
    setEndColor:function (endColor) {
        this._endColor.fromColor(endColor);
    },

    /**
     * get end color variance of each particle
     * @return {cc.Color}
     */
    getEndColorVar:function () {
        return cc.color(this._endColorVar.r, this._endColorVar.g, this._endColorVar.b, this._endColorVar.a);
    },

    /**
     * set end color variance of each particle
     * @param {cc.Color} endColorVar
     */
    setEndColorVar:function (endColorVar) {
        this._endColorVar.fromColor(endColorVar);
    },

    /**
     * get initial angle of each particle
     * @return {Number}
     */
    getStartSpin:function () {
        return this.startSpin;
    },

    /**
     * set initial angle of each particle
     * @param {Number} startSpin
     */
    setStartSpin:function (startSpin) {
        this.startSpin = startSpin;
    },

    /**
     * get initial angle variance of each particle
     * @return {Number}
     */
    getStartSpinVar:function () {
        return this.startSpinVar;
    },

    /**
     * set initial angle variance of each particle
     * @param {Number} startSpinVar
     */
    setStartSpinVar:function (startSpinVar) {
        this.startSpinVar = startSpinVar;
    },

    /**
     * get end angle of each particle
     * @return {Number}
     */
    getEndSpin:function () {
        return this.endSpin;
    },

    /**
     * set end angle of each particle
     * @param {Number} endSpin
     */
    setEndSpin:function (endSpin) {
        this.endSpin = endSpin;
    },

    /**
     * get end angle variance of each particle
     * @return {Number}
     */
    getEndSpinVar:function () {
        return this.endSpinVar;
    },

    /**
     * set end angle variance of each particle
     * @param {Number} endSpinVar
     */
    setEndSpinVar:function (endSpinVar) {
        this.endSpinVar = endSpinVar;
    },

    /**
     * get emission rate of the particles
     * @return {Number}
     */
    getEmissionRate:function () {
        return this.emissionRate;
    },

    /**
     * set emission rate of the particles
     * @param {Number} emissionRate
     */
    setEmissionRate:function (emissionRate) {
        this.emissionRate = emissionRate;
    },

    /**
     * get maximum particles of the system
     * @return {Number}
     */
    getTotalParticles:function () {
        return this._totalParticles;
    },

    /**
     * set maximum particles of the system
     * @param {Number} tp totalParticles
     */
    setTotalParticles:function (tp) {
        this._renderCmd.setTotalParticles(tp);
    },

    /**
     * get Texture of Particle System
     * @return {Texture2D}
     */
    getTexture:function () {
        return this._texture;
    },

    /**
     * set Texture of Particle System
     * @param {cc.Texture2D } texture
     */
    setTexture:function (texture) {
        if(!texture)
            return;

        if(texture.loaded){
            this.setTextureWithRect(texture, cc.rect(0, 0, texture.width, texture.height));
        } else {
            this._textureLoaded = false;
            texture.once("load", function (event) {
                this._textureLoaded = true;
                this.setTextureWithRect(texture, cc.rect(0, 0, texture.width, texture.height));
            }, this);
        }
    },

    /** conforms to CocosNodeTexture protocol */
    /**
     * get BlendFunc of Particle System
     * @return {cc.BlendFunc}
     */
    getBlendFunc:function () {
        return this._blendFunc;
    },

    /**
     * set BlendFunc of Particle System
     * @param {Number} src
     * @param {Number} dst
     */
    setBlendFunc:function (src, dst) {
        if (dst === undefined) {
            if (this._blendFunc !== src) {
                this._blendFunc = src;
                this._updateBlendFunc();
            }
        } else {
            if (this._blendFunc.src !== src || this._blendFunc.dst !== dst) {
                this._blendFunc = {src:src, dst:dst};
                this._updateBlendFunc();
            }
        }
    },

    /**
     * does the alpha value modify color getter
     * @return {Boolean}
     */
    isOpacityModifyRGB:function () {
        return this._opacityModifyRGB;
    },

    /**
     * does the alpha value modify color setter
     * @param newValue
     */
    setOpacityModifyRGB:function (newValue) {
        this._opacityModifyRGB = newValue;
    },

    /**
     * <p>whether or not the particles are using blend additive.<br/>
     *     If enabled, the following blending function will be used.<br/>
     * </p>
     * @return {Boolean}
     * @example
     *    source blend function = GL_SRC_ALPHA;
     *    dest blend function = GL_ONE;
     */
    isBlendAdditive:function () {
        return (( this._blendFunc.src === cc.macro.SRC_ALPHA && this._blendFunc.dst === cc.macro.ONE) ||
                (this._blendFunc.src === cc.macro.ONE && this._blendFunc.dst === cc.macro.ONE));
    },

    /**
     * <p>whether or not the particles are using blend additive.<br/>
     *     If enabled, the following blending function will be used.<br/>
     * </p>
     * @param {Boolean} isBlendAdditive
     */
    setBlendAdditive:function (isBlendAdditive) {
        var locBlendFunc = this._blendFunc;
        if (isBlendAdditive) {
            locBlendFunc.src = cc.macro.SRC_ALPHA;
            locBlendFunc.dst = cc.macro.ONE;
        } else {
            this._renderCmd._setBlendAdditive();
        }
    },

    /**
     * get particles movement type: Free or Grouped
     * @return {Number}
     */
    getPositionType:function () {
        return this.positionType;
    },

    /**
     * set particles movement type: Free or Grouped
     * @param {Number} positionType
     */
    setPositionType:function (positionType) {
        this.positionType = positionType;
    },

    /**
     *  <p> return whether or not the node will be auto-removed when it has no particles left.<br/>
     *      By default it is false.<br/>
     *  </p>
     * @return {Boolean}
     */
    isAutoRemoveOnFinish:function () {
        return this.autoRemoveOnFinish;
    },

    /**
     *  <p> set whether or not the node will be auto-removed when it has no particles left.<br/>
     *      By default it is false.<br/>
     *  </p>
     * @param {Boolean} isAutoRemoveOnFinish
     */
    setAutoRemoveOnFinish:function (isAutoRemoveOnFinish) {
        this.autoRemoveOnFinish = isAutoRemoveOnFinish;
    },

    /**
     * return kind of emitter modes
     * @return {Number}
     */
    getEmitterMode:function () {
        return this.emitterMode;
    },

    /**
     * <p>Switch between different kind of emitter modes:<br/>
     *  - CCParticleSystem.Mode.GRAVITY: uses gravity, speed, radial and tangential acceleration<br/>
     *  - CCParticleSystem.Mode.RADIUS: uses radius movement + rotation <br/>
     *  </p>
     * @param {Number} emitterMode
     */
    setEmitterMode:function (emitterMode) {
        this.emitterMode = emitterMode;
    },

    /**
     * initializes a ccsg.ParticleSystem
     */
    init:function () {
        return this.initWithTotalParticles(150);
    },

    /**
     * <p>
     *     initializes a CCParticleSystem from a plist file. <br/>
     *      This plist files can be creted manually or with Particle Designer:<br/>
     *      http://particledesigner.71squared.com/
     * </p>
     * @param {String} plistFile
     * @return {boolean}
     */
    initWithFile:function (plistFile) {
        this._plistFile = plistFile;
        var dict = cc.loader.getRes(plistFile);
        if(!dict){
            cc.logID(6008);
            return false;
        }

        // XXX compute path from a path, should define a function somewhere to do it
        return this.initWithDictionary(dict, "");
    },

    /**
     * return bounding box of particle system in world space
     * @return {Rect}
     */
    getBoundingBoxToWorld:function () {
        return cc.rect(0, 0, cc._canvas.width, cc._canvas.height);
    },

    /**
     * initializes a particle system from a NSDictionary and the path from where to load the png
     * @param {object} dictionary
     * @param {String} dirname
     * @return {Boolean}
     */
    initWithDictionary:function (dictionary, dirname) {
        var buffer = null;
        var image = null;
        var locValueForKey = this._valueForKey;

        var maxParticles = parseInt(locValueForKey("maxParticles", dictionary));
        // self, not super
        if (this.initWithTotalParticles(maxParticles)) {
            // angle
            this.angle = parseFloat(locValueForKey("angle", dictionary));
            this.angleVar = parseFloat(locValueForKey("angleVariance", dictionary));

            // duration
            this.duration = parseFloat(locValueForKey("duration", dictionary));

            // blend function
            this._blendFunc.src = parseInt(locValueForKey("blendFuncSource", dictionary));
            this._blendFunc.dst = parseInt(locValueForKey("blendFuncDestination", dictionary));

            // color
            var locStartColor = this._startColor;
            locStartColor.r = parseFloat(locValueForKey("startColorRed", dictionary)) * 255;
            locStartColor.g = parseFloat(locValueForKey("startColorGreen", dictionary)) * 255;
            locStartColor.b = parseFloat(locValueForKey("startColorBlue", dictionary)) * 255;
            locStartColor.a = parseFloat(locValueForKey("startColorAlpha", dictionary)) * 255;

            var locStartColorVar = this._startColorVar;
            locStartColorVar.r = parseFloat(locValueForKey("startColorVarianceRed", dictionary)) * 255;
            locStartColorVar.g = parseFloat(locValueForKey("startColorVarianceGreen", dictionary)) * 255;
            locStartColorVar.b = parseFloat(locValueForKey("startColorVarianceBlue", dictionary)) * 255;
            locStartColorVar.a = parseFloat(locValueForKey("startColorVarianceAlpha", dictionary)) * 255;

            var locEndColor = this._endColor;
            locEndColor.r = parseFloat(locValueForKey("finishColorRed", dictionary)) * 255;
            locEndColor.g = parseFloat(locValueForKey("finishColorGreen", dictionary)) * 255;
            locEndColor.b = parseFloat(locValueForKey("finishColorBlue", dictionary)) * 255;
            locEndColor.a = parseFloat(locValueForKey("finishColorAlpha", dictionary)) * 255;

            var locEndColorVar = this._endColorVar;
            locEndColorVar.r = parseFloat(locValueForKey("finishColorVarianceRed", dictionary)) * 255;
            locEndColorVar.g = parseFloat(locValueForKey("finishColorVarianceGreen", dictionary)) * 255;
            locEndColorVar.b = parseFloat(locValueForKey("finishColorVarianceBlue", dictionary)) * 255;
            locEndColorVar.a = parseFloat(locValueForKey("finishColorVarianceAlpha", dictionary)) * 255;

            // particle size
            this.startSize = parseFloat(locValueForKey("startParticleSize", dictionary));
            this.startSizeVar = parseFloat(locValueForKey("startParticleSizeVariance", dictionary));
            this.endSize = parseFloat(locValueForKey("finishParticleSize", dictionary));
            this.endSizeVar = parseFloat(locValueForKey("finishParticleSizeVariance", dictionary));

            // position
            this.setPosition(parseFloat(locValueForKey("sourcePositionx", dictionary)),
                              parseFloat(locValueForKey("sourcePositiony", dictionary)));
            this._posVar.x = parseFloat(locValueForKey("sourcePositionVariancex", dictionary));
            this._posVar.y = parseFloat(locValueForKey("sourcePositionVariancey", dictionary));

            // Spinning
            this.startSpin = parseFloat(locValueForKey("rotationStart", dictionary));
            this.startSpinVar = parseFloat(locValueForKey("rotationStartVariance", dictionary));
            this.endSpin = parseFloat(locValueForKey("rotationEnd", dictionary));
            this.endSpinVar = parseFloat(locValueForKey("rotationEndVariance", dictionary));

            this.emitterMode = parseInt(locValueForKey("emitterType", dictionary));

            // Mode A: Gravity + tangential accel + radial accel
            if (this.emitterMode === _ccsg.ParticleSystem.Mode.GRAVITY) {
                var locModeA = this.modeA;
                // gravity
                locModeA.gravity.x = parseFloat(locValueForKey("gravityx", dictionary));
                locModeA.gravity.y = parseFloat(locValueForKey("gravityy", dictionary));

                // speed
                locModeA.speed = parseFloat(locValueForKey("speed", dictionary));
                locModeA.speedVar = parseFloat(locValueForKey("speedVariance", dictionary));

                // radial acceleration
                var pszTmp = locValueForKey("radialAcceleration", dictionary);
                locModeA.radialAccel = (pszTmp) ? parseFloat(pszTmp) : 0;

                pszTmp = locValueForKey("radialAccelVariance", dictionary);
                locModeA.radialAccelVar = (pszTmp) ? parseFloat(pszTmp) : 0;

                // tangential acceleration
                pszTmp = locValueForKey("tangentialAcceleration", dictionary);
                locModeA.tangentialAccel = (pszTmp) ? parseFloat(pszTmp) : 0;

                pszTmp = locValueForKey("tangentialAccelVariance", dictionary);
                locModeA.tangentialAccelVar = (pszTmp) ? parseFloat(pszTmp) : 0;

                // rotation is dir
                var locRotationIsDir = locValueForKey("rotationIsDir", dictionary);
                if (locRotationIsDir !== null) {
                    locRotationIsDir = locRotationIsDir.toString().toLowerCase();
                    locModeA.rotationIsDir = (locRotationIsDir === "true" || locRotationIsDir === "1");
                }
                else {
                    locModeA.rotationIsDir = false;
                }
            } else if (this.emitterMode === _ccsg.ParticleSystem.Mode.RADIUS) {
                // or Mode B: radius movement
                var locModeB = this.modeB;
                locModeB.startRadius = parseFloat(locValueForKey("maxRadius", dictionary));
                locModeB.startRadiusVar = parseFloat(locValueForKey("maxRadiusVariance", dictionary));
                locModeB.endRadius = parseFloat(locValueForKey("minRadius", dictionary));
                locModeB.endRadiusVar = 0;
                locModeB.rotatePerSecond = parseFloat(locValueForKey("rotatePerSecond", dictionary));
                locModeB.rotatePerSecondVar = parseFloat(locValueForKey("rotatePerSecondVariance", dictionary));
            } else {
                cc.logID(6009);
                return false;
            }

            // life span
            this.life = parseFloat(locValueForKey("particleLifespan", dictionary));
            this.lifeVar = parseFloat(locValueForKey("particleLifespanVariance", dictionary));

            // emission Rate
            this.emissionRate = this._totalParticles / this.life;

            // Set a compatible default for the alpha transfer
            this._opacityModifyRGB = false;

            // texture
            // Try to get the texture from the cache
            var textureName = locValueForKey("textureFileName", dictionary);
            var imgPath = textureName && cc.path.changeBasename(this._plistFile, textureName);
            var tex = imgPath && cc.textureCache.getTextureForKey(imgPath);

            if (tex) {
                this.setTexture(tex);
            } else {
                var textureData = locValueForKey("textureImageData", dictionary);

                if (!textureData || textureData.length === 0) {
                    if (imgPath) {
                        tex = cc.textureCache.addImage(imgPath);
                        if (!tex)
                            return false;
                        this.setTexture(tex);
                    }
                    return true;
                } else {
                    buffer = cc.Codec.unzipBase64AsArray(textureData, 1);
                    if (!buffer) {
                        cc.logID(6010);
                        return false;
                    }

                    var imageFormat = cc.getImageFormatByData(buffer);

                    if(imageFormat !== cc.ImageFormat.TIFF && imageFormat !== cc.ImageFormat.PNG){
                        cc.logID(6011);
                        return false;
                    }

                    var canvasObj = document.createElement("canvas");
                    if(imageFormat === cc.ImageFormat.PNG){
                        var myPngObj = new PNGReader(buffer);
                        myPngObj.render(canvasObj);
                    } else {
                        tiffReader.parseTIFF(buffer,canvasObj);
                    }

                    imgPath = imgPath || this._plistFile;
                    cc.textureCache.cacheImage(imgPath, canvasObj);

                    var addTexture = cc.textureCache.getTextureForKey(imgPath);
                    if(!addTexture)
                        cc.logID(6012);
                    this.setTexture(addTexture);
                }
            }
            return true;
        }
        return false;
    },

    /**
     * Initializes a system with a fixed number of particles
     * @param {Number} numberOfParticles
     * @return {Boolean}
     */
    initWithTotalParticles:function (numberOfParticles) {
        this._totalParticles = numberOfParticles;

        var locParticles = this._particles;
        if (!locParticles) {
            cc.logID(6013);
            return false;
        }
        locParticles.length = numberOfParticles;
        for(var i = 0; i < numberOfParticles; i++){
            locParticles[i] = new cc.Particle();
        }

        this._allocatedParticles = numberOfParticles;

        // default, active
        this._isActive = true;

        // default blend function
        this._blendFunc.src = cc.macro.BLEND_SRC;
        this._blendFunc.dst = cc.macro.BLEND_DST;

        // default movement type;
        this.positionType = _ccsg.ParticleSystem.Type.FREE;

        // by default be in mode A:
        this.emitterMode = _ccsg.ParticleSystem.Mode.GRAVITY;

        // default: modulate
        // XXX: not used
        //  colorModulate = YES;
        this.autoRemoveOnFinish = false;

        this._renderCmd._initWithTotalParticles(numberOfParticles);
        return true;
    },
    
    /**
     * Add a particle to the emitter
     * @return {Boolean}
     */
    addParticle: function () {
        if (this.isFull())
            return false;

        var particle = this._renderCmd.addParticle();
        this.initParticle(particle);
        ++this.particleCount;
        return true;
    },

    /**
     * Initializes a particle
     * @param {cc.Particle} particle
     */
    initParticle:function (particle) {
        var locRandomMinus11 = cc.randomMinus1To1;
        // timeToLive
        // no negative life. prevent division by 0
        particle.timeToLive = this.life + this.lifeVar * locRandomMinus11();
        particle.timeToLive = Math.max(0, particle.timeToLive);

        // position
        particle.pos.x = this._sourcePosition.x + this._posVar.x * locRandomMinus11();
        particle.pos.y = this._sourcePosition.y + this._posVar.y * locRandomMinus11();

        // Color
        var start, end;
        var locStartColor = this._startColor, locStartColorVar = this._startColorVar;
        var locEndColor = this._endColor, locEndColorVar = this._endColorVar;
        start = {
            r: cc.clampf(locStartColor.r + locStartColorVar.r * locRandomMinus11(), 0, 255),
            g: cc.clampf(locStartColor.g + locStartColorVar.g * locRandomMinus11(), 0, 255),
            b: cc.clampf(locStartColor.b + locStartColorVar.b * locRandomMinus11(), 0, 255),
            a: cc.clampf(locStartColor.a + locStartColorVar.a * locRandomMinus11(), 0, 255)
        };
        end = {
            r: cc.clampf(locEndColor.r + locEndColorVar.r * locRandomMinus11(), 0, 255),
            g: cc.clampf(locEndColor.g + locEndColorVar.g * locRandomMinus11(), 0, 255),
            b: cc.clampf(locEndColor.b + locEndColorVar.b * locRandomMinus11(), 0, 255),
            a: cc.clampf(locEndColor.a + locEndColorVar.a * locRandomMinus11(), 0, 255)
        };

        particle.color = start;
        var locParticleDeltaColor = particle.deltaColor, locParticleTimeToLive = particle.timeToLive;
        locParticleDeltaColor.r = (end.r - start.r) / locParticleTimeToLive;
        locParticleDeltaColor.g = (end.g - start.g) / locParticleTimeToLive;
        locParticleDeltaColor.b = (end.b - start.b) / locParticleTimeToLive;
        locParticleDeltaColor.a = (end.a - start.a) / locParticleTimeToLive;

        // size
        var startS = this.startSize + this.startSizeVar * locRandomMinus11();
        startS = Math.max(0, startS); // No negative value

        particle.size = startS;
        if (this.endSize === _ccsg.ParticleSystem.START_SIZE_EQUAL_TO_END_SIZE) {
            particle.deltaSize = 0;
        } else {
            var endS = this.endSize + this.endSizeVar * locRandomMinus11();
            endS = Math.max(0, endS); // No negative values
            particle.deltaSize = (endS - startS) / locParticleTimeToLive;
        }

        // rotation
        var startA = this.startSpin + this.startSpinVar * locRandomMinus11();
        var endA = this.endSpin + this.endSpinVar * locRandomMinus11();
        particle.rotation = startA;
        particle.deltaRotation = (endA - startA) / locParticleTimeToLive;

        // position
        if (this.positionType === _ccsg.ParticleSystem.Type.FREE)
            particle.startPos = this.convertToWorldSpace(this._pointZeroForParticle);
        else if (this.positionType === _ccsg.ParticleSystem.Type.RELATIVE){
            particle.startPos.x = this._position.x;
            particle.startPos.y = this._position.y;
        }

        // direction
        var a = cc.degreesToRadians(this.angle + this.angleVar * locRandomMinus11());

        // Mode Gravity: A
        if (this.emitterMode === _ccsg.ParticleSystem.Mode.GRAVITY) {
            var locModeA = this.modeA, locParticleModeA = particle.modeA;
            var s = locModeA.speed + locModeA.speedVar * locRandomMinus11();

            // direction
            locParticleModeA.dir.x = Math.cos(a);
            locParticleModeA.dir.y = Math.sin(a);
            cc.pMultIn(locParticleModeA.dir, s);

            // radial accel
            locParticleModeA.radialAccel = locModeA.radialAccel + locModeA.radialAccelVar * locRandomMinus11();

            // tangential accel
            locParticleModeA.tangentialAccel = locModeA.tangentialAccel + locModeA.tangentialAccelVar * locRandomMinus11();

            // rotation is dir
            if(locModeA.rotationIsDir)
                particle.rotation = -cc.radiansToDegrees(cc.pToAngle(locParticleModeA.dir));
        } else {
            // Mode Radius: B
            var locModeB = this.modeB, locParitlceModeB = particle.modeB;

            // Set the default diameter of the particle from the source position
            var startRadius = locModeB.startRadius + locModeB.startRadiusVar * locRandomMinus11();
            var endRadius = locModeB.endRadius + locModeB.endRadiusVar * locRandomMinus11();

            locParitlceModeB.radius = startRadius;
            locParitlceModeB.deltaRadius = (locModeB.endRadius === _ccsg.ParticleSystem.START_RADIUS_EQUAL_TO_END_RADIUS) ? 0 : (endRadius - startRadius) / locParticleTimeToLive;

            locParitlceModeB.angle = a;
            locParitlceModeB.degreesPerSecond = cc.degreesToRadians(locModeB.rotatePerSecond + locModeB.rotatePerSecondVar * locRandomMinus11());
        }
    },

    /**
     * stop emitting particles. Running particles will continue to run until they die
     */
    stopSystem:function () {
        this._isActive = false;
        this._elapsed = this.duration;
        this._emitCounter = 0;
    },

    /**
     * Kill all living particles.
     */
    resetSystem:function () {
        this._isActive = true;
        this._elapsed = 0;
        var locParticles = this._particles;
        for (this._particleIdx = 0; this._particleIdx < this.particleCount; ++this._particleIdx)
            locParticles[this._particleIdx].timeToLive = 0 ;
    },

    /**
     * whether or not the system is full
     * @return {Boolean}
     */
    isFull:function () {
        return (this.particleCount >= this._totalParticles);
    },

    /**
     * should be overridden by subclasses
     * @param {cc.Particle} particle
     * @param {cc.Vec2} newPosition
     */
    updateQuadWithParticle:function (particle, newPosition) {
        this._renderCmd.updateQuadWithParticle(particle, newPosition);
    },

    /**
     * should be overridden by subclasses
     */
    postStep:function () {
        this._renderCmd.postStep();
    },

    /**
     * update emitter's status
     * @override
     * @param {Number} dt delta time
     */
    update:function (dt) {
        this._renderCmd.setDirtyFlag(_ccsg.Node._dirtyFlags.contentDirty);
        if (this._isActive && this.emissionRate) {
            var rate = 1.0 / this.emissionRate;
            //issue #1201, prevent bursts of particles, due to too high emitCounter
            if (this.particleCount < this._totalParticles)
                this._emitCounter += dt;

            while ((this.particleCount < this._totalParticles) && (this._emitCounter > rate)) {
                this.addParticle();
                this._emitCounter -= rate;
            }

            this._elapsed += dt;
            if (this.duration !== -1 && this.duration < this._elapsed)
                this.stopSystem();
        }
        this._particleIdx = 0;

        var worldToNodeTransform = this.getWorldToNodeTransform();
        var currentPosition = cc.Particle.TemporaryPoints[0];
        if (this.positionType === _ccsg.ParticleSystem.Type.FREE) {
            cc.pIn(currentPosition, this.convertToWorldSpace(this._pointZeroForParticle));
        } else if (this.positionType === _ccsg.ParticleSystem.Type.RELATIVE) {
            currentPosition.x = this._position.x;
            currentPosition.y = this._position.y;
        }

        if (this._visible) {
            // Used to reduce memory allocation / creation within the loop
            var tpa = cc.Particle.TemporaryPoints[1],
                tpb = cc.Particle.TemporaryPoints[2],
                tpc = cc.Particle.TemporaryPoints[3];

            var locParticles = this._particles;
            while (this._particleIdx < this.particleCount) {

                // Reset the working particles
                cc.pZeroIn(tpa);
                cc.pZeroIn(tpb);
                cc.pZeroIn(tpc);

                var selParticle = locParticles[this._particleIdx];

                // life
                selParticle.timeToLive -= dt;

                if (selParticle.timeToLive > 0) {
                    // Mode A: gravity, direction, tangential accel & radial accel
                    if (this.emitterMode === _ccsg.ParticleSystem.Mode.GRAVITY) {

                        var tmp = tpc, radial = tpa, tangential = tpb;

                        // radial acceleration
                        if (selParticle.pos.x || selParticle.pos.y) {
                            cc.pIn(radial, selParticle.pos);
                            cc.pNormalizeIn(radial);
                        } else {
                            cc.pZeroIn(radial);
                        }

                        cc.pIn(tangential, radial);
                        cc.pMultIn(radial, selParticle.modeA.radialAccel);

                        // tangential acceleration
                        var newy = tangential.x;
                        tangential.x = -tangential.y;
                        tangential.y = newy;

                        cc.pMultIn(tangential, selParticle.modeA.tangentialAccel);

                        cc.pIn(tmp, radial);
                        cc.pAddIn(tmp, tangential);
                        cc.pAddIn(tmp, this.modeA.gravity);
                        cc.pMultIn(tmp, dt);
                        cc.pAddIn(selParticle.modeA.dir, tmp);


                        cc.pIn(tmp, selParticle.modeA.dir);
                        cc.pMultIn(tmp, dt);
                        cc.pAddIn(selParticle.pos, tmp);
                    } else {
                        // Mode B: radius movement
                        var selModeB = selParticle.modeB;
                        // Update the angle and radius of the particle.
                        selModeB.angle += selModeB.degreesPerSecond * dt;
                        selModeB.radius += selModeB.deltaRadius * dt;

                        selParticle.pos.x = -Math.cos(selModeB.angle) * selModeB.radius;
                        selParticle.pos.y = -Math.sin(selModeB.angle) * selModeB.radius;
                    }

                    // color
                    this._renderCmd._updateDeltaColor(selParticle, dt);

                    // size
                    selParticle.size += (selParticle.deltaSize * dt);
                    selParticle.size = Math.max(0, selParticle.size);

                    // angle
                    selParticle.rotation += (selParticle.deltaRotation * dt);

                    //
                    // update values in quad
                    //
                    var newPos = tpa;
                    if (this.positionType === _ccsg.ParticleSystem.Type.FREE || this.positionType === _ccsg.ParticleSystem.Type.RELATIVE) {
                        var diff = tpb, localStartPos = tpc;
                        // current Position convert To Node Space
                        cc._pointApplyAffineTransformIn(currentPosition, worldToNodeTransform, diff);
                        // start Position convert To Node Space
                        cc._pointApplyAffineTransformIn(selParticle.startPos, worldToNodeTransform, localStartPos);
                        cc.pSubIn(diff, localStartPos);

                        cc.pIn(newPos, selParticle.pos);
                        cc.pSubIn(newPos, diff);
                    } else {
                        cc.pIn(newPos, selParticle.pos);
                    }

                    this._renderCmd.updateParticlePosition(selParticle, newPos);

                    // update particle counter
                    ++this._particleIdx;
                } else {
                    // life < 0
                    if (this._particleIdx !== this.particleCount -1){
                         var deadParticle = locParticles[this._particleIdx];
                        locParticles[this._particleIdx] = locParticles[this.particleCount -1];
                        locParticles[this.particleCount -1] = deadParticle;
                    }

                    --this.particleCount;
                    if (this.particleCount === 0 && this.autoRemoveOnFinish) {
                        this.unscheduleUpdate();
                        this._parent.removeChild(this, true);
                        this._renderCmd.updateLocalBB && this._renderCmd.updateLocalBB();
                        return;
                    }
                }
            }
            this._renderCmd.updateLocalBB && this._renderCmd.updateLocalBB();
        }

        this.postStep();
    },

    /**
     * update emitter's status (dt = 0)
     */
    updateWithNoTime:function () {
        this.update(0);
    },

    //
    // return the string found by key in dict.
    // @param {string} key
    // @param {object} dict
    // @return {String} "" if not found; return the string if found.
    // @private
    //
    _valueForKey:function (key, dict) {
        if (dict) {
            var pString = dict[key];
            return pString != null ? pString : "";
        }
        return "";
    },

    _updateBlendFunc:function () {
        var locTexture = this._texture;
        if (locTexture && locTexture instanceof cc.Texture2D) {
            this._opacityModifyRGB = false;
            var locBlendFunc = this._blendFunc;
            if (locBlendFunc.src === cc.macro.BLEND_SRC && locBlendFunc.dst === cc.macro.BLEND_DST) {
                if (locTexture.hasPremultipliedAlpha()) {
                    this._opacityModifyRGB = true;
                } else {
                    locBlendFunc.src = cc.macro.SRC_ALPHA;
                    locBlendFunc.dst = cc.macro.ONE_MINUS_SRC_ALPHA;
                }
            }
        }
    },

    /**
     * to copy object with deep copy.
     * returns a clone of action.
     *
     * @return {ccsg.ParticleSystem}
     */
    clone:function () {
        var retParticle = new _ccsg.ParticleSystem();

        // self, not super
        if (retParticle.initWithTotalParticles(this.getTotalParticles())) {
            // angle
            retParticle.setAngle(this.getAngle());
            retParticle.setAngleVar(this.getAngleVar());

            // duration
            retParticle.setDuration(this.getDuration());

            // blend function
            var blend = this.getBlendFunc();
            retParticle.setBlendFunc(blend.src,blend.dst);

            // color
            retParticle.setStartColor(this.getStartColor());

            retParticle.setStartColorVar(this.getStartColorVar());

            retParticle.setEndColor(this.getEndColor());

            retParticle.setEndColorVar(this.getEndColorVar());

            // this size
            retParticle.setStartSize(this.getStartSize());
            retParticle.setStartSizeVar(this.getStartSizeVar());
            retParticle.setEndSize(this.getEndSize());
            retParticle.setEndSizeVar(this.getEndSizeVar());

            // position
            retParticle.setPosition(cc.p(this.x, this.y));
            retParticle.setPosVar(cc.p(this.getPosVar().x,this.getPosVar().y));

            retParticle.setPositionType(this.getPositionType());

            // Spinning
            retParticle.setStartSpin(this.getStartSpin()||0);
            retParticle.setStartSpinVar(this.getStartSpinVar()||0);
            retParticle.setEndSpin(this.getEndSpin()||0);
            retParticle.setEndSpinVar(this.getEndSpinVar()||0);

            retParticle.setEmitterMode(this.getEmitterMode());

            // Mode A: Gravity + tangential accel + radial accel
            if (this.getEmitterMode() === _ccsg.ParticleSystem.Mode.GRAVITY) {
                // gravity
                var gra = this.getGravity();
                retParticle.setGravity(cc.p(gra.x,gra.y));

                // speed
                retParticle.setSpeed(this.getSpeed());
                retParticle.setSpeedVar(this.getSpeedVar());

                // radial acceleration
                retParticle.setRadialAccel(this.getRadialAccel());
                retParticle.setRadialAccelVar(this.getRadialAccelVar());

                // tangential acceleration
                retParticle.setTangentialAccel(this.getTangentialAccel());
                retParticle.setTangentialAccelVar(this.getTangentialAccelVar());

            } else if (this.getEmitterMode() === _ccsg.ParticleSystem.Mode.RADIUS) {
                // or Mode B: radius movement
                retParticle.setStartRadius(this.getStartRadius());
                retParticle.setStartRadiusVar(this.getStartRadiusVar());
                retParticle.setEndRadius(this.getEndRadius());
                retParticle.setEndRadiusVar(this.getEndRadiusVar());

                retParticle.setRotatePerSecond(this.getRotatePerSecond());
                retParticle.setRotatePerSecondVar(this.getRotatePerSecondVar());
            }

            // life span
            retParticle.setLife(this.getLife());
            retParticle.setLifeVar(this.getLifeVar());

            // emission Rate
            retParticle.setEmissionRate(this.getEmissionRate());

            // Set a compatible default for the alpha transfer
            retParticle.setOpacityModifyRGB(this.isOpacityModifyRGB());
            // texture
            var texture = this.getTexture();
            if(texture){
                var size = texture.getContentSize();
                retParticle.setTextureWithRect(texture, cc.rect(0, 0, size.width, size.height));
            }
        }
        return retParticle;
    },

    /**
     * <p> Sets a new CCSpriteFrame as particle.</br>
     * WARNING: this method is experimental. Use setTextureWithRect instead.
     * </p>
     * @param {cc.SpriteFrame} spriteFrame
     */
    setDisplayFrame: function (spriteFrame) {
        if (!spriteFrame)
            return;

        var locOffset = spriteFrame.getOffset();
        if (locOffset.x !== 0 || locOffset.y !== 0)
            cc.logID(6015);

        // update texture before updating texture rect
        var texture = spriteFrame.getTexture(), locTexture = this._texture;
        if (locTexture !== texture)
            this.setTexture(texture);
    },

    /**
     * Sets a new texture with a rect. The rect is in Points.
     * @param {Texture2D} texture
     * @param {Rect} rect
     */
    setTextureWithRect: function (texture, rect) {
        var locTexture = this._texture;
        if (locTexture !== texture) {
            this._texture = texture;
            this._updateBlendFunc();
        }
        this.initTexCoordsWithRect(rect);
    },

    /**
     * listen the event that coming to foreground on Android  (An empty function for native)
     * @param {cc._Class} obj
     */
    listenBackToForeground:function (obj) {
        //do nothing
    }
});

var _p = _ccsg.ParticleSystem.prototype;

// Extended properties
/** @expose */
_p.opacityModifyRGB;
cc.defineGetterSetter(_p, "opacityModifyRGB", _p.isOpacityModifyRGB, _p.setOpacityModifyRGB);
/** @expose */
_p.active;
cc.defineGetterSetter(_p, "active", _p.isActive);
/** @expose */
_p.sourcePos;
cc.defineGetterSetter(_p, "sourcePos", _p.getSourcePosition, _p.setSourcePosition);
/** @expose */
_p.posVar;
cc.defineGetterSetter(_p, "posVar", _p.getPosVar, _p.setPosVar);
/** @expose */
_p.gravity;
cc.defineGetterSetter(_p, "gravity", _p.getGravity, _p.setGravity);
/** @expose */
_p.speed;
cc.defineGetterSetter(_p, "speed", _p.getSpeed, _p.setSpeed);
/** @expose */
_p.speedVar;
cc.defineGetterSetter(_p, "speedVar", _p.getSpeedVar, _p.setSpeedVar);
/** @expose */
_p.tangentialAccel;
cc.defineGetterSetter(_p, "tangentialAccel", _p.getTangentialAccel, _p.setTangentialAccel);
/** @expose */
_p.tangentialAccelVar;
cc.defineGetterSetter(_p, "tangentialAccelVar", _p.getTangentialAccelVar, _p.setTangentialAccelVar);
/** @expose */
_p.radialAccel;
cc.defineGetterSetter(_p, "radialAccel", _p.getRadialAccel, _p.setRadialAccel);
/** @expose */
_p.radialAccelVar;
cc.defineGetterSetter(_p, "radialAccelVar", _p.getRadialAccelVar, _p.setRadialAccelVar);
/** @expose */
_p.rotationIsDir;
cc.defineGetterSetter(_p, "rotationIsDir", _p.getRotationIsDir, _p.setRotationIsDir);
/** @expose */
_p.startRadius;
cc.defineGetterSetter(_p, "startRadius", _p.getStartRadius, _p.setStartRadius);
/** @expose */
_p.startRadiusVar;
cc.defineGetterSetter(_p, "startRadiusVar", _p.getStartRadiusVar, _p.setStartRadiusVar);
/** @expose */
_p.endRadius;
cc.defineGetterSetter(_p, "endRadius", _p.getEndRadius, _p.setEndRadius);
/** @expose */
_p.endRadiusVar;
cc.defineGetterSetter(_p, "endRadiusVar", _p.getEndRadiusVar, _p.setEndRadiusVar);
/** @expose */
_p.rotatePerS;
cc.defineGetterSetter(_p, "rotatePerS", _p.getRotatePerSecond, _p.setRotatePerSecond);
/** @expose */
_p.rotatePerSVar;
cc.defineGetterSetter(_p, "rotatePerSVar", _p.getRotatePerSecondVar, _p.setRotatePerSecondVar);
/** @expose */
_p.startColor;
cc.defineGetterSetter(_p, "startColor", _p.getStartColor, _p.setStartColor);
/** @expose */
_p.startColorVar;
cc.defineGetterSetter(_p, "startColorVar", _p.getStartColorVar, _p.setStartColorVar);
/** @expose */
_p.endColor;
cc.defineGetterSetter(_p, "endColor", _p.getEndColor, _p.setEndColor);
/** @expose */
_p.endColorVar;
cc.defineGetterSetter(_p, "endColorVar", _p.getEndColorVar, _p.setEndColorVar);
/** @expose */
_p.totalParticles;
cc.defineGetterSetter(_p, "totalParticles", _p.getTotalParticles, _p.setTotalParticles);
/** @expose */
_p.texture;
cc.defineGetterSetter(_p, "texture", _p.getTexture, _p.setTexture);


// Different modes
/**
 * Mode A:Gravity + Tangential Accel + Radial Accel
 * @Class
 * @Construct
 * @param {cc.Vec2} [gravity=] Gravity value.
 * @param {Number} [speed=0] speed of each particle.
 * @param {Number} [speedVar=0] speed variance of each particle.
 * @param {Number} [tangentialAccel=0] tangential acceleration of each particle.
 * @param {Number} [tangentialAccelVar=0] tangential acceleration variance of each particle.
 * @param {Number} [radialAccel=0] radial acceleration of each particle.
 * @param {Number} [radialAccelVar=0] radial acceleration variance of each particle.
 * @param {boolean} [rotationIsDir=false]
 */
_ccsg.ParticleSystem.ModeA = function (gravity, speed, speedVar, tangentialAccel, tangentialAccelVar, radialAccel, radialAccelVar, rotationIsDir) {
    /** Gravity value. Only available in 'Gravity' mode. */
    this.gravity = gravity ? gravity : cc.p(0,0);
    /** speed of each particle. Only available in 'Gravity' mode.  */
    this.speed = speed || 0;
    /** speed variance of each particle. Only available in 'Gravity' mode. */
    this.speedVar = speedVar || 0;
    /** tangential acceleration of each particle. Only available in 'Gravity' mode. */
    this.tangentialAccel = tangentialAccel || 0;
    /** tangential acceleration variance of each particle. Only available in 'Gravity' mode. */
    this.tangentialAccelVar = tangentialAccelVar || 0;
    /** radial acceleration of each particle. Only available in 'Gravity' mode. */
    this.radialAccel = radialAccel || 0;
    /** radial acceleration variance of each particle. Only available in 'Gravity' mode. */
    this.radialAccelVar = radialAccelVar || 0;
    /** set the rotation of each particle to its direction Only available in 'Gravity' mode. */
    this.rotationIsDir = rotationIsDir || false;
};

/**
 * Mode B: circular movement (gravity, radial accel and tangential accel don't are not used in this mode)
 * @Class
 * @Construct
 * @param {Number} [startRadius=0] The starting radius of the particles.
 * @param {Number} [startRadiusVar=0] The starting radius variance of the particles.
 * @param {Number} [endRadius=0] The ending radius of the particles.
 * @param {Number} [endRadiusVar=0] The ending radius variance of the particles.
 * @param {Number} [rotatePerSecond=0] Number of degrees to rotate a particle around the source pos per second.
 * @param {Number} [rotatePerSecondVar=0] Variance in degrees for rotatePerSecond.
 */
_ccsg.ParticleSystem.ModeB = function (startRadius, startRadiusVar, endRadius, endRadiusVar, rotatePerSecond, rotatePerSecondVar) {
    /** The starting radius of the particles. Only available in 'Radius' mode. */
    this.startRadius = startRadius || 0;
    /** The starting radius variance of the particles. Only available in 'Radius' mode. */
    this.startRadiusVar = startRadiusVar || 0;
    /** The ending radius of the particles. Only available in 'Radius' mode. */
    this.endRadius = endRadius || 0;
    /** The ending radius variance of the particles. Only available in 'Radius' mode. */
    this.endRadiusVar = endRadiusVar || 0;
    /** Number of degress to rotate a particle around the source pos per second. Only available in 'Radius' mode. */
    this.rotatePerSecond = rotatePerSecond || 0;
    /** Variance in degrees for rotatePerSecond. Only available in 'Radius' mode. */
    this.rotatePerSecondVar = rotatePerSecondVar || 0;
};

/**
 * The Particle emitter lives forever
 * @constant
 * @type Number
 */
_ccsg.ParticleSystem.DURATION_INFINITY = -1;

/**
 * The starting size of the particle is equal to the ending size
 * @constant
 * @type Number
 */
_ccsg.ParticleSystem.START_SIZE_EQUAL_TO_END_SIZE = -1;

/**
 * The starting radius of the particle is equal to the ending radius
 * @constant
 * @type Number
 */
_ccsg.ParticleSystem.START_RADIUS_EQUAL_TO_END_RADIUS = -1;

/**
 * Enum for particle mode
 * @readonly
 * @enum {number}
 */
_ccsg.ParticleSystem.Mode = cc.Enum({
    /** The gravity mode (A mode) */
    GRAVITY: 0,
    /** The radius mode (B mode) */
    RADIUS: 1
});

/**
 * Enum for particle type
 * @readonly
 * @enum {number}
 */
_ccsg.ParticleSystem.Type = cc.Enum({
    /**
     * Living particles are attached to the world and are unaffected by emitter repositioning.
     */
    FREE: 0,

    /**
     * Living particles are attached to the world but will follow the emitter repositioning.<br/>
     * Use case: Attach an emitter to an sprite, and you want that the emitter follows the sprite.
     */
    RELATIVE: 1,

    /**
     * Living particles are attached to the emitter and are translated along with it.
     */
    GROUPED: 2
});
