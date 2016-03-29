#define tSize 0.05
#define blobPower 0.45

uniform float size;
uniform vec2 iResolution;
uniform float speed;

float samp1()
{
    return blobPower * (size);
}

float blob(vec2 uv, vec2 triPos, float flip)
{
    float len = length(uv - triPos);
    
    float r = min(0.7, ((1.-tSize*2.)/(3.*len)*0.1*samp1()));
    return r;
}

void main()
{
    vec2 uv = -1.0 + 2.0 * gl_FragCoord.xy / iResolution.y;
    
    vec3 col = vec3(0.);
    
    for (float i = 0.; i < 80.; i++)
    {
        float b = blob(uv+ vec2(0.75-tSize*i, sin(i)*0.05), vec2(0.,-1.5+mod(speed/10.*(cos(i)), 2.+i*0.1)), floor(cos(i)+0.99));
        col += vec3(b);
    }
    //col = mix(vec3(1.),vec3(0.), floor(ceil(col.r) - 1.0));
    //	col = vec3( col.x );
    float alpha = 1.;
    alpha = 1.-col.x;
    gl_FragColor = vec4(col,alpha);
}