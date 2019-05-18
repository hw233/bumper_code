// gray.frag.js
module.exports = {
    Wave_Vert: `
    attribute vec4 a_position;
    attribute vec2 a_texCoord;
    attribute vec4 a_color;
    varying vec2 v_texCoord;
    varying vec4 v_fragmentColor;
    void main()
    {
        gl_Position = CC_PMatrix * a_position;
        v_fragmentColor = a_color;
        v_texCoord = a_texCoord;
    }
    `,

    Wave_Frag: `
    #ifdef GL_ES
        precision mediump float;
        #endif
        
        #define F cos(x-y)*cos(y),sin(x+y)*sin(y)
    
        uniform float time;
        uniform vec2 resolution;
        varying vec2 v_texCoord;
    
        vec2 s(vec2 p)
        {
            float d=time*0.2;
            float x=8.*(p.x+d);
            float y=8.*(p.y+d);
            return vec2(F);
        }
        void mainImage( out vec4 fragColor, in vec2 fragCoord )
        {
            // 换成resolution
            vec2 rs = resolution.xy;
            // 换成纹理坐标v_texCoord.xy
            vec2 uv = v_texCoord.xy;
            vec2 q = uv+10./resolution.x*(s(uv)-s(uv+rs));
            //反转y
            // q.y=1.-q.y;
            fragColor = texture2D(CC_Texture0,q);
        }
        void main()
        {
            mainImage(gl_FragColor, gl_FragCoord.xy);
        }
    `
};
