#include "../PixelShader.h"

SamplerState g_SrcSampler       : register( s0 ) ;
Texture2D    g_SrcTexture       : register( t0 ) ;
Texture2D    g_DstTexture       : register( t1 ) ;

static const float CRTGamma = 1.5;
static const float monitorGamma = 2.4;
static const float distance = 1.5;
static const int curvature = 0;
static const float radius = 2.0;
static const float cornerSize = 0.001;
static const int cornerSmooth = 1000;
static const float xTilt = 0;
static const float yTilt = 0;
static const int overScanX = 0;
static const int overScanY = 0;
static const float dotMask = 0.0;
static const int sharper = 3;
static const float scanlineWeight = 0.3;
static const float lum = 0.0;

int __Bfe(int src, int off, int bits) { int mask = (1u << bits) - 1; return (src >> off) & mask; }
int __BfiM(int src, int ins, int bits) { int mask = (1u << bits) - 1; return (ins & mask) | (src & (~mask)); }
int2 Rmp8x8(int a) { return int2(__Bfe(a, 1u, 3u), __BfiM(__Bfe(a, 3u, 3u), a, 1u)); }
uint2 GetInputSize() {
    uint2 dimensions;
    g_SrcTexture.GetDimensions(dimensions.x, dimensions.y);
    return dimensions;
}
uint2 GetOutputSize() {
    uint2 dimensions;
    g_DstTexture.GetDimensions(dimensions.x, dimensions.y);
    return dimensions;
}
float2 GetScale() {
    return float2(GetOutputSize()) / GetInputSize();
}

#pragma warning(disable: 3571) 

#define FIX(c) max(abs(c), 1e-5)
#define PI 3.141592653589

#define TEX2D(c) pow(g_SrcTexture.Sample(g_SrcSampler, c), CRTGamma)

#define aspect float2(1.0, 0.75)

float intersect(float2 xy, float4 sin_cos_angle) {
	float A = dot(xy, xy) + distance * distance;
	float B = 2.0 * (radius * (dot(xy, sin_cos_angle.xy) - distance * sin_cos_angle.zw.x * sin_cos_angle.zw.y) - distance * distance);
	float C = distance * distance + 2.0 * radius * distance * sin_cos_angle.zw.x * sin_cos_angle.zw.y;
	return (-B - sqrt(B * B - 4.0 * A * C)) / (2.0 * A);
}

float2 bkwtrans(float2 xy, float4 sin_cos_angle) {
	float c = intersect(xy, sin_cos_angle);
	float2 point_ = c * xy;
	point_ += radius * sin_cos_angle.xy;
	point_ /= radius;
	float2 tang = sin_cos_angle.xy / sin_cos_angle.zw;
	float2 poc = point_ / sin_cos_angle.zw;
	float A = dot(tang, tang) + 1.0;
	float B = -2.0 * dot(poc, tang);
	float C = dot(poc, poc) - 1.0;
	float a = (-B + sqrt(B * B - 4.0 * A * C)) / (2.0 * A);
	float2 uv = (point_ - a * sin_cos_angle.xy) / sin_cos_angle.zw;
	float r = FIX(radius * acos(a));
	return uv * r / sin(r / radius);
}

float2 fwtrans(float2 uv, float4 sin_cos_angle) {
	float r = FIX(sqrt(dot(uv, uv)));
	uv *= sin(r / radius) / r;
	float x = 1.0 - cos(r / radius);
	float D = distance / radius + x * sin_cos_angle.z * sin_cos_angle.w + dot(uv, sin_cos_angle.xy);
	return distance * (uv * sin_cos_angle.zw - x * sin_cos_angle.xy) / D;
}

float3 maxscale(float4 sin_cos_angle) {
	float2 c = bkwtrans(-radius * sin_cos_angle.xy / (1.0 + radius / distance * sin_cos_angle.z * sin_cos_angle.w), sin_cos_angle);
	float2 a = 0.5 * aspect;
	float2 lo = float2(fwtrans(float2(-a.x, c.y), sin_cos_angle).x,
		fwtrans(float2(c.x, -a.y), sin_cos_angle).y) / aspect;
	float2 hi = float2(fwtrans(float2(+a.x, c.y), sin_cos_angle).x,
		fwtrans(float2(c.x, +a.y), sin_cos_angle).y) / aspect;
	return float3((hi + lo) * aspect * 0.5, max(hi.x - lo.x, hi.y - lo.y));
}

float4 scanlineWeights(float distance1, float4 color) {

#ifdef USEGAUSSIAN
	float4 wid = 0.3 + 0.1 * pow(color, 3.0);
	float v = distance1 / (wid * scanline_weight / 0.3);
	float4 weights = { v, v, v, v };
	return (lum + 0.4) * exp(-weights * weights) / wid;
#else
	float4 wid = 2.0 + 2.0 * pow(color, 4.0);
	float v = distance1 / scanlineWeight;
	float4 weights = float4(v, v, v, v);
	return (lum + 1.4) * exp(-pow(weights * rsqrt(0.5 * wid), wid)) / (0.6 + 0.2 * wid);
#endif
}

float4 Pass1(float2 pos : TEXCOORD0) : COLOR
{
    const uint2 outputSize = GetOutputSize();
    const uint2 inputSize = GetInputSize();
    
    float4 sin_cos_angle = { sin(float2(xTilt, yTilt)), cos(float2(xTilt, yTilt)) };
    float3 stretch = maxscale(sin_cos_angle);
    float2 TextureSize = float2(sharper * inputSize.x, inputSize.y);
	
    float mod_factor = pos.x * outputSize.x;
    float2 ilfac = { 1.0, clamp(floor(inputSize.y / 1000.0), 1.0, 2.0) };
    float2 one = ilfac / TextureSize;

    float2 xy = 0.0;
    if (curvature > 0)
    {
        float2 cd = pos;
        cd = (cd - 0.5) * aspect * stretch.z + stretch.xy;
        xy = bkwtrans(cd, sin_cos_angle) / float2(overScanX / 100.0, overScanY / 100.0) / aspect + float2(0.5, 0.5);
    }
    else
    {
        xy = pos;
    }

    float2 cd2 = xy;
    cd2 = (cd2 - 0.5) * float2(overScanX, overScanY) / 100.0 + 0.5;
    cd2 = min(cd2, 1.0 - cd2) * aspect;
    float2 cdist = float2(cornerSize, cornerSize);
    cd2 = (cdist - min(cd2, cdist));
    float dist = sqrt(dot(cd2, cd2));
    float cval = clamp((cdist.x - dist) * cornerSmooth, 0.0, 1.0);

    float2 ratio_scale = (xy * TextureSize - 0.5) / ilfac;

    float filter = rcp(GetScale().y);
    float2 uv_ratio = frac(ratio_scale);

    xy = (floor(ratio_scale) * ilfac + 0.5) / TextureSize;

    float4 coeffs = PI * float4(1.0 + uv_ratio.x, uv_ratio.x, 1.0 - uv_ratio.x, 2.0 - uv_ratio.x);
    coeffs = FIX(coeffs);
    coeffs = 2.0 * sin(coeffs) * sin(coeffs / 2.0) / (coeffs * coeffs);
    coeffs /= dot(coeffs, float4(1.0, 1.0, 1.0, 1.0));

    float4 col = clamp(mul(coeffs, float4x4(
		TEX2D(xy + float2(-one.x, 0.0)),
		TEX2D(xy),
		TEX2D(xy + float2(one.x, 0.0)),
		TEX2D(xy + float2(2.0 * one.x, 0.0)))),
		0.0, 1.0);
    float4 col2 = clamp(mul(coeffs, float4x4(
		TEX2D(xy + float2(-one.x, one.y)),
		TEX2D(xy + float2(0.0, one.y)),
		TEX2D(xy + one),
		TEX2D(xy + float2(2.0 * one.x, one.y)))),
		0.0, 1.0);

    col = pow(col, CRTGamma);
    col2 = pow(col2, CRTGamma);

    float4 weights = scanlineWeights(uv_ratio.y, col);
    float4 weights2 = scanlineWeights(1.0 - uv_ratio.y, col2);

    uv_ratio.y = uv_ratio.y + 1.0 / 3.0 * filter;
    weights = (weights + scanlineWeights(uv_ratio.y, col)) / 3.0;
    weights2 = (weights2 + scanlineWeights(abs(1.0 - uv_ratio.y), col2)) / 3.0;
    uv_ratio.y = uv_ratio.y - 2.0 / 3.0 * filter;
    weights = weights + scanlineWeights(abs(uv_ratio.y), col) / 3.0;
    weights2 = weights2 + scanlineWeights(abs(1.0 - uv_ratio.y), col2) / 3.0;

    float3 mul_res = (col * weights + col2 * weights2).rgb;
    mul_res *= float3(cval, cval, cval);

    float3 dotMaskWeights = lerp(
		float3(1.0, 1.0 - dotMask, 1.0),
		float3(1.0 - dotMask, 1.0, 1.0 - dotMask),
		floor(fmod(mod_factor, 2.0))
	);
    mul_res *= dotMaskWeights;
    mul_res = pow(mul_res, 1.0 / monitorGamma);
	
	return float4(mul_res, 1.0);
}

float4 CRTGeom_PS( STRETCHRECT_VS_OUTPUT In ) : SV_TARGET0
{
    return Pass1(In.TexCoords0);
}