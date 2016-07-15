#include <DataMat.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cassert>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <fstream>

#define DEBUG_PRINT(n,what) if(_Verbosity>n) {cout << what;}
#define DEBUG_DATA(n,data,file,suffix) if(_Verbosity>n){ToFile(data,file,suffix);}

#define DEBUGPixel if(false)
typedef double Real ;
double WFunc (double Wa, double Wb, double Sigma); 
extern double FracGaussianWidth ;
int CGALLinearInterp(const DataMat& Input, const vector<pair<int, int> > Indices, DataMat& Interpolant ) ;

namespace
{
	inline bool CloseToZero(double d)
	{
		return fabs(d) < 1e-3 ;
	}
	
	template<typename Real>
			inline Real clamp(const Real min_value, const Real max_value, const Real x)
	{ return std::max(std::min(x,max_value),min_value); }
        
	inline double bilinear_interpolation(const DataMat& array, const Real x, const Real y)
	{
		typedef unsigned int size_type;
		typedef double real_type;
                
		const size_type x_size = array.x_size();
		const size_type y_size = array.y_size();
                
		const size_type x_index  =
				clamp<size_type>(0,x_size-1,static_cast<size_type>(x));
		const size_type xx_index =
				clamp<size_type>(0,x_size-1,x_index+1);
                
		const size_type y_index  =
				clamp<size_type>(0,y_size-1,static_cast<size_type>(y));
		const size_type yy_index =
				clamp<size_type>(0,y_size-1,y_index+1);
                
		const real_type x_alpha = x - x_index;
		const real_type y_alpha = y - y_index;
                
		return
				(1.0-x_alpha) * (1.0-y_alpha) * array(y_index, x_index) +
				x_alpha       * (1.0-y_alpha) * array(y_index,xx_index) +
				(1.0-x_alpha) * y_alpha       * array(yy_index, xx_index) +
				x_alpha       * y_alpha       * array(yy_index,xx_index);                        
	}

}

// assume datamat values are in [0,1]
// for nBins in MaxVal-MinVal, the bin centers are dw = (MaxVal-MinVal)/nBins units apart
// the bin centres are at (2p+1)dw/2 where p = 0, 1, 2 ... nBins-1
// For a given value v, its bin # is k=floor(v/dw)
// Distance to its center is d = v-(2k+1)dw/2
// Contribution of v = 1-d/dw for bin # k and d/dw for bin # k+/-1 

void DataMat::Histogram(vector<double>& hist, vector<double>& CumulHist, int nBins, bool normalize) const
{
	if (nBins<=0) return; 
// 	hist.resize(nBins) ;
// 	CumulHist.resize(nBins) ;
	double MinVal(_V[0][0]), MaxVal(_V[0][0]) ;
	int nR(NRows()), nC(NCols()) ;
	for (int r(0); r<nR; r++)
	{                
		for (int c(0); c<nC; c++)
		{ MinVal=min(MinVal, _V[r][c]); MaxVal=max(MaxVal, _V[r][c]);}
	}
	MinVal = 0 ;
	MaxVal = 1; 	
	double dw((MaxVal-MinVal)/nBins) ;
	
// 	With Linear inteprolation
	for (int r(0); r<nR; r++)
	{                
		for (int c(0); c<nC; c++)
		{
			int k = max(0, min((int)floor(_V[r][c]/dw), nBins-1)) ;
			if (k<0 || k>=nBins) cout << k << " " << _V[r][c] << endl ;
			assert (k>=0 && k<nBins) ;
			
			double d = _V[r][c]-(2*k+1)*dw/2 ;
			int l = (d>0) ? k+1: k-1;
			l = max(0, min(nBins-1, l)) ;
			hist[l] += fabs(d/dw);
			hist[k] += 1-fabs(d/dw) ;
// 			if (d < 0)
// 			{
// 				cout << k << " " << d << " " << dw << " " << _V[r][c] << " " << (2*k+1)*dw/2 << " " << _V[r][c]-(2*k+1)*dw/2 << " " << fabs(d/dw) << " " << 1-fabs(d/dw) << endl ;
// 			} 
		}
	}
	
// 	Without Linear interpolation
// 	for (int r(0); r<nR; r++)
// 	{                
// 		for (int c(0); c<nC; c++)
// 		{
// 			int loc = min((int)floor(_V[r][c]*dw), nBins-1) ;
// 			if (loc<0 || loc>=nBins) cout << loc << " " << _V[r][c] << endl ;
// 			assert (loc>=0 && loc<nBins) ;
// 			hist[loc]+=1.0; 
// 		}
// 	}		
	int KSize(5) ;
	double Var(1) ;
	vector<double> oldhist(hist) ;
	for (int i(0); i<nBins; i++)
	{
		double blur (0), w(0)  ;
// 		cout << " i = " << i << endl ;
		for (int j(0); j<KSize; j++)
		{
			int k = i-KSize/2+j ;
			if (k<0 || k>=nBins) continue ;
// 			cout << " \t\t j " << j << " " << k << endl ; 
			const double wt = exp(-Var*pow((i-j)/(1.0*KSize),2.0)) ;
			blur += oldhist[k] * wt ; w += wt ;
			
// 			blur += oldhist[k] ;
// 			w ++ ;
		}
		if (w>1e-9) 
			hist[i] = blur/w ;
	}
// 		cin.ignore() ;
	double SumHist(0) ;
	for (int i(0); i<nBins; i++)
	{
		if (normalize) hist[i] /= (nR*nC) ;
		SumHist += hist[i] ;
		CumulHist[i] = SumHist ;
	}
	
}

                
DataMat::DataMat(int nRows, int nCols) 
{
	vector <double> V(nCols, 0.0) ;
	for (int i(0); i<nRows; i++) _V.push_back(V) ;
}

DataMat::DataMat(const DataMat& mat,int nRows, int nCols) 
{
	vector <double> V(nCols, 0.0) ;
	for (int i(0); i<nRows; i++) 
	{
		_V.push_back(V) ;
	
		for(int j=0;j<nCols;++j)
			operator()(i,j) = mat.Interp( i/(double)nRows * mat.NRows(), j/(double)nCols * mat.NCols() ) ;
	}
}

double DataMat::Interp(double i,double j) const
{
	int I = (int)floor(i) ;
	int J = (int)floor(j) ;
	double di(i-I) ;
	double dj(j-J) ;

	int Ip1 = min(I+1,NRows()-1) ;
	int Jp1 = min(J+1,NCols()-1) ;

	return (1.0 - di) * ( (1.0 - dj) * operator()(I  ,J) + dj * operator()(I  ,Jp1) )
			+ di  * ( (1.0 - dj) * operator()(Ip1,J) + dj * operator()(Ip1,Jp1) ) ;
}

DataMat::DataMat(const DataMat& DM) 
{               
	int nR(DM.NRows()) ;                
	for (int i(0); i<nR; i++) _V.push_back(DM._V[i]) ;
}

void DataMat::resize(const DataMat& D) 
{
	int nR(D.NRows()) ;                
	_V.resize(D._V.size()) ;
	for (int i(0); i<nR; i++) _V[i].resize(D._V[i].size()) ;
}
                                
const DataMat&  DataMat::operator += (const DataMat& D) 
{
	int minNR(min(NRows(), D.NRows())), minNC(min(NCols(), D.NCols())) ;
	for (int i(0); i<minNR; i++)
		for(int j(0); j<minNC; j++)
			_V[i][j] += D._V[i][j] ;

	return *this ;
}

const DataMat&  DataMat::operator *= (const DataMat& D) 
{
	int minNR(min(NRows(), D.NRows())), minNC(min(NCols(), D.NCols())) ;
	for (int i(0); i<minNR; i++)
		for(int j(0); j<minNC; j++)
			_V[i][j] *= D._V[i][j] ;

	return *this ;
}

const DataMat&  DataMat::Assign (const DataMat& D, double d) 
{
	int minNR(min(NRows(), D.NRows())), minNC(min(NCols(), D.NCols())) ;        
	for (int i(0); i<minNR; i++)
		for(int j(0); j<minNC; j++)
			_V[i][j] = d*D._V[i][j] ;

	return *this ;
}

const DataMat&  DataMat::operator = (const DataMat& D) 
{
    _V = D._V ;
	return *this ;
}

const DataMat&  DataMat::operator -= (const DataMat& D) 
{
	int minNR(min(NRows(), D.NRows())), minNC(min(NCols(), D.NCols())) ;
	for (int i(0); i<minNR; i++)
		for(int j(0); j<minNC; j++)
			_V[i][j] -= D._V[i][j] ;

	return *this ;
}

const DataMat&  DataMat::operator -= (double d) 
{
    	for (int i(0); i<NRows(); i++)
		for(int j(0); j<NCols(); j++)
			_V[i][j] -= d ;
    return *this; 
}

const DataMat&  DataMat::operator *= (double d) 
{
    	for (int i(0); i<NRows(); i++)
		for(int j(0); j<NCols(); j++)
			_V[i][j] *= d ;
    return *this ;
}

double DataMat::GetDR() const 
{        
	int NR(NRows()), NC(NCols()) ;
	if (!NR) return 0 ;
	double Max(_V[0][0]), Min(_V[0][0]) ;
	for (int i(0); i<NR; i++)
		for(int j(0); j<NC; j++)
	{
		Max = max(_V[i][j], Max) ;
		Min = min(_V[i][j], Min) ;
	}
	return Max-Min ;
}

void DataMat::Open(const string& Fname) 
{
	int nR(0), nC(0); 
	ifstream ifsbin(Fname.c_str(), ios::binary) ;
        if (!ifsbin) cerr << "Cannot open file " << Fname << endl ;
	ifsbin.read((char*)&nR, sizeof(int)) ;
	ifsbin.read((char*)&nC, sizeof(int)) ;
// 	cout << nR  << " " << nC << endl ;
	resize(nR, nC); 
	double *d = new double[nR*nC] ;
	ifsbin.read((char*)d, sizeof(double)*nR*nC) ;
	for (int r(0), i(0); r<nR; r++) for(int c(0); c<nC; c++) _V[r][c]=d[i++] ;
	ifsbin.close() ;
	delete[] d ;
}

void DataMat::BinaryWrite(const string& fname, bool Verbose) const
{
	if (_V.size()==0) return; 
	int nR(NRows()), nC(NCols()) ;
	if (Verbose) cout << "Writing " << fname << " of size " << nR << " x " << nC << endl ;
	ofstream ofsbin(fname.c_str(), ios::binary) ;
	ofsbin.write((char*)&nR, sizeof(int)) ;
	ofsbin.write((char*)&nC, sizeof(int)) ;
	double *arr = new double[nR*nC] ;
	for (int r(0), i(0); r<nR; r++) for(int c(0); c<nC; c++) arr[i++]=_V[r][c]; 
	ofsbin.write((char*)arr, sizeof(double)*nR*nC) ;
	ofsbin.close(); 

	delete[] arr ;
}

/*void DataMat::PopulateWithFunc(const RBFRep& F) 
{
	int NR(NRows()), NC(NCols()) ;
	for (int i(0); i<NR; i++)
		for(int j(0); j<NC; j++)
	{
		VecGSL p(2) ;
		p(0) = i ;
		p(1) = j ;
		_V[i][j] = F(p) ;
	}        
}
*/
int DataMat::InflPoints(vector<pair<int, int> >& p, double Thresh) const 
{
	int NR(NRows()), NC(NCols()) ;
//         for (int i(0); i<NR; i++) { p.push_back(pair<int,int>(i, 0)) ; p.push_back(pair<int,int>(i, NC-1)) ; }
//         for (int i(0); i<NC; i++) { p.push_back(pair<int,int>(0, i)) ; p.push_back(pair<int,int>(NR-1, i)) ; }        
	p.push_back(pair<int,int>(0, 0)) ; p.push_back(pair<int,int>(0, NC-1)) ;
	p.push_back(pair<int,int>(0, NC-1)) ; p.push_back(pair<int,int>(NR-1, NC-1)) ; 
	for (int i(1); i<NR-1; i++)
	{
		for(int j(1); j<NC-1; j++)
		{
			double Lap = 8*_V[i][j] - _V[i-1][j-1] - _V[i-1][j] - _V[i-1][j+1]
						- _V[i][j-1] - _V[i][j] - _V[i][j+1]
						- _V[i+1][j-1] - _V[i+1][j] - _V[i+1][j+1] ;
			if (fabs(Lap) < Thresh) p.push_back(pair<int,int>(i,j)) ;
		}        
	}
	return p.size() ;  
}
                
void DataMat::AddNoise(double minVal, double maxVal, double Prob) 
{
	int NR(NRows()), NC(NCols()) ;
	for (int i(0); i<NR; i++)
	{
		for(int j(0); j<NC; j++)
		{
			double Pr = rand()/(double)RAND_MAX ;
			if (Pr<Prob)
			{
				double val = minVal + (maxVal-minVal) * rand()/(double)RAND_MAX ;
				_V[i][j] += val ;
			}
		}
	}
}
                        



double DataMat::MaxVal() const 
{
    int NR(NRows()), NC(NCols()) ;
    if (!(NR+NC)) return 0 ;
    double maxval(0.0) ;
    maxval = _V[0][0] ;
    for (int r(0); r<NR; r++)
	{                
		for (int c(0); c<NC; c++)
		{
		    maxval = max(maxval,_V[r][c]) ;
		}
	}
	return maxval ;
}


void 	DataMat::ComputeGradientMagSq(DataMat& g) const
{
    const int h(NRows()), w(NCols()) ;
    g.resize(h,w);
    const DataMat& t(*this) ;
    for(int i(0); i<h-1; i++)
	for(int j(0); j<w-1; j++)
	{	    
	    const double gx = 0.5 * (t(i,j+1)-t(i,j) + t(i+1,j+1)-t(i+1,j))  ;
	    const double gy = 0.5 * (t(i+1,j)-t(i,j) + t(i+1,j+1)-t(i,j+1))  ;
	    g(i,j) = gx*gx+gy*gy ;
	}
}

// // Fit parab exactly to N4 + central pixel
// int DataMat::StationaryPointsParab(vector<pair<int, int> >& mins, vector<pair<int, int> >& maxs, double Thresh) const 
// {
// 	const int KSize(5) ;
// 	int NR(NRows()), NC(NCols()) ;
// 	PFMImage Viz(NC, NR) ;        
//         
// 	double d[] =  {
// 		1,   0,  -1,   0,   1,   
//   0,   1,   0,   1,   1,   
//   0,   0,   0,   0,   1,
//   1,   0,   1,   0,   1,   
//   0,   1,   0,  -1,   1,   
// 	};
// 	MatGSL A(5,5,d) ;
// 	PermGSL p(5) ;
// 	int signum;        
// 	gsl_linalg_LU_decomp (A, p, &signum) ;
// 	map<int, bool> Already ;
// 	for (int r(1); r<NR-1; r++)
// 	{                
// 		for (int c(1); c<NC-1; c++)
// 		{
// 			VecGSL b(5) ;
// 			b(0) = _V[r-1][c] ;
// 			b(1) = _V[r][c+1] ;
// 			b(2) = _V[r][c] ;
// 			b(3) = _V[r+1][c] ;
// 			b(4) = _V[r][c-1] ;
// 			VecGSL x(5) ;
// 			gsl_linalg_LU_solve(A, p, b, x)  ;
// 			if (fabs(x(0)) < 1e-5 || fabs(x(1))<1e-5) continue ;
// 			double xc = x(2)/(-2*x(0)) ;
// 			double yc = x(3)/(-2*x(1)) ;
//                         
// 			if (xc<-1 || xc>1 || yc<-1 || yc>1) continue ;
//                         
// 			int maxr(r) ;
// 			if (xc>0.5) maxr = r+1 ; else if (xc<-0.5) maxr = r-1 ;
// 			int maxc(c) ;
// 			if (yc>0.5) maxc = c+1 ; else if (yc<-0.5) maxc = c-1 ;
//                         
// 			if (Already[maxr*NC+maxc]) continue ;
//                         
// 			double c1 = x(4)- (x(0)*xc*xc + x(1)*yc*yc) ;
// 			if (fabs(c1-_V[maxr][maxc])>Thresh) continue ;
//                         
// 			if (x(0)<0) { maxs.push_back(pair<int, int>(maxr, maxc)) ; Viz(maxr, maxc).g = 1; Already[maxr*NC+maxc] = true; }
// 			else if (x(0)>0) {mins.push_back(pair<int, int>(maxr, maxc)) ; Viz(maxr, maxc).r = 1; Already[maxr*NC+maxc] = true; }                        
// 		}
// 	}
// //         for (int r(0); r<NR; r++) 
// //         {
// //                 mins.push_back(pair<int,int>(r, 0)); maxs.push_back(pair<int,int>(r, 0));
// //                 mins.push_back(pair<int,int>(r, NC-1)); maxs.push_back(pair<int,int>(r, NC-1));
// //                 Viz(r,0).r = 1 ; Viz(r,NC-1).r = 1 ; Viz(r,0).g = 1 ; Viz(r,NC-1).g = 1 ;
// //         }
// //         for (int c(0); c<NC; c++) 
// //         {
// //                 mins.push_back(pair<int,int>(0, c)); maxs.push_back(pair<int,int>(0, c));
// //                 mins.push_back(pair<int,int>(NR-1, c)); maxs.push_back(pair<int,int>(NR-1, c));
// //                 Viz(0,c).r = 1 ; Viz(NR-1,c).r = 1 ; Viz(0,c).g = 1 ; Viz(NR-1,c).g = 1 ;
// //         }
//         
// 	stringstream ss ;
// 	string str("./output/debug"); 
// 	int suffix(-1) ;
// 	ss << str  ;  
// 	if (suffix>=0) ss << suffix ;
// 	ss << ".exr" ;
//         
// 	sort(mins.begin(), mins.end() ) ;
// 	sort(maxs.begin(), maxs.end() ) ;
// 	Viz.Write(ss.str().c_str()) ;
// 	return mins.size()+maxs.size() ;       
// }
// 
// // Least squares fit to pixels in a certain window
// int DataMat::StationaryPointsParabMult(vector<pair<int, int> >& mins, vector<pair<int, int> >& maxs, double Thresh) const 
// {
// 	const int KSize(3) ;
// 	const int NR(NRows()), NC(NCols()) ;
// 	PFMImage Viz(NC, NR) ;        
// 	int xpow[6] = {2, 0, 1, 1, 0, 0} ;
// 	int ypow[6] = {0, 2, 1, 0, 1, 0} ;
//         
// 	MatGSL A(6,6) ;
// 	for (int r(0); r<6; r++)
// 	{
// 		for(int c(0); c<6; c++)
// 		{
// 			double sum(0) ;
// 			for (int i(0); i<KSize; i++)
// 			{
// 				for(int j(0); j<KSize; j++)
// 				{
// 					int x(-KSize/2+i) ;
// 					int y(-KSize/2+j) ;
//                                          
// 					sum += pow((double)x, xpow[r]+xpow[c]) * pow((double)y, ypow[r]+ypow[c]) ;
// 				}                                
// 			}
// 			A(r,c) = sum ;                                        
// 		}
// 	}        
// 	PermGSL p(6) ;
// 	int signum;        
// 	gsl_linalg_LU_decomp (A, p, &signum) ;
// 	double Zeros[6] = {0,0,0, 0,0,0};
// 	map<int, bool> Already ;
//         
// 	for (int r(KSize/2); r<NR-KSize/2; r++)
// 	{                
// 		for (int c(KSize/2); c<NC-KSize/2; c++)
// 		{                     
// //                         cout << r << ":" << c << "   " ;    
// 			VecGSL b(6, Zeros) ;                        
// 			for (int i(0); i<KSize; i++)
// 			{
// 				for(int j(0); j<KSize; j++)
// 				{
// 					int x(-KSize/2+i) ;
// 					int y(-KSize/2+j) ;
//                                         
// 					DEBUGPixel
// 					{ cout << " \t " << _V[x+r][y+c] ;} 
// 					for(int term(0); term<6; term++)
// 					{
// 						b(term) += pow(double(x), xpow[term]) * pow(double(y), ypow[term]) * _V[x+r][y+c] ;
// 					}
// 				}                                
// 				DEBUGPixel
// 				{ cout << endl;} 
// 			}                        
// 			VecGSL x(6) ;
// 			gsl_linalg_LU_solve(A, p, b, x)  ;                        
// 			double denom(4*x(0)*x(1) - x(2)*x(2)) ;
// 			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                         // No Minimum in both directions, test each individually
// 			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 			if(CloseToZero(denom))  
// 			{
// 				int ExtremaType(0) ; // -1 for minima and 1 for maxima
// 				double exR(0), exC(0) ; // initialize with central pixel
// 				if(CloseToZero(x(2))) // C = 0, so extrema is either in d/dr. i.e. r = -D/2A or in d/dc. i.e. c = -E/2B
// 				{
// 					if(!CloseToZero(x(0)))
// 					{
// 						exR = -0.5*x(3)/x(0) ;
// 						if ((exR>-KSize/2 & exR<KSize/2)) ExtremaType = (x(0) > 0) ? -1: 1 ;
// 					}
// 					else if(!CloseToZero(x(1))) // B != 0, so extrema is in d/dc. 
// 					{
// 						exC = -0.5*x(4)/x(1); 
// 						if ((exC>-KSize/2 && exC<KSize/2)) ExtremaType = (x(1) > 0) ? -2: 2 ;
// 					}                                        
// 				}
// 				if (ExtremaType==-1 || ExtremaType==1) // Max/Min along row
// 				{
// 					int Xri(exR + r) ;                                        
// 					for (int ind(0); ind<KSize; ind++) // max/min at all columns
// 					{ 
// 						int ci(c-KSize/2+ind) ;                                                
// 						if(!Already[Xri*NC+ci])
// 						{
// 							if(ExtremaType<0) mins.push_back(pair<int,int>(Xri, ci)) ; else maxs.push_back(pair<int,int>(Xri, ci)) ;
// 							if(ExtremaType<0) Viz(Xri, ci).r=1 ; else Viz(Xri, ci).g=1 ;
// 							Already[Xri*NC+ci] = true ;
// 						} 
// 					}
// 				}
// 				if (ExtremaType==-2 || ExtremaType==2) // Max/Min along column
// 				{
// 					int Xci(exC + c) ;
// 					for (int ind(0); ind<KSize; ind++) // max/min at all rows
// 					{ 
// 						int ri(r-KSize/2+ind) ;                                                
// 						if(!Already[ri*NC+Xci])
// 						{
// 							if(ExtremaType<0) mins.push_back(pair<int,int>(ri, Xci)) ; else maxs.push_back(pair<int,int>(ri, Xci)) ;
// 							if(ExtremaType<0) Viz(ri, Xci).r=1 ; else Viz(ri, Xci).g=1 ;
// 							Already[ri*NC+Xci] = true; 
// 						} 
// 					}
// 				}
// 				DEBUGPixel
// 				{
// 					cout << " --> " << ExtremaType << ":  " << exR << " " << exC << endl ;
// 				}
// 				continue; 
// 			}                                
// 			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                         //                      There is a true extremum, check if it is inside kernel span
// 			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 			double DerivMatElems[4] = {2*x(1), -x(2), -x(2), 2*x(0)} ;
// 			MatGSL Deriv(2,2, DerivMatElems);
// 			double rhsElems[2] = {-x(3), -x(4)} ;
// 			VecGSL rhs(2, rhsElems);
// 			VecGSL Stn(2) ;                        
// 			Stn = 1.0/denom*Deriv*rhs ;                                                
// 			DEBUGPixel
// 			{
// 				cout << " --> " << ~Stn << endl ;
// 			}
// 			if (Stn(0)<-KSize/2 || Stn(1)<-KSize/2 || Stn(0)>KSize/2 || Stn(1)>KSize/2) continue ;
//                         
// 			int maxr(r+Stn(0)) ;                        
// 			int maxc(c+Stn(1)) ;                                                
//                         
// 			if (Already[maxr*NC+maxc] || fabs (_V[r][c]-x(5))>Thresh) 
// 			{
// 				Viz(r, c).b = 10; 
// //                                 cout << " ---> " << maxr << " " << maxc << "\t";                                
// 				continue ;
// 			}
//                         
// 			if (x(2)>0) { maxs.push_back(pair<int, int>(maxr, maxc)) ; Viz(maxr, maxc).g = 1; Already[maxr*NC+maxc] = true;}
// 			else if (x(2)<0) {mins.push_back(pair<int, int>(maxr, maxc)) ; Viz(maxr, maxc).r = 1; Already[maxr*NC+maxc] = true; }                        
// 		}
// //                 cout << endl ;
// 	}
// //         for (int r(0); r<NR; r++) 
// //         {
// //                 mins.push_back(pair<int,int>(r, 0)); maxs.push_back(pair<int,int>(r, 0));
// //                 mins.push_back(pair<int,int>(r, NC-1)); maxs.push_back(pair<int,int>(r, NC-1));
// //                 Viz(r,0).r = 1 ; Viz(r,NC-1).r = 1 ; Viz(r,0).g = 1 ; Viz(r,NC-1).g = 1 ;
// //         }
// //         for (int c(0); c<NC; c++) 
// //         {
// //                 mins.push_back(pair<int,int>(0, c)); maxs.push_back(pair<int,int>(0, c));
// //                 mins.push_back(pair<int,int>(NR-1, c)); maxs.push_back(pair<int,int>(NR-1, c));
// //                 Viz(0,c).r = 1 ; Viz(NR-1,c).r = 1 ; Viz(0,c).g = 1 ; Viz(NR-1,c).g = 1 ;
// //         }
// //         cout << NR << " " << NC << endl ;
//         
// 	stringstream ss ;
// 	string str("./output/debug"); 
// 	int suffix(-1) ;
// 	ss << str  ;  
// 	if (suffix>=0) ss << suffix ;
// 	ss << ".exr" ;
//         
// 	sort(mins.begin(), mins.end() ) ;
// 	sort(maxs.begin(), maxs.end() ) ;
// 	for (int i(1);i<mins.size(); i++) 
// 		if (mins[i].first== mins[i-1].first && mins[i].second==mins[i-1].second ) cout << mins[i].first << " " << mins[i].second << endl ;
// //         for (int i(1);i<mins.size(); i++) 
// //                 if(mins[i].first ==100) cout << mins[i].first << " " << mins[i].second << endl ;
// 	for (int i(1);i<maxs.size(); i++) 
// 		if (maxs[i].first== maxs[i-1].first && maxs[i].second==maxs[i-1].second ) cout << "\t\t" << maxs[i].first << " " << maxs[i].second << endl ;
// //         for (int i(1);i<maxs.size(); i++) 
// //                 if(maxs[i].first ==100) cout << "\t\t\t\t" << maxs[i].first << " " << maxs[i].second << endl ;
// 	Viz.Write(ss.str().c_str()) ;
// 	return mins.size()+maxs.size() ;       
// }
