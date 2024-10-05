
#version 130

uniform sampler2D texture;
uniform vec2 textureSize;
uniform int kernelSize;

float brightness = 0.65;

void main() {
    vec2 texelSize = 1.0 / textureSize;
    int halfKernel = kernelSize / 2;
    
    vec4 sum = vec4(0.0);
    
    for (int i = -halfKernel; i <= halfKernel; i++) {
        for (int j = -halfKernel; j <= halfKernel; j++) {
            vec2 offset = vec2(float(i), float(j)) * texelSize;
            sum += texture2D(texture, gl_TexCoord[0].xy + offset);
        }
    }

    sum /= float(kernelSize * kernelSize);
    sum *= brightness;
    
    gl_FragColor = vec4(sum.x, sum.y, sum.z, 1.0);
}