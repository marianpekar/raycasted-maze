#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(bool predictable)
{
	if(predictable)		
		for(auto i = 0; i < 512; i++)
		{
			m_permutations[i] = m_p[i % 256];
		}
	else
		for (auto i = 0; i < 512; i++)
		{
			auto random = rand() % 256;
			m_permutations[i] = random % 256;
		}
}

float PerlinNoise::Perlin(float x, float y)
{
	int a = floor(x);
	int b = floor(y);

	x = x - a;
	y = y - b;

	a &= 255;
	b &= 255;

	auto gi00 = m_permutations[a + m_permutations[b]] % 8;
	auto gi01 = m_permutations[a + m_permutations[b]] % 8;
	auto gi10 = m_permutations[a + m_permutations[b]] % 8;
	auto gi11 = m_permutations[a + m_permutations[b]] % 8;

	auto n00 = PerlinDot(m_grads[gi00], x, y);
	auto n10 = PerlinDot(m_grads[gi10], x - 1, y);
	auto n01 = PerlinDot(m_grads[gi01], x, y - 1);
	auto n11 = PerlinDot(m_grads[gi11], x - 1, y - 1);


	float u = Fade(x);
	float v = Fade(y);

	float x1 = Lerp(n00, n10, u);
	float x2 = Lerp(n01, n11, u);

	return Lerp(x1, x2, v);
}

float PerlinNoise::FractalBrownianMotion(float x, float y, int octaves, float persistence)
{
	float total = 0;
	float frequency = 1;
	float amplitude = 1;
	float maxValue = 0;

	for (auto i = 0; i < octaves; i++) {
		total = Perlin(x * frequency, y * frequency) * amplitude;
		maxValue += amplitude;
		amplitude *= persistence;
		frequency *= 2;
	}

	return total / maxValue;
}

float PerlinNoise::PerlinDot(Grad grad, float x, float y)
{
	return grad.m_x * x + grad.m_y * y;
}

float PerlinNoise::Lerp(float a, float b, float t)
{
	return (1.0f - t) * a + t * b;
}

float PerlinNoise::Fade(float t)
{
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise::Remap(float value, float min1, float max1, float min2, float max2)
{
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
