// shader based on https://www.shadertoy.com/view/MtBSRW
// edited for own use Jonas Fehr

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;
uniform float u_zoom;
uniform float u_balance;
uniform float u_contrast;
uniform bool bwSwitch;
uniform bool bgTransparent;
uniform bool enableFBM;
uniform bool enableRMF;
uniform vec3 u_color;
// All noise and fbm from iq


float noise3D(vec3 p)
{
    return fract(sin(dot(p ,vec3(12.9898,78.233,128.852))) * 43758.5453)*2.0-1.0;
}

float simplex3D(vec3 p)
{
    
    float f3 = 1.0/3.0;
    float s = (p.x+p.y+p.z)*f3;
    int i = int(floor(p.x+s));
    int j = int(floor(p.y+s));
    int k = int(floor(p.z+s));
    
    float g3 = 1.0/6.0;
    float t = float((i+j+k))*g3;
    float x0 = float(i)-t;
    float y0 = float(j)-t;
    float z0 = float(k)-t;
    x0 = p.x-x0;
    y0 = p.y-y0;
    z0 = p.z-z0;
    
    int i1,j1,k1;
    int i2,j2,k2;
    
    if(x0>=y0)
    {
        if      (y0>=z0){ i1=1; j1=0; k1=0; i2=1; j2=1; k2=0; } // X Y Z order
        else if (x0>=z0){ i1=1; j1=0; k1=0; i2=1; j2=0; k2=1; } // X Z Y order
        else            { i1=0; j1=0; k1=1; i2=1; j2=0; k2=1; } // Z X Z order
    }
    else 
    { 
        if      (y0<z0) { i1=0; j1=0; k1=1; i2=0; j2=1; k2=1; } // Z Y X order
        else if (x0<z0) { i1=0; j1=1; k1=0; i2=0; j2=1; k2=1; } // Y Z X order
        else            { i1=0; j1=1; k1=0; i2=1; j2=1; k2=0; } // Y X Z order
    }
    
    float x1 = x0 - float(i1) + g3; 
    float y1 = y0 - float(j1) + g3;
    float z1 = z0 - float(k1) + g3;
    float x2 = x0 - float(i2) + 2.0*g3; 
    float y2 = y0 - float(j2) + 2.0*g3;
    float z2 = z0 - float(k2) + 2.0*g3;
    float x3 = x0 - 1.0 + 3.0*g3; 
    float y3 = y0 - 1.0 + 3.0*g3;
    float z3 = z0 - 1.0 + 3.0*g3;   
                 
    vec3 ijk0 = vec3(i,j,k);
    vec3 ijk1 = vec3(i+i1,j+j1,k+k1);   
    vec3 ijk2 = vec3(i+i2,j+j2,k+k2);
    vec3 ijk3 = vec3(i+1,j+1,k+1);  
            
    vec3 gr0 = normalize(vec3(noise3D(ijk0),noise3D(ijk0*2.01),noise3D(ijk0*2.02)));
    vec3 gr1 = normalize(vec3(noise3D(ijk1),noise3D(ijk1*2.01),noise3D(ijk1*2.02)));
    vec3 gr2 = normalize(vec3(noise3D(ijk2),noise3D(ijk2*2.01),noise3D(ijk2*2.02)));
    vec3 gr3 = normalize(vec3(noise3D(ijk3),noise3D(ijk3*2.01),noise3D(ijk3*2.02)));
    
    float n0 = 0.0;
    float n1 = 0.0;
    float n2 = 0.0;
    float n3 = 0.0;

    float t0 = 0.5 - x0*x0 - y0*y0 - z0*z0;
    if(t0>=0.0)
    {
        t0*=t0;
        n0 = t0 * t0 * dot(gr0, vec3(x0, y0, z0));
    }
    float t1 = 0.5 - x1*x1 - y1*y1 - z1*z1;
    if(t1>=0.0)
    {
        t1*=t1;
        n1 = t1 * t1 * dot(gr1, vec3(x1, y1, z1));
    }
    float t2 = 0.5 - x2*x2 - y2*y2 - z2*z2;
    if(t2>=0.0)
    {
        t2 *= t2;
        n2 = t2 * t2 * dot(gr2, vec3(x2, y2, z2));
    }
    float t3 = 0.5 - x3*x3 - y3*y3 - z3*z3;
    if(t3>=0.0)
    {
        t3 *= t3;
        n3 = t3 * t3 * dot(gr3, vec3(x3, y3, z3));
    }
    return 96.0*(n0+n1+n2+n3);
    
}

float fbm(vec3 p)
{
    float f;
    f  = 0.50000*(simplex3D( p )); p = p*2.01;
    f += 0.25000*(simplex3D( p )); p = p*2.02;
    f += 0.12500*(simplex3D( p )); p = p*2.03;
    f += 0.06250*(simplex3D( p )); p = p*2.04;
    f += 0.03125*(simplex3D( p )); p = p*2.05;
    f += 0.015625*(simplex3D( p ));
    return f;
}

//absolute value
float ridgedMultifractal(vec3 p)
{
    float f;
    f  = 0.50000*abs(simplex3D( p )); p = p*2.01;
    f += 0.25000*abs(simplex3D( p )); p = p*2.02;
    f += 0.12500*abs(simplex3D( p )); p = p*2.03;
    f += 0.06250*abs(simplex3D( p )); p = p*2.04;
    f += 0.03125*abs(simplex3D( p ));
    return f;
}



float cloud( vec3 p, float balance, bool fbm, bool rmf)
{
    float cloud = 0.0;
    if(fbm && rmf){
        cloud += balance*fbm( p * vec3(1.0, 0.2, 0.2))*0.5+0.5; p = p*2.01;
        cloud += (1.0-balance)*ridgedMultifractal( p );
    }
    else if(fbm){
        cloud += fbm( p * vec3(1.0, 0.2, 0.2))*0.5+0.5; p = p*2.01;
    }
    else if(rmf){
        cloud += ridgedMultifractal( p );
    }

    
    return cloud;
}



void main( )
{
    vec2 uv = gl_FragCoord.xy / u_resolution.xy*1.0-0.5;
    uv.x*=(u_resolution.x/u_resolution.y);
    uv*=u_zoom*10.0;    
    float n = 0.0;
    n = cloud(vec3(u_time, vec2(uv)), u_balance, enableFBM, enableRMF);

    n = ((n - 0.5) * max(u_contrast, 0.0)) + 0.5;

    float a = 1.0;
    if (bwSwitch) n = 1.0-n;
    if (bgTransparent) a = n;

    vec2 st = gl_FragCoord.xy/u_resolution;

    float v = 0.0;
    
    gl_FragColor = vec4(vec3(n*u_color), a);
    

}