let ShaderUtils = require("ShaderUtils");
import ComponentBase from "../common/ComponentBase";

import {MessageType} from "../common/EventCenter";

cc.Class({
    extends: ComponentBase,

    properties: {
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
        let self = this;

        this.time = 0;
        this.resolution = cc.v2( this.node.getContentSize().width, this.node.getContentSize().height);

        this.useWater();

        this.registerEvent(MessageType.Game_Over, function(){
            self.reset();
        });
    },

    useWater() {
        if (this.program) return;

        let WaveShader = require("wave");

        let vert = WaveShader.Wave_Vert;
        let frag = WaveShader.Wave_Frag;

        this.program = ShaderUtils.getShader(vert, frag);

        if (cc.sys.isNative) {
            let glProgram_state = cc.GLProgramState.getOrCreateWithGLProgram(this.program);
            glProgram_state.setUniformFloat("time", this.time);
            glProgram_state.setUniformVec2( "resolution", this.resolution );
        } else {
            let res = this.program.getUniformLocationForName( "resolution" );
            let ba = this.program.getUniformLocationForName("time");
            this.program.setUniformLocationWith2f( res, this.resolution.x,this.resolution.y );
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
        this.time += dt;//(Date.now() - this.startTime) / 1000;
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

    reset(){
        this.time = 0;
    },
});
