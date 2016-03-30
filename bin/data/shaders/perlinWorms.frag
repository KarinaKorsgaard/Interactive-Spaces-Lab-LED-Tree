// shader based on https://www.shadertoy.com/view/MtBSRW
//

#ifdef GL_ES
precision mediump float;
#endif
//uniforms
//tempodivision 100]
uniform vec2 u_resolution;//100,1000;100,1000]
uniform float u_time;//0,50]
uniform float u_zoom;//1,10]
uniform float u_date;//1,3000]
uniform bool u_linesY;//1,3000]
//uniforms

// quadrilin: 252 cubic:269   + 21 for noise transform

#define h(n) fract(sin(n+vec2(0,157))*7.)
#define N m=fract(u*s); l=dot(u*s-m,vec2(1,157));s+=s; m*=m*(3.-m-m); r= mix(h(l),h(++l),m.x); f+= mix(r.x,r.y,m.y)/s;
#define PI 3.14159265359


mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

void main() {
    
    vec2 u = gl_FragCoord.xy;
    //u.x+=u_time;
    
    //rotate because ...
    u -= vec2(0.5);
    u = rotate2d( u_time/10000. ) * u;
    u += vec2(.5);
    
    if(u_linesY){
        u = gl_FragCoord.yy;
    }

    //vec2 u = vec2(0.0);
    vec3 f = vec3(cos(u_time/10000000.));
    
    u = (u_zoom*200.)*u/u_resolution.y-vec2(7,8);
    vec2 m,r; float l,s=.5;
    
    N N N N
    
    // --- comment all below for brute noise, or chose your noise transform :
    f = sin(f*20.+u_time);
    
    
    gl_FragColor = vec4(f*-1.,f.x);
}