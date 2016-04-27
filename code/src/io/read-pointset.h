#ifndef READ_POINTSET_H
#define READ_POINTSET_H

//void read_pointset(const char* filename, Array<Pixel> &grid, int width, int height);
void read_pointset_to_vector(std::string const& filename, std::vector<float>& vec);
void read_pointset_vector_to_array(std::string filename, float **inpdata, int &data_size, int &num_vertices);

template <typename T>
void read_pointsetnD(std::string filename, std::vector<T>& vec, int dims);

//void read_pointsetnD(std::string filename, std::vector<float>& vec, int dims);
//void read_pointsetnD(std::string filename, std::vector<double>& vec, int dims);

template <typename T>
void read_glvertices(std::string filename, std::vector<T>& vec, int dim);
#endif
