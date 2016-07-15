/* Generate a distribution based on an AA pattern and a loadable displacement map
 * Values which depend on pattern parameters are read from a header file so
 *  that different parameters can be used with this code.
 * We could also load these dynamically, but hard-coding them gives substantial speedup
 * Created by Abdalla Ahmed
 * 2014-11-11
 * Last revision 2015-09-10
 */

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <getopt.h>
#include <time.h>
#include <algorithm>
#include <sstream>

#include "sampler-points.h"
#include "t191861r110771.h"

const unsigned long aa_q = aa_t + aa_r;                                         // m is assumed 1.
const unsigned long aa_ONE = 2 * aa_q;                                          // We call 2q "ONE" coz it's the denominator of indexes in a unit torus
const unsigned long aa_p = aa_q + aa_t;                                         // Please refer to JGT paper by Ahmed for the meaning of all these parameters.
const unsigned long aa_2t = 2 * aa_t;                                           // We fix these to avoid repeated calculations in the inner loop
const unsigned long aa_ONE_minus_2t = aa_ONE - aa_2t;                           // ~
const unsigned long aa_minus_2t = aa_2t - aa_ONE;                               // ~

struct TPoint {
    double x, y;
};

TPoint *map;
double maxDisplacement;                                                         // A supremum for the shift in x or y directions.

int aa(TPoint *list, int nxn) {                                                 // This function generates n x n points in the unit domain
    double n = sqrt(nxn);                                                       // Average number of points in a single row
    double w = (n / aa_t) * aa_ONE;                                             // The width to scan: aa_ONE produces aa_t points per row
    double scanRange = w + 2 * maxDisplacement + 2;                                  // Add a margin for displacement of points; and a margin for th
    double scale = 1.0 / w;                                                     // Multiplication is faster than division
    int dX0 = rand() % aa_t;                                                    // Randomly choose a slicing position in the pattern. Only affects spatial domain,
    int dY0 = rand() % aa_t;                                                    // but it doesn't make much difference in statistics or spectrum
    int dX, dY;                                                                 // Indexes
    unsigned long count = 0;                                                    // Counter for generated points
    for (int y = 0; y < scanRange; y++) {                                       // Loop through rows
        dY = (aa_p*y + dY0) % aa_ONE;                                           // dY alternates between only two values in a row (for even/odd values of x)
        if ((dY % aa_q) >= aa_t) continue;                                      // If both values do not fulfill the pattern condition skip this row
        int xStart = 0;                                                         // Assume dY would work for even values of x
        dX = (aa_q*y + dX0) % aa_ONE;                                           // Find dX at beginning of row. NB this can also be rewritten to use addition only
        if (dY >= aa_q) {                                                       // If dY actually satisfy the condition for odd values of x
            xStart++;                                                           // Advance one step to odd x's; start at x = 1
            dY -= aa_q;                                                         // Adjust dY accordingly; this remains fixed for all odd values of x
            dX = (dX + aa_p) % aa_ONE;                                          // And adjust dX.
        }
        TPoint *row = &map[bsearch(dY) * MAP_WIDTH];                            // Since dY is fixed we can point at the beginning of concerned row in map
        for (int x = xStart; x < scanRange; x += 2) {                           // Step only through even or odd values of x (for which dY fulfills condition)
            if (dX < aa_t) {                                                    // Check pattern condition on dX; dY is already checked
                TPoint& shift = row[ bsearch(dX) ];                             // Locate the appropriate shift
                double xx = scale * (x + shift.x - maxDisplacement);            // Apply shift and scale to unit domain
                double yy = scale * (y + shift.y - maxDisplacement);
                if (xx >= 0 && xx < 1 && yy >= 0 && yy < 1){
                    TPoint newpt;
                    newpt.x = xx;
                    newpt.y = yy;
                    list[count++] = newpt;// {xx, yy};
                }
            }
            if (dX < aa_ONE_minus_2t) dX += aa_2t; else dX += aa_minus_2t;      // "dX = (dX + 2*aa_p) % aa_ONE", Avoiding division gives substantial speedup.
        }
    }
    return count;
}

void loadVectors(const char *fileName) {                                        // This function loads the vectors lookup table
    FILE *vectorsFile = fopen(fileName, "r");
    if (!vectorsFile) {
        fprintf(stderr, "Failed to open %s\n", fileName);
        exit(1);
    }
    int ng = MAP_WIDTH * MAP_WIDTH;                                             // MAP_WIDTH is supplied in the header file, ng is the number of groups
    map = new TPoint[ng];
    maxDisplacement = 0;
    for (int i = 0; i < ng; i++) {
        fscanf(vectorsFile, " %lf %lf", &map[i].x, &map[i].y);
        if (feof(vectorsFile)) {
            fprintf(stderr, "Failed to read all vectors from %s\n", fileName);
            exit(1);
        }
        maxDisplacement = std::max(maxDisplacement, (fabs(map[i].x)));
        maxDisplacement = std::max(maxDisplacement, (fabs(map[i].y)));
    }
    maxDisplacement = ceil(maxDisplacement);
}

// This function prints a text listing of generated points
void printText(char *fileName, TPoint *p, int n) {
    FILE *outputFile = (fileName[0] == '-' ? stdout : fopen(fileName, "w"));
    if (!outputFile) {
        fprintf(stderr, "Failed to open %s for writing\n", fileName);
        exit(1);
    }
    fprintf(outputFile, "%d\n", n);                                             // Print number of output points.
    for (int i = 0; i < n; i++) {
        fprintf(outputFile, "%lf %lf\n", p[i].x, p[i].y);                       // output it
    }
}

const char *USAGE_MESSAGE = "Usage: %s [-o fileName] <vetor-list-file> [n]\n"
"Use - as fileName for stdout\n"
"Please skip printouts to test performance with huge numbers (e.g. 100M)\n"
;

template<int Dimension, typename T>
std::vector<T> PointSampler<Dimension, T>::bluenoise_samples(int N, T* bBox, std::string samplingpattern, std::string filepath, int dim){

    std::vector<T> samples;

    double maxRange = bBox[2*dim-1] - bBox[dim-1];
    std::stringstream ss;
    std::string vectorsFileName = "";
    if(samplingpattern == "bnot"){
        ss << filepath << "/code/src/sampler-points/displacement-maps/blue.dat";
        vectorsFileName = ss.str().c_str();
    }
    else if(samplingpattern == "step"){
        ss << filepath << "/code/src/sampler-points/displacement-maps/step.dat";
        vectorsFileName = ss.str().c_str();
    }
    else if(samplingpattern == "fpo"){
        ss << filepath << "/code/src/sampler-points/displacement-maps/fpo-like.dat";
        vectorsFileName = ss.str().c_str();
    }
    else{
        std::cerr << "Only bnot, step and fpo bluenoise samplers are available !!! ";
        exit(-2);
    }

    loadVectors(vectorsFileName.c_str());
    TPoint *list = new TPoint[(int)(1.1*N)]();                                    // Add a 10% safety margin in buffer size. This is admittedly subjective
    int actual = aa(list, N);
    for(int i = 0; i < actual; i++){
        samples.push_back(maxRange * list[i].x + bBox[0]);
        samples.push_back(maxRange * list[i].y + bBox[0]);
    }

    delete [] list;
    return samples;
}

template std::vector<double> pointSampler2dd::bluenoise_samples(int N, double* bBox, std::string samplingpattern, std::string filepath, int dim);

//int main(int argc,char **argv) {
//    setlocale(LC_NUMERIC, "");
//    srand(time(NULL));
//    int opt;
//    char *outputFileName = NULL;
//    while ((opt = getopt(argc, argv, "o:")) != -1) {
//        switch (opt) {
//            case 'o': outputFileName = optarg; break;                           // Use '-' for stdout
//            default: fprintf(stderr, USAGE_MESSAGE, argv[0]); exit(1);
//        }
//    }
//    if (optind > argc - 1) {
//        fprintf(stderr, USAGE_MESSAGE, argv[0]); exit(1);
//    }
//    char *vectorsFileName = argv[optind];
//    fprintf(stderr, "Loading vectors: ");
//    loadVectors(vectorsFileName);
//    fprintf(stderr, "done\n");
//    int n = (optind == argc - 2 ? atoi(argv[optind+1]) : 1024);
//    TPoint *list = new TPoint[(int)(1.1*n)];                                    // Add a 10% safety margin in buffer size. This is admittedly subjective
//    fprintf(stderr, "Generating %d points..\n", n);
//    clock_t t0 = clock();
//    int actual = aa(list, n);
//    clock_t t1 = clock();
//    double totalTime = (double)(t1 - t0) / CLOCKS_PER_SEC;
//    fprintf(stderr, "generated %'d points in %.6fs (%'d points per second)..\n",
//            actual, totalTime, (int)(actual/totalTime));
//    if (outputFileName != NULL) {                                               // Please skip printouts to test performance with huge numbers (e.g. 100M)
//        printText(outputFileName, list, actual);
//    }
//    return 0;
//}
