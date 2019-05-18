// ShaderUtils.js

let ShaderType = cc.Enum({
    Default: 0,
    Fluxay: 1,
    Fluxay_Super: 2,
    Wave: 3
});


let ShaderUtils = {
    shaderPrograms: {},

    setShader: function(sprite, shaderName) {
        let glProgram = this.shaderPrograms[shaderName];
        if (!glProgram) {
            glProgram = new cc.GLProgram();
            let vert = require(cc.js.formatStr("%s.vert", shaderName));
            let frag = require(cc.js.formatStr("%s.frag", shaderName));
            if(cc.sys.isNative){
                glProgram.initWithString(vert, frag);
            }else{
                glProgram.initWithVertexShaderByteArray(vert, frag);
                glProgram.addAttribute(cc.macro.ATTRIBUTE_NAME_POSITION, cc.macro.VERTEX_ATTRIB_POSITION);  
                glProgram.addAttribute(cc.macro.ATTRIBUTE_NAME_COLOR, cc.macro.VERTEX_ATTRIB_COLOR);  
                glProgram.addAttribute(cc.macro.ATTRIBUTE_NAME_TEX_COORD, cc.macro.VERTEX_ATTRIB_TEX_COORDS);  
            }
            glProgram.link();  
            glProgram.updateUniforms();
            this.shaderPrograms[shaderName] = glProgram;
        }
        sprite._sgNode.setShaderProgram(glProgram);
        return glProgram;
    },

    getShader: function(vert, frag){
        let glProgram = new cc.GLProgram();
        if(cc.sys.isNative){
            glProgram.initWithString(vert, frag);
        }else{
            glProgram.initWithVertexShaderByteArray(vert, frag);
            glProgram.addAttribute(cc.macro.ATTRIBUTE_NAME_POSITION, cc.macro.VERTEX_ATTRIB_POSITION);  
            glProgram.addAttribute(cc.macro.ATTRIBUTE_NAME_COLOR, cc.macro.VERTEX_ATTRIB_COLOR);  
            glProgram.addAttribute(cc.macro.ATTRIBUTE_NAME_TEX_COORD, cc.macro.VERTEX_ATTRIB_TEX_COORDS);  
        }
        glProgram.link();  
        glProgram.updateUniforms();
        glProgram.use();
        return glProgram;
    }
};

module.exports = ShaderUtils;
// {
//     ShaderType: ShaderType,
//     ShaderUtils: ShaderUtils,
// };