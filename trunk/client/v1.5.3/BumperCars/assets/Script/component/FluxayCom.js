let ShaderUtils = require("ShaderUtils");

cc.Class({
    extends: cc.Component,

    properties: {
        model: 1,
    },

    program: null,
    startTime: null,
    resolution: null,

    // onLoad () {},

    uniformResolution: null,
    uniformTime: null,
    uniformTex0: null,

    time: null,

    onLoad(){
        this.time = 0;
        this.startTime = Date.now();
        this.resolution = cc.v2( this.node.getContentSize().width, this.node.getContentSize().height);

        this.useWater();
    },

    useWater() {
        if (this.program) return;

        let vert = require("fluxay").Fluxay_vert;
        let frag = null;
        if(this.model == 1){
            frag = require("fluxay").Fluxay_frag;
        }else if(this.model == 2){
            frag = require("fluxay").Fluxay_frag_s;
        }else if(this.model == 3){
            frag = require("fluxay").Fluxay_frag_super;
        }
        this.program = ShaderUtils.getShader(vert, frag);

        if (cc.sys.isNative) {
            let glProgram_state = cc.GLProgramState.getOrCreateWithGLProgram(this.program);
            glProgram_state.setUniformFloat("time", this.time);
        } else {
            let ba = this.program.getUniformLocationForName("time");
            this.program.setUniformLocationWith1f(ba, this.time);
        }
        this.setProgram(this.node.getComponent(cc.Sprite)._sgNode, this.program);
    },

    setProgram(node, program) {
        if (cc.sys.isNative) {
            let glProgram_state = cc.GLProgramState.getOrCreateWithGLProgram(program);
            node.setGLProgramState(glProgram_state);
        } else {
            node.setShaderProgram(program);
        }
    },

    update (dt) {
        this.time = (Date.now() - this.startTime) / 1000;
        if (this.program) {
            this.program.use();
            if (cc.sys.isNative) {
                let glProgram_state = cc.GLProgramState.getOrCreateWithGLProgram(this.program);
                glProgram_state.setUniformFloat("time", this.time);
            } else {
                let ct = this.program.getUniformLocationForName("time");
                this.program.setUniformLocationWith1f(ct, this.time);
            }
        }
    },
});
