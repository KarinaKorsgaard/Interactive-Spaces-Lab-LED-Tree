///*--------------------------------------------------------------------------------------
// License CC0 - http://creativecommons.org/publicdomain/zero/1.0/
// To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
// ----------------------------------------------------------------------------------------
// ^ This means do ANYTHING YOU WANT with this code. Because we are programmers, not lawyers.
// -Otavio Good
// */
//
uniform vec2 iResolution;
uniform float iGlobalTime;
//uniform float u_density;
//uniform float u_amount;
//uniform vec3 u_color;
//
//
//
//// various noise functions
//float Hash2d(vec2 uv)
//{
//    float f = uv.x + uv.y * 47.0;
//    return fract(cos(f*3.333)*100003.9);
//}
//float Hash3d(vec3 uv)
//{
//    float f = uv.x + uv.y * 37.0 + uv.z * 521.0;
//    return fract(cos(f*3.333)*100003.9);
//}
//float mixP(float f0, float f1, float a)
//{
//    return mix(f0, f1, a*a*(3.0-2.0*a));
//}
//const vec2 zeroOne = vec2(0.0, 1.0);
//float noise2d(vec2 uv)
//{
//    vec2 fr = fract(uv.xy);
//    vec2 fl = floor(uv.xy);
//    float h00 = Hash2d(fl);
//    float h10 = Hash2d(fl + zeroOne.yx);
//    float h01 = Hash2d(fl + zeroOne);
//    float h11 = Hash2d(fl + zeroOne.yy);
//    return mixP(mixP(h00, h10, fr.x), mixP(h01, h11, fr.x), fr.y);
//}
//
//float noise(vec3 uv)
//{
//    vec3 fr = fract(uv.xyz);
//    vec3 fl = floor(uv.xyz);
//    float h000 = Hash3d(fl);
//    float h100 = Hash3d(fl + zeroOne.yxx);
//    float h010 = Hash3d(fl + zeroOne.xyx);
//    float h110 = Hash3d(fl + zeroOne.yyx);
//    float h001 = Hash3d(fl + zeroOne.xxy);
//    float h101 = Hash3d(fl + zeroOne.yxy);
//    float h011 = Hash3d(fl + zeroOne.xyy);
//    float h111 = Hash3d(fl + zeroOne.yyy);
//    return mixP(
//                mixP(mixP(h000, h100, fr.x), mixP(h010, h110, fr.x), fr.y),
//                mixP(mixP(h001, h101, fr.x), mixP(h011, h111, fr.x), fr.y)
//                , fr.z);
//}
//
//float PI=3.14159265;
//
//vec3 saturate(vec3 a)
//{
//    return clamp(a, 0.0, 1.0);
//}
//vec2 saturate(vec2 a)
//{
//    return clamp(a, 0.0, 1.0);
//}
//float saturate(float a)
//{
//    return clamp(a, 0.0, 1.0);
//}
//
//float Density(vec3 p)
//{
//    //float ws = 0.06125*0.125;
//    //vec3 warp = vec3(noise(p*ws), noise(p*ws + 111.11), noise(p*ws + 7111.11));
//    float final = noise(p*0.06125);// + sin(iGlobalTime)*0.5-1.95 + warp.x*4.0;
//    float other = noise(p*0.06125 + 1234.567);
//    other -= 0.5;
//    final -= 0.5;
//    final = 0.1/(abs(final*final*other));
//    final += 0.5;
//    return final*1./10.;
//}
//
//mat2 rotate2d(float _angle){
//    return mat2(cos(_angle),-sin(_angle),
//                sin(_angle),cos(_angle));
//}
//
//void main()
//{
//    // ---------------- First, set up the camera rays for ray marching ----------------
//    
//    vec2 uv = gl_FragCoord.xy/iResolution.yy * 2.0 - 1.0;// - 0.5;
//
//    
//    uv -= vec2(0.5);
//    uv = rotate2d( sin(PI/2.)*PI ) * uv;
//    uv += vec2(1.5);
//
//    // Camera up vector.
//    vec3 camUp=vec3(0,1,0); // vuv
//    
//    // Camera lookat.
//    vec3 camLookat=vec3(0,0.0,0);	// vrp
//    
//    float mx=0.2*PI*2.0 + iGlobalTime * 0.0001;
//    float my=-0.5*10.0 + sin(iGlobalTime * 0.005)*0.2+0.1;//*PI/2.01;
//    vec3 camPos=vec3(cos(my)*cos(mx),sin(my),cos(my)*sin(mx))*(200.2); 	// prp
//    
//    // Camera setup.
//    vec3 camVec=normalize(camLookat - camPos);//vpn
//    vec3 sideNorm=normalize(cross(camUp, camVec));	// u
//    vec3 upNorm=cross(camVec, sideNorm);//v
//    vec3 worldFacing=(camPos + camVec);//vcv
//    vec3 worldPix = worldFacing + uv.x * sideNorm * (iResolution.x/iResolution.y) + uv.y * upNorm;//scrCoord
//    vec3 relVec = normalize(worldPix - camPos);//scp
//    
//    // --------------------------------------------------------------------------------
//    float t = 1.0;
//    float inc = 0.02;
//    float maxDepth = u_amount*200.0;
//    float minDepth = 1.0;
//    vec3 pos = vec3(0,0,0);
//    float density = 0.0;
//    // ray marching time
//    for (int i = 0; i < 57; i++)	// This is the count of how many times the ray actually marches.
//    {
//        if ((t > maxDepth)) break;
//        if ((t < minDepth)) break;
//        
//        pos = camPos + relVec * t;
//        float temp = Density(pos);
//        temp *= saturate(t-1.0);
//        
//        inc =  3.;//+temp;	// add temp because this makes it look extra crazy!
//        density += temp * inc;
//        t += inc;
//    }
//    
//    // --------------------------------------------------------------------------------
//    // Now that we have done our ray marching, let's put some color on this.
//    vec3 finalColor = vec3(u_color*0.00001)* density*(u_density+0.2);
//    
//
//    
//    // output the final color with sqrt for "gamma correction"
//    gl_FragColor = vec4(sqrt(clamp(finalColor, 0.0, 1.0)),1.);
//}

//#define Use_Linear
//#define Use_Cosine
#define Use_Smoothstep
//#define Use_Cubic
//#define Use_ThirdOrderSpline
//#define Use_Catmull_Rom


//--------------------------------------------------------------------------------
//  1 out, 1 in...
#define HASHSCALE .1031
float Hash(float p)
{
    vec3 p3  = fract(vec3(p) * HASHSCALE);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

//--------------------------------------------------------------------------------
float Cubic(float x, float v0,float v1, float v2,float v3)
{
    float p = (v3 - v2) - (v0 - v1);
    return p*(x*x*x) + ((v0 - v1) - p)*(x*x) + (v2 - v0)*x + v1;
}

//--------------------------------------------------------------------------------
float Catmull_Rom(float x, float v0,float v1, float v2,float v3)
{
    float c2 = -.5 * v0	+ 0.5*v2;
    float c3 = v0		+ -2.5*v1 + 2.0*v2 + -.5*v3;
    float c4 = -.5 * v0	+ 1.5*v1 + -1.5*v2 + 0.5*v3;
    return(((c4 * x + c3) * x + c2) * x + v1);
    
    //	Or, the same result with...
    //	float x2 = x  * x;
    //	float x3 = x2 * x;
    //	return 0.5 * ( ( 2.0 * v1) + (-v0 + v2) * x +
    //                  (2.0 * v0 - 5.0 *v1 + 4.0 * v2 - v3) * x2 +
    //                  (-v0 + 3.0*v1 - 3.0 *v2 + v3) * x3);
    
    
}

//--------------------------------------------------------------------------------
float ThirdOrderSpline(float x, float L1,float L0, float H0,float H1)
{
    return 		  L0 +.5 *
    x * ( H0-L1 +
         x * ( H0 + L0 * -2.0 +  L1 +
              x * ((H0 - L0)* 9.0	 + (L1 - H1)*3.0 +
                   x * ((L0 - H0)* 15.0 + (H1 - L1)*5.0 +
                        x * ((H0 - L0)* 6.0	 + (L1 - H1)*2.0 )))));
}

//--------------------------------------------------------------------------------
float Cosine(float x, float v0, float v1)
{
    x = (1.0-cos(x*3.1415927)) * .5;
    return (v1-v0)*x + v0;
}

//--------------------------------------------------------------------------------
float Linear(float x, float v0, float v1)
{
    return (v1-v0)*x + v0;
}

//--------------------------------------------------------------------------------
float Smoothstep(float x, float v0, float v1)
{
    x = x*x*(3.0-2.0*x);
    return (v1-v0)*x + v0;
}

//================================================================================
void main()
{
    vec2 uv = gl_FragCoord.yy / iResolution.xy;
    uv.x *= iResolution.x/iResolution.y;
    
    float pos = (iGlobalTime*0.1-uv.x) * 3.0;
    float x  = fract(pos);
    float v0 = Hash(floor(pos));
    float v1 = Hash(floor(pos)+1.0);
    float v2 = Hash(floor(pos)+2.0);
    float v3 = Hash(floor(pos)+3.0);
    float f;
    
#ifdef Use_Linear
    f = Linear(x, v1, v2);
#elif defined Use_Cosine
    f = Cosine(x, v1, v2);
#elif defined Use_Smoothstep
    f = Smoothstep(x, v1, v2);
#elif defined Use_Cubic
    f = Cubic(x, v0, v1, v2, v3);
#elif defined Use_Catmull_Rom
    f = Catmull_Rom(x, v0, v1, v2, v3);
#elif defined Use_ThirdOrderSpline
    f = ThirdOrderSpline(x, v0, v1, v2, v3);
#endif
    
//    // Blobs...
    f = .02 / abs(f-uv.y);
//    float d = .03/length((vec2(((uv.x)/9.0*.25), uv.y)-vec2(x+.03, v1)) * vec2(.25,1.0));
//    f = max(f, d*d);
//    d = .03/length((vec2(((uv.x)/9.0*.25), uv.y)-vec2(x-.97, v2)) * vec2(.25,1.0));
//    f = max(f, d*d);

    gl_FragColor = vec4(vec3(1.0) * f, 1.0);
}
