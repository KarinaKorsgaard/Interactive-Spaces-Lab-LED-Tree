uniform vec2 iResolution;
uniform float iGlobalTime;

float metaball(int index)
{
    float i = float(index) * 0.5;
    
    vec2 pos = vec2(iResolution.y*0.5 + (iResolution.y * 0.4) * sin(iGlobalTime * 0.3 * i ),
                    iResolution.y*0.5 + (iResolution.y * 0.4) * cos(iGlobalTime * 0.7 * i ));
    
    float radius = (iResolution.x * 0.025) + (i * (iResolution.x * 0.0125));
    
    return (radius / sqrt( (gl_FragCoord.x-pos.x)*(gl_FragCoord.x-pos.x) + (gl_FragCoord.y-pos.y)*(gl_FragCoord.y-pos.y) ) );
}

float getMetaballValue()
{
    float sum = 0.0;
    for(int i = 1; i < 8; i++)
    {
        sum += metaball(gl_FragCoord.yy,i);
    }
    
    return sum;
}

void main()
{
    float sum = getMetaballValue();
    
    vec2 uv = gl_FragCoord / iResolution.yy;
    vec3 col = clamp(sum,0.0,2.0) * vec3(uv,0.5+0.5);
    
    gl_FragColor = vec4(col,1.0);
}