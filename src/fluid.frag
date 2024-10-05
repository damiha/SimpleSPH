#version 120

uniform vec2 resolution;
uniform vec2 pixelPositions[1024];
uniform int pointCount;
uniform int kernelRadiusInPixels;

float densityToCutoff = 0.25;
float densityForFoam = 0.65;

float minOpacity = 0.8;
float maxOpacity = 1.0;

void main(){

    float density = 0.;

    for(int i = 0; i < pointCount; i++){
        float additionalDensity = clamp(1.0 - length(gl_FragCoord.xy - pixelPositions[i]) * 1.0 / kernelRadiusInPixels, 0., 1.);
        density += additionalDensity;
    }

    density = clamp(density, 0.0, 1.);

    // cutoff for hard water surface
    if(density < densityToCutoff){
        gl_FragColor = vec4(0., 0., 0., 0.);
    }

    else{
        // visible so normalize
        float lambda = (density - densityForFoam) / (1.0 - densityForFoam);
        lambda = lambda > 0. ? lambda : 0.;

        float opacity = lambda * minOpacity + (1. - lambda) * maxOpacity;

        // 250, 193, 37
        vec4 darkColor = vec4(250., 193., 37., opacity * 255.) / 255.;
        vec4 lightColor = vec4(252., 238., 197., opacity * 255.) / 255.;

        gl_FragColor = lambda * darkColor + (1. - lambda) * lightColor;
    }
}