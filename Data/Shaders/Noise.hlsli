

float2 randomVec(float2 fact)
{
    float2 angle = float2(dot(fact, float2(127.1, 311.7)), dot(fact, float2(269.5, 183.3)));
    return frac(sin(angle) * 43758.5453123) * 2 - 1;
}

float PerinNoise(float density, float2 uv)
{
    float2 uvFloor = floor(uv * density);
    float2 uvFrac = frac(uv * density);

    float2 v00 = randomVec(uvFloor + float2(0, 0));
    float2 v01 = randomVec(uvFloor + float2(0, 1));
    float2 v10 = randomVec(uvFloor + float2(1, 0));
    float2 v11 = randomVec(uvFloor + float2(1, 1));

    float c00 = dot(v00, uvFloor - float2(0, 0));
    float c01 = dot(v01, uvFloor - float2(0, 1));
    float c10 = dot(v10, uvFloor - float2(1, 0));
    float c11 = dot(v11, uvFloor - float2(1, 1));

    float2 u = uvFrac * uvFrac * (3 - 2 * uvFrac);

    float v0010 = lerp(c00, c10, u.x);
    float v0111 = lerp(c01, c11, u.x);

    return lerp(v0010, v0111, u.y) / 2 + 0.5;
}

float FractalSumNoise(float density, float2 uv)
{
    float fn;
    fn = PerinNoise(density * 1, uv) * 1.0 / 2;
    fn += PerinNoise(density * 2, uv) * 1.0 / 4;
    fn += PerinNoise(density * 4, uv) * 1.0 / 8;
    fn += PerinNoise(density * 8, uv) * 1.0 / 16;
    return fn;
}

float2 gradientNoiseDir(float2 p)
{
    p = p % 289;
    float x = (34 * p.x + 1) * p.x % 289 + p.y;
    x = (34 * x + 1) * x % 289;
    x = frac(x / 41) * 2 - 1;
    return normalize(float2(x - floor(x + 0.5), abs(x) - 0.5));
}

float gradientNoise(float2 p)
{
    float2 ip = floor(p);
    float2 fp = frac(p);
    float d00 = dot(gradientNoiseDir(ip), fp);
    float d01 = dot(gradientNoiseDir(ip + float2(0, 1)), fp - float2(0, 1));
    float d10 = dot(gradientNoiseDir(ip + float2(1, 0)), fp - float2(1, 0));
    float d11 = dot(gradientNoiseDir(ip + float2(1, 1)), fp - float2(1, 1));
    fp = fp * fp * fp * (fp * (fp * 6 - 15) + 10);
    return lerp(lerp(d00, d01, fp.y), lerp(d10, d11, fp.y), fp.x);
}

float GradientNoiseFloat(float2 UV, float Scale)
{
    return gradientNoise(UV * Scale) + 0.5;
}
