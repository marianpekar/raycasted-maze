#include <iostream>
#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(const bool predictable)
{
	if(predictable)		
		for(auto i = 0; i < 512; i++)
		{
			m_permutations[i] = m_p[i % 256];
		}
	else
		for (int& m_permutation : m_permutations)
		{
			const int random = rand() % 256;
			m_permutation = random % 256;
		}
}

float PerlinNoise::Perlin(const float x, const float y) const
{
	int a = static_cast<int>(std::floor(x));
	int b = static_cast<int>(std::floor(y));

	a &= 255;
	b &= 255;

	const int gi00 = m_permutations[a + m_permutations[b]] % 8;
	const int gi01 = m_permutations[a + m_permutations[b]] % 8;
	const int gi10 = m_permutations[a + m_permutations[b]] % 8;
	const int gi11 = m_permutations[a + m_permutations[b]] % 8;

	const float n00 = PerlinDot(m_grads[gi00], x, y);
	const float n10 = PerlinDot(m_grads[gi10], x - 1, y);
	const float n01 = PerlinDot(m_grads[gi01], x, y - 1);
	const float n11 = PerlinDot(m_grads[gi11], x - 1, y - 1);

	const float u = Fade(x);
	const float v = Fade(y);

	const float x1 = Lerp(n00, n10, u);
	const float x2 = Lerp(n01, n11, u);

	return Lerp(x1, x2, v);
}

float PerlinNoise::FractalBrownianMotion(float x, float y, int octaves, float persistence) const
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

float PerlinNoise::PerlinDot(const Grad& grad, const float x, const float y)
{
	return grad.m_x * x + grad.m_y * y;
}

float PerlinNoise::Lerp(const float a, const float b, const float t)
{
	return (1.0f - t) * a + t * b;
}

float PerlinNoise::Fade(const float t)
{
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise::Remap(const float value,
	const float min1, const float max1,
	const float min2, const float max2)
{
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
