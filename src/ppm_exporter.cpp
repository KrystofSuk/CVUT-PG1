#include "ppm_exporter.h"

void PPM_Exporter::Export(const std::string& _path, int* _img, std::pair<int, int> _size) const
{
	std::ofstream output(_path + ".ppm");
	output << "P3\n" << _size.first << " " << _size.second << "\n" << 255 << std::endl;

	for (int i = 0; i < _size.first * _size.second * 3; ++i)
		output << _img[i] << " ";

	output.close();
}
