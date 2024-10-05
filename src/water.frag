#version 120

uniform vec2 resolution;
uniform vec2 pixelPositions[1024];
uniform int pointCount;
uniform int kernelRadiusInPixels;

void main(){

    float density = 0.;
    int neighbors = 0;

    for(int i = 0; i < pointCount; i++){
        float additionalDensity = clamp(1.0 - length(gl_FragCoord.xy - pixelPositions[i]) * 1.0 / kernelRadiusInPixels, 0., 1.);

        if(additionalDensity > 0.){
            neighbors += 1;
        }
        density += additionalDensity;
    }

    density = clamp(density, 0.0, 1.);

    // cutoff for hard water surface
    if(density < 0.25){
        gl_FragColor = vec4(0., 0., 0., 1.);
    }

    else{
        // visible so normalize
        float lambda = (density - 0.5) / (0.5);

        // 250, 193, 37
        vec4 darkColor = vec4(250., 193., 37., 255.) / 255.;
        vec4 lightColor = vec4(252., 238., 197., 255.) / 255.;

        gl_FragColor = lambda * darkColor + (1. - lambda) * lightColor;
    }
}