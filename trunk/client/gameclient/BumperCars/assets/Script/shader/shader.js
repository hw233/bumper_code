
const ShaderLib = {
    Fluxay: {
        vert:  
        `
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
        frag:
        `
        #ifdef GL_ES
        precision mediump float;
        #endif
        varying vec2 v_texCoord;
        uniform float time;
        void main()
        {
            vec4 src_color = texture2D(CC_Texture0, v_texCoord).rgba;

            float width = 0.08;       //流光的宽度范围 (调整该值改变流光的宽度)
            float start = tan(time/1.414);  //流光的起始x坐标
            float strength = 0.006;   //流光增亮强度   (调整该值改变流光的增亮强度)
            float offset = 0.5;      //偏移值         (调整该值改变流光的倾斜程度)
            if( v_texCoord.x < (start - offset * v_texCoord.y) &&  v_texCoord.x > (start - offset * v_texCoord.y - width))
            {
                vec3 improve = strength * vec3(255, 255, 255);
                vec3 result = improve * vec3( src_color.r, src_color.g, src_color.b);
                gl_FragColor = vec4(result, src_color.a);

            }else{
                gl_FragColor = src_color;
            }
        }
        `,
    },

    Fluxay_Super: {
        vert:  
        `
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
        frag:
        `
        #define TAU 6.120470874064187
        #define MAX_ITER 5
        uniform float time; 
        varying vec2 v_texCoord;
        varying vec4 v_fragmentColor;
        void mainImage( out vec4 fragColor, in vec2 fragCoord ) 
        {
            float time = time * .5+5.;
            // uv should be the 0-1 uv of texture...
            vec2 uv = v_texCoord.xy;//fragCoord.xy / iResolution.xy;
            
    
            vec2 p = mod(uv*TAU, TAU)-250.0;
    
            vec2 i = vec2(p);
            float c = 1.0;
            float inten = .0045;
    
            for (int n = 0; n < MAX_ITER; n++) 
            {
                float t =  time * (1.0 - (3.5 / float(n+1)));
                i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(1.5*t + i.x));
                c += 1.0/length(vec2(p.x / (cos(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
            }
            c /= float(MAX_ITER);
            c = 1.17-pow(c, 1.4);
            vec4 tex = texture2D(CC_Texture0,uv);
            vec3 colour = vec3(pow(abs(c), 20.0));
            colour = clamp(colour + vec3(0.0, 0.0, .0), 0.0, tex.a);
    
            // 混合波光
            float alpha = c*tex[3];  
            tex[0] = tex[0] + colour[0]*alpha; 
            tex[1] = tex[1] + colour[1]*alpha; 
            tex[2] = tex[2] + colour[2]*alpha; 
            fragColor = v_fragmentColor * tex;
        }
        void main()
        {
            mainImage(gl_FragColor, gl_FragCoord.xy);
        }
        `,
    },

    Wave: {
        vert: 
        `
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
        frag:
        `#ifdef GL_ES
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
        }`
    }
};

module.exports = ShaderLib;