struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

//Change RGB Color to HSB
float3 RGBtoHSB(float3 RGB)
{
    float3 HSB;
    float minRGB = min(min(RGB.r, RGB.g), RGB.b);
    float maxRGB = max(max(RGB.r, RGB.g), RGB.b);
    float deltaRGB = maxRGB - minRGB;
    
    HSB.z = maxRGB;
    
    if (maxRGB != 0)
    {
        HSB.y = deltaRGB / maxRGB;
    }
    else
    {
        HSB.y = 0;
        HSB.x = -1;
        return HSB;
    }
    
    if (RGB.r == maxRGB)
    {
        HSB.x = (RGB.g - RGB.b) / deltaRGB;
    }
    else if (RGB.g == maxRGB)
    {
        HSB.x = 2 + (RGB.b - RGB.r) / deltaRGB;
    }
    else
    {
        HSB.x = 4 + (RGB.r - RGB.g) / deltaRGB;
    }
    
    HSB.x *= 60;
    
    if (HSB.x < 0)
    {
        HSB.x += 360;
    }
    
    return HSB;
}

//Change HSB to RGB
float3 HSBtoRGB(float3 HSB)
{
    float3 RGB;
    float C = HSB.y * HSB.z;
    float X = C * (1 - abs(fmod(HSB.x / 60.0, 2) - 1));
    float m = HSB.z - C;
    
    if (HSB.x >= 0 && HSB.x < 60)
    {
        RGB = float3(C, X, 0);
    }
    else if (HSB.x >= 60 && HSB.x < 120)
    {
        RGB = float3(X, C, 0);
    }
    else if (HSB.x >= 120 && HSB.x < 180)
    {
        RGB = float3(0, C, X);
    }
    else if (HSB.x >= 180 && HSB.x < 240)
    {
        RGB = float3(0, X, C);
    }
    else if (HSB.x >= 240 && HSB.x < 300)
    {
        RGB = float3(X, 0, C);
    }
    else if (HSB.x >= 300 && HSB.x < 360)
    {
        RGB = float3(C, 0, X);
    }
    
    RGB += m;
    
    return RGB;
}

float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
    float3 hsb = RGBtoHSB(input.color.rgb);
    hsb.z *= 0.5f;
    input.color = float4(HSBtoRGB(hsb), 1.0f);
    return input.color;
}
