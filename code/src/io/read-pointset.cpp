#include "./../core/utils.h"
#include "read-pointset.h"
#include <fstream>
#include <limits>

void read_pointset_to_vector(std::string const& filename, std::vector <float>& vec){
    std::ifstream file(filename.c_str());
    //std::size_t line_count;
    float x=0,y=0,z=0, a=0;
    //file >> line_count;

    // skip first two lines
    //file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    //file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //vec.reserve(line_count*3);
    //std::size_t pointer_num;

    while (file >> x >> y >> z >> a){
        vec.push_back(x);
        vec.push_back(y);
        vec.push_back(z);
        vec.push_back(a);
    }
    std::cerr <<"vec size: " << vec.size() << std::endl;
//	For(i, 36){
//		std::cerr << "reading: " << vec[3*i] <<" " << vec[3*i+1] <<" " << vec[3*i+2] << std::endl;
//	}

}

void read_pointset_vector_to_array(std::string filename, float **inpdata, int &data_size, int &num_vertices){
    std::vector<float> vecdata;

    read_pointset_to_vector(filename, vecdata);

    data_size = vecdata.size();
    *inpdata = new float[data_size];

    For(i, data_size){
        (*inpdata)[i] = vecdata[i];
    }

    num_vertices = data_size / 4.0f;
}

template <typename T>
void read_pointsetnD(std::string filename, std::vector<T> &vec, int dims){
    std::ifstream file(filename.c_str());

    if(file.is_open()){
        if(dims == 1){
            if (file) {
                T x=0;

                while (file >> x){
                    vec.push_back(x);
                }
            }
        }
        else if(dims==2){
            if (file) {
                T x[2];
                x[0]=0,x[1]=0;
                while (file >> x[0] >> x[1]){
                    vec.push_back(x[0]);
                    vec.push_back(x[1]);
                }
            }
        }
        else if(dims==3){
            if (file) {
                T x[3];
                x[0]=0,x[1]=0,x[2]=0;
                while (file >> x[0] >> x[1] >> x[2]){
                    vec.push_back(x[0]);
                    vec.push_back(x[1]);
                    vec.push_back(x[2]);
                }
            }
        }
        else if(dims==4){
            if (file) {
                T x[4];
                x[0]=0,x[1]=0,x[2]=0,x[3]=0;
                while (file >> x[0] >> x[1] >> x[2] >> x[3]){
                    vec.push_back(x[0]);
                    vec.push_back(x[1]);
                    vec.push_back(x[2]);
                    vec.push_back(x[3]);
                }
            }
        }
        else if(dims==5){
            if (file) {
                T x[5];
                x[0]=0,x[1]=0,x[2]=0,x[3]=0,x[4]=0;
                while (file >> x[0] >> x[1] >> x[2] >> x[3] >> x[4]){
                    vec.push_back(x[0]);
                    vec.push_back(x[1]);
                    vec.push_back(x[2]);
                    vec.push_back(x[3]);
                    vec.push_back(x[4]);
                }
            }
        }
        else{
            std::cerr << " Dimensions are not correct !!! " << filename << std::endl;
            exit(-1);
        }
    }
    else{
        std::cerr << " Unable to open file: " << filename << std::endl;
        exit(-1);
    }
    //std::cerr <<"Number of points in the data: " << vec.size() / dims << std::endl;
}

template void read_pointsetnD(std::string filename, std::vector<float>& vec, int dim);
template void read_pointsetnD(std::string filename, std::vector<double>& vec, int dim);
template void read_pointsetnD(std::string filename, std::vector<unsigned long>& vec, int dim);

/*
void read_pointsetnD(std::string filename, std::vector<double>& vec, int dims){
    std::ifstream file(filename.c_str());
    if(dims == 1){
        if (file) {
            double x=0;

            while (file >> x){
                vec.push_back(x);
            }
        }
    }
    else if(dims==2){
        if (file) {
            double x[2];
            x[0]=0,x[1]=0;
            while (file >> x[0] >> x[1]){
                vec.push_back(x[0]);
                vec.push_back(x[1]);
            }
        }
    }
    else if(dims==3){
        if (file) {
            double x[3];
            x[0]=0,x[1]=0,x[2]=0;
            while (file >> x[0] >> x[1] >> x[2]){
                vec.push_back(x[0]);
                vec.push_back(x[1]);
                vec.push_back(x[2]);
            }
        }
    }
    else if(dims==5){
        if (file) {
            double x[5];
            x[0]=0,x[1]=0,x[2]=0,x[3]=0,x[4]=0;
            while (file >> x[0] >> x[1] >> x[2] >> x[3] >> x[4]){
                vec.push_back(x[0]);
                vec.push_back(x[1]);
                vec.push_back(x[2]);
                vec.push_back(x[3]);
                vec.push_back(x[4]);
            }
        }
    }
    else{
        std::cerr << " File doesn't exist! " << filename << std::endl;
        exit(-1);
    }
    //std::cerr <<"data size: " << vec.size() / dims << std::endl;
}
*/
template <typename T>
void read_glvertices(std::string filename, std::vector<T>& vec, int dim){
    std::ifstream file(filename.c_str());

    std::cerr <<"WARNING: The first line while reading a file is always ignored !!!" << std::endl;

    //Ignore the first line of the datafiles
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(dim==1){
        if (file) {
            T x=0;
            while (file >> x){
                vec.push_back(x);
                vec.push_back(0);
                vec.push_back(0);
                vec.push_back(1);
            }
        }
        else{
            std::cerr << " File doesn't exist! " << filename << std::endl;
            exit(-1);
        }
    }
    else if(dim==2){
        if (file) {
            T x=0,y=0;
            while (file >> x >> y){
                //std::cout << x <<" " << y << std::endl;
                vec.push_back(x);
                vec.push_back(y);
                vec.push_back(-1.25);
                vec.push_back(1);
            }
        }
        else{
            std::cerr << " File doesn't exist! " << filename << std::endl;
            exit(-1);
        }
    }
    else if(dim==3){
        if (file) {
            T x=0,y=0,z=0;
            while (file >> x >> y >> z){
                vec.push_back(x);
                vec.push_back(y);
                vec.push_back(z);
                vec.push_back(1);
            }
        }
        else{
            std::cerr << " File doesn't exist! " << filename << std::endl;
            exit(-1);
        }
    }
    else if(dim==4){
        if (file) {
            T x=0,y=0,z=0,a=0;
            while (file >> x >> y >> z >> a){
                vec.push_back(x);
                vec.push_back(y);
                vec.push_back(z);
                vec.push_back(a);
            }
        }
        else{
            std::cerr << " File doesn't exist! " << filename << std::endl;
            exit(-1);
        }
    }
    std::cerr <<"vec size: " << vec.size() << std::endl;

}

template void read_glvertices(std::string filename, std::vector<float>& vec, int dim);
template void read_glvertices(std::string filename, std::vector<double>& vec, int dim);


/*
void read_pointset_to_vector(std::string inputfile, std::vector<float>& vec){

    std::ifstream fp(inputfile.c_str());
    char line[200];
    int size;
    float x=0.f,y=0.f,z=0.f;

    if(!fp){
        cout << "Failed to open file: "<<inputfile<<endl;
        exit(1);
    }

    while(1){
        fp.getline(line, 199);
        istringstream token(line);
        token >> x >> y >> z;
        std::cerr <<"reading: " <<  x <<" " << y << " " << z << std::endl;
        vec.push_back(x);
        vec.push_back(y);
        vec.push_back(z);

        if(fp.eof()) break;
    }
}
*/
