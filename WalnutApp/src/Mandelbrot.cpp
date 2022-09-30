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

	void GeneratePallete(uint32_t*& v, int numberOfColors)
	{
		v = new uint32_t[numberOfColors];
		for (size_t i = 0; i < numberOfColors; i++)
		{
			v[i] = Utils::random<uint32_t>(0, UINT32_MAX) | 0xff000000;
		}
		std::sort(v, v+numberOfColors);
		v[numberOfColors] = 0xff000000;
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
		Utils::GeneratePallete(m_Pallete, maxiteration);
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


	//Main Calculation
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

			m_ImageData[x + y * width] = m_Pallete[iteration];
		}
	}
	m_FinalImage->SetData(m_ImageData);
}
