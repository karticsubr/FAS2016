#include "write-png.h"
#include "./../core/utils.h"

#include <string.h>
#include <iostream>

#define BPP 24

/*Gamma correction : thanks to Gael Cathelin */
template <typename T>
void mygamma_correction(T& data){
    if(data <= 0.0031308){
        data *= 12.92;
    }
    else{
        data = 1.055 * pow(data, 1./2.4) - 0.055;
    }
}

template void mygamma_correction(double &data);

/** write to PNG file **/
template <typename T>
void write_png_rgb(std::string output_file, T *data_ptr, int xRes, int yRes, bool colorOffset) {

    FIBITMAP* true_bitmap = FreeImage_Allocate(xRes, yRes, BPP);

    RGBQUAD true_color;
    for(int r = 0; r < yRes; r++){
        for(int c = 0; c < xRes; c++){

            T rgb[3];
            For(k,3){
                if(colorOffset){
                    rgb[k] = 255.f * data_ptr[3*(r*xRes+c)+k];
                    rgb[k] = Clamp(rgb[k], T(0.0), T(255.0));
                }
                else{
                    rgb[k] = data_ptr[3*(r*xRes+c)+k];
                    rgb[k] = Clamp(rgb[k], T(0.0), T(255.0));
                }
            }

            true_color.rgbRed   = rgb[0];
            true_color.rgbGreen = rgb[1];
            true_color.rgbBlue  = rgb[2];

            FreeImage_SetPixelColor(true_bitmap, c, r, &true_color);
        }
    }
    if(output_file.compare(output_file.size()-4, 4,".png") != 0){
        output_file.erase(output_file.end()-4, output_file.end());
        output_file += ".png";
    }
    FreeImage_Save(FIF_PNG, true_bitmap, output_file.c_str() , 0);
}
template void write_png_rgb(std::string output_file, double* data_ptr, int xRes, int yRes, bool colorOffset);
template void write_png_rgb(std::string output_file, float* data_ptr, int xRes, int yRes, bool colorOffset);

template <typename T>
void write_png_grey(std::string output_file, const T* data_ptr, int xRes, int yRes, bool colorOffset) {

    FIBITMAP* true_bitmap = FreeImage_Allocate(xRes, yRes, BPP);

    RGBQUAD true_color;
    for(int r = 0; r < yRes; r++){
        for(int c = 0; c < xRes; c++){
            T rgb[3];
            For(k,3){
                T tempColor = 0;
                if(colorOffset){
                    if(data_ptr[r*xRes+c] > 1)
                        tempColor = 1;
                    else if(data_ptr[r*xRes+c] < 0)
                        tempColor = 0;
                    else
                        tempColor = data_ptr[r*xRes+c];
                    rgb[k] = 255 * tempColor;
                    //rgb[k] = Clamp(rgb[k], T(0.0),T(255.0));
                }
                else{
                    if(data_ptr[r*xRes+c] > T(255.0))
                        tempColor = T(255.0);
                    else if(data_ptr[r*xRes+c] < T(0.0))
                        tempColor = T(0.0);
                    else
                        tempColor = data_ptr[r*xRes+c];
                    rgb[k] = tempColor;
                    //rgb[k] = Clamp(rgb[k],T( 0.),T(255.0));
                }
            }

            true_color.rgbRed   = rgb[0];
            true_color.rgbGreen = rgb[1];
            true_color.rgbBlue  = rgb[2];

            FreeImage_SetPixelColor(true_bitmap, c, r, &true_color);
        }
    }
    if(output_file.compare(output_file.size()-4, 4,".png") != 0){
        output_file.erase(output_file.end()-4, output_file.end());
        output_file += ".png";
    }
    FreeImage_Save(FIF_PNG, true_bitmap, output_file.c_str() , 0);
}

template void write_png_grey(std::string output_file, const double* data_ptr, int xRes, int yRes, bool colorOffset);
template void write_png_grey(std::string output_file, const float* data_ptr, int xRes, int yRes, bool colorOffset);
template void write_png_grey(std::string output_file, const int* data_ptr, int xRes, int yRes, bool colorOffset);

/*void write_png_grey(std::string output_file, float* data_ptr, int xRes, int yRes, bool colorOffset) {

    FIBITMAP* true_bitmap = FreeImage_Allocate(xRes, yRes, BPP);

    RGBQUAD true_color;
    for(int r = 0; r < yRes; r++){
        for(int c = 0; c < xRes; c++){
            float rgb[3];
            For(k,3){
                if(colorOffset){
                    if(data_ptr[r*xRes+c] > 1)
                        data_ptr[r*xRes+c] = 1;
                    else if(data_ptr[r*xRes+c] < 0)
                        data_ptr[r*xRes+c] = 0;
                    rgb[k] = 255.f * data_ptr[r*xRes+c];
                    rgb[k] = Clamp(rgb[k], 0.f,255.f);
                }
                else{
                    if(data_ptr[r*xRes+c] > 255.f)
                        data_ptr[r*xRes+c] = 255.f;
                    else if(data_ptr[r*xRes+c] < 0.f)
                        data_ptr[r*xRes+c] = 0.f;
                    rgb[k] = data_ptr[r*xRes+c];
                    rgb[k] = Clamp(rgb[k], 0.f,255.f);
                }
            }

            true_color.rgbRed   = rgb[0];
            true_color.rgbGreen = rgb[1];
            true_color.rgbBlue  = rgb[2];

            FreeImage_SetPixelColor(true_bitmap, c, r, &true_color);
        }
    }
    if(output_file.compare(output_file.size()-4, 4,".png") != 0){
        output_file.erase(output_file.end()-4, output_file.end());
        output_file += ".png";
    }
    FreeImage_Save(FIF_PNG, true_bitmap, output_file.c_str() , 0);
}
*/

/** write to PNG file **
void postProcess(const std::string& output_file, float* data_ptr, bool gamma, bool org,
                 int xStart, int xEnd, int yStart, int yEnd, int xRes, int yRes) {

    FIBITMAP* gamma_bitmap = FreeImage_Allocate(xEnd-xStart, yEnd-yStart, BPP);
    FIBITMAP* true_bitmap = FreeImage_Allocate(xEnd-xStart, yEnd-yStart, BPP);

    RGBQUAD gamma_color;
    RGBQUAD true_color;
    float t_c =0, t_r = 0;
    for(int r = yStart; r < yEnd; r++){
        for(int c = xStart; c < xEnd; c++){

            float g_rgb[3], t_rgb[3];
            float fcol = data_ptr[r*xRes+c];

            For(i,3){
                g_rgb[i] = fcol;
                t_rgb[i] = fcol;
                mygamma_correction(g_rgb[i]);
            }

            gamma_color.rgbRed   = 255.f*g_rgb[0];
            gamma_color.rgbGreen = 255.f*g_rgb[1];
            gamma_color.rgbBlue  = 255.f*g_rgb[2];


            true_color.rgbRed   = 255.f*t_rgb[0];
            true_color.rgbGreen = 255.f*t_rgb[1];
            true_color.rgbBlue  = 255.f*t_rgb[2];


            t_r = r - yStart;
            t_c = c - xStart;

            FreeImage_SetPixelColor(gamma_bitmap, t_c, t_r, &gamma_color);
            FreeImage_SetPixelColor(true_bitmap, t_c, t_r, &true_color);
        }
    }

    if(gamma){
        std::stringstream ss;
        ss << output_file;
        std::string str = ss.str();
        str.erase(str.end()-4, str.end());
        FreeImage_Save(FIF_PNG, gamma_bitmap, (str+"-gamma.png").c_str() , 0);
    }
    if(org)
        FreeImage_Save(FIF_PNG, true_bitmap, output_file.c_str() , 0);



//    if(FreeImage_Save(FIF_PNG, gamma_bitmap, str.c_str() , 0))
//        std::cerr << "Image with CUDA gamma-corrected successfully tested!" << std::endl;

}


// write to PNG file **
void debug_postProcess(std::string& output_file, float* data_ptr, int xStart, int xEnd, int yStart, int yEnd, int xRes, int yRes) {

    FIBITMAP* true_bitmap = FreeImage_Allocate(xEnd-xStart, yEnd-yStart, BPP);

    RGBQUAD true_color;
    float t_c =0, t_r = 0;
    int steps = 10;
    for(int r = yStart; r < yEnd; r++){
        for(int c = xStart; c < xEnd; c++){

            float t_rgb[3];
            float fcol = data_ptr[r*xRes+c];
            For(i,3){
                t_rgb[i] = fcol;
            }

            if(t_rgb[0] > 0.0f){
                float valNorm = Clamp(1.0f-float(ceilf(steps*t_rgb[0]))/float(steps), 0.0f, 1.0f);
                t_rgb[0] = 1;
                t_rgb[1] = valNorm;
                t_rgb[2] = valNorm;
            }
            else{
                float valNorm = Clamp(1.0f-float(ceilf(-steps*t_rgb[0]))/float(steps), 0.0f, 1.0f);
                t_rgb[0] = valNorm;
                t_rgb[1] = valNorm;
                t_rgb[2] = 1;
                }

            true_color.rgbRed   = 255.f*t_rgb[0];
            true_color.rgbGreen = 255.f*t_rgb[1];
            true_color.rgbBlue  = 255.f*t_rgb[2];

            t_r = r - yStart;
            t_c = c - xStart;

            FreeImage_SetPixelColor(true_bitmap, t_c, t_r, &true_color);
        }
    }

    unsigned found = output_file.find_last_of("/");
        output_file = output_file.substr(0,found+1)+"debug-"+output_file.substr(found+1);

    //str.erase(str.end()-4, str.end());
    FreeImage_Save(FIF_PNG, true_bitmap, output_file.c_str() , 0);

//    if(FreeImage_Save(FIF_PNG, gamma_bitmap, str.c_str() , 0))
//        std::cerr << "Image with CUDA gamma-corrected successfully tested!" << std::endl;

}
// **/
