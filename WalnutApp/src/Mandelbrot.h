#pragma once
#include <Walnut/Image.h>
#include <memory>
#include "glm/glm.hpp"
#include <random>
#include <iostream>
#include <stdlib.h>
#include <time.h>   
#include <vector>
#include <algorithm>

class Mandelbrot
{
public:
	Mandelbrot() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(int maxiterations, double zoom = 1.0f, float xoffset = 0.0f, float yoffset = 0.0f);

	std::shared_ptr<Walnut::Image> GetFinalImage() const {
		return m_FinalImage;
	}
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
	int m_PrevMaxIteration = 0;
	uint32_t *m_Pallete;
};