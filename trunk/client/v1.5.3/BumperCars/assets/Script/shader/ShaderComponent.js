let ShaderUtils = require("ShaderUtils").ShaderUtils;
let ShaderType = require("ShaderUtils").ShaderType;
let ShaderLib = require('shader');

cc.Class({
    // extends: cc.Component,

    // properties: {
    //     _shader: ShaderType.Default,

    //     shader: {
    //         type: ShaderType,
            
    //         get: function(){
    //             return this._shader;
    //         },

    //         set: function(value){
    //             if (this._shader !== value) {
    //                 this._shader = value;
    //                 this._applyShader();
    //             }
    //         }
    //     }
    // },

    // program: null,
    // startTime: null,
    // resolution: null,

    // uniformResolution: null,
    // uniformTime: null,
    // uniformTex0: null,

    // time: null,

    // onLoad(){
    //     let self = this;

    //     this.time = 0;
    //     this.resolution = cc.v2( this.node.getContentSize().width, this.node.getContentSize().height);

    //     this._applyShader();
    // },

    // _applyShader() {
    //     if (this.program) return;

    //     if(this._shader === ShaderType.Default)return;

    //     let name = ShaderType[this._shader];
    //     let context = ShaderLib[name];

    //     let vert = context.vert;
    //     let frag = context.frag;

    //     this.program = ShaderUtils.getShader(vert, frag);

    //     if (cc.sys.isNative) {
    //         let glProgram_state = cc.GLProgramState.getOrCreateWithGLProgram(this.program);
    //         glProgram_state.setUniformFloat("time", this.time);
    //         glProgram_state.setUniformVec2( "resolution", this.resolution );
    //     } else {
    //         let res = this.program.getUniformLocationForName( "resolution" );
    //         let ba = this.program.getUniformLocationForName("time");
    //         this.program.setUniformLocationWith2f( res, this.resolution.x,this.resolution.y );
    //         this.program.setUniformLocationWith1f(ba, this.time);
    //     }
    //     this.setProgram(this.node.getComponent(cc.Sprite)._sgNode, this.program);
    // },

    // setProgram(node, program) {
    //     if (cc.sys.isNative) {
    //         let glProgram_state = cc.GLProgramState.getOrCreateWithGLProgram(program);
    //         node.setGLProgramState(glProgram_state);
    //     } else {
    //         node.setShaderProgram(program);
    //     }
    // },

    // update (dt) {
    //     this.time += dt;//(Date.now() - this.startTime) / 1000;
    //     if (this.program) {
    //         this.program.use();
    //         if (cc.sys.isNative) {
    //             let glProgram_state = cc.GLProgramState.getOrCreateWithGLProgram(this.program);
    //             glProgram_state.setUniformFloat("time", this.time);
    //         } else {
    //             let ct = this.program.getUniformLocationForName("time");
    //             this.program.setUniformLocationWith1f(ct, this.time);
    //         }
    //     }
    // },

    // reset(){
    //     this.time = 0;
    // },
});
