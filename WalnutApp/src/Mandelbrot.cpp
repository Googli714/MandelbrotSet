#include "Mandelbrot.h"

namespace Utils {
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}

	double Round(double d)
	{
		return floor(d + 0.5f);
	}

	template<typename T>
	T random(T range_from, T range_to) {
		std::random_device                  rand_dev;
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<T>    distr(range_from, range_to);
		return distr(generator);
	}
}

void Mandelbrot::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Mandelbrot::Render(int maxiteration, double zoom, float xoffset, float yoffset)
{
	if (maxiteration != m_PrevMaxIteration)
	{
		m_Pallete = std::vector<uint32_t>(maxiteration);
		m_Pallete.resize(maxiteration);
		for (size_t i = 0; i < maxiteration; i++)
		{
			m_Pallete[i] = Utils::random<uint32_t>(0, UINT32_MAX);
			m_Pallete[i] |= 0xff000000;
		}
		m_PrevMaxIteration = maxiteration;
	}

	double xrange_start = -2.0f / zoom;
	double xrange_end = 1.0f / zoom;
	double yrange_start = -1.0f / zoom;
	double yrange_end = 1.0f / zoom;

	int width = m_FinalImage->GetWidth();
	int height = m_FinalImage->GetHeight();

	if (static_cast<double>(height) / width > 2.0f / 3.0f)
	{
		height = width * 2.0f / 3.0f;
	}
	else
	{
		width = height * 3.0f / 2.0f;
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
	m_FinalImage->Resize(width, height);


	double xslope = 1.0f * (xrange_end - xrange_start) / (width);
	double yslope = 1.0f * (yrange_end - yrange_start) / (height);

	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			double x0 = xrange_start + xslope * x - xoffset;
			double y0 = yrange_start + yslope * y - yoffset;

			double xT = 0.0;
			double yT = 0.0;
			uint32_t iteration = 0;
			while (xT * xT + yT * yT <= 4 && iteration < maxiteration)
			{
				double xtemp = xT * xT - yT * yT + x0;
				yT = 2.0 * xT * yT + y0;
				xT = xtemp;
				iteration++;
			}

			uint32_t color;
			double iter = double(iteration) / maxiteration;

			if (iteration == maxiteration)
			{
				color = 0xff000000;
			}
			else
			{
				//color = Utils::ConvertToRGBA(glm::vec4(iter, 0.0f, 0.0f, 1.0f));
				//color = pallete[(int)floor(iteration / 100.0f)];
				color = m_Pallete[iteration];
			}

			m_ImageData[x + y * width] = color;
		}
	}

	m_FinalImage->SetData(m_ImageData);
}
