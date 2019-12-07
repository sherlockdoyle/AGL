#version 450 core

uniform int itr;
uniform float zoom;
uniform vec2 screenSize;
uniform vec2 dxy;

float threshold = 100;

float mandelbrot(vec2 c)
{
    int n = 0;
    float zx = c.x, zy = c.y;
    while(zx*zx+zy*zy<threshold && n<itr)
    {
        float tmp = (zx * zx) - (zy * zy) + c.x;
        zy = -abs(2 * zx * zy) + c.y;
        zx = abs(tmp);
        ++n;
    }
    return float(n)/itr;
}

vec4 map_to_color(float t)
{
    float r = 9.0 * (1.0 - t) * t * t * t;
    float b = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
    float g = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;
    return vec4(r, g, b, 1);
}

void main()
{
    float t = mandelbrot(((gl_FragCoord.xy - screenSize/2)/zoom) - dxy);
    gl_FragColor = map_to_color(t);
}
