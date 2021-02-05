#version 150

in vec2 pass_TexCoord;

out vec4 out_Color;

uniform sampler2D Processing;

uniform bool Gray;
uniform bool HorizontalMirroring;
uniform bool VerticalMirroring;
uniform bool Blur;

float pixel = 1.0/350;
vec2 pixels[9];

float lum = 0.0f;
vec3 sum[9];
float kernel[9];


vec2 texCoord = pass_TexCoord; 

void main() {

    out_Color = texture(Processing, texCoord);

    // inverting the texture coordinates around the desired axes (here y)
    if (HorizontalMirroring) {

        texCoord.y = 1.0 - texCoord.y;
        out_Color = texture(Processing, texCoord);

    }

    // inverting the texture coordinates around the desired axes (here x)
    if (VerticalMirroring) {

        texCoord.x = 1.0 - texCoord.x;
        out_Color = texture(Processing, texCoord);

    }

    // computing grayscale with the luminance like it's given in the slides
    if (Gray) {

        lum = (0.2126 * out_Color.r + 0.7152 * out_Color.g + 0.0722 * out_Color.b);
        out_Color = vec4(lum, lum, lum, 1.0);
    }

    if (Blur) {

        // 3x3 gaussian kernel matrix is needed (defining it like in the slides):

        kernel = float[](
            1.0/16, 1.0/8, 1.0/16,
            1.0/8, 1.0/4, 1.0/8,
            1.0/16, 1.0/8, 1.0/16
        );

        // overwriting the colour of the target pixel with the weighted sum of the colours of the neighbouring pixels

        pixels = vec2[](
            vec2(-pixel, pixel),
            vec2(0.0, pixel),
            vec2(pixel, pixel),
            vec2(-pixel, 0.0),
            vec2(0.0, 0.0),
            vec2(pixel, 0.0),
            vec2(-pixel, -pixel),
            vec2(0.0, -pixel),
            vec2(pixel, -pixel)
        );
       

        for (int i = 0; i < 9; i++) {
            sum[i] = vec3(texture(Processing, texCoord.st + pixels[i]));
        }

        vec3 result = vec3(0.0, 0.0, 0.0);

        for (int i = 0; i < 9; i++) {
            result += sum[i] * kernel[i];
        }

        out_Color = vec4(result, 1.0);
        
    }
}