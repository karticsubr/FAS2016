#ifndef _DATAMAT_H_
#define _DATAMAT_H_

#include <vector>
#include<iostream>
#include <iterator>
#include <map>
#include <cmath>
#include <algorithm>
#include <functional>  
using namespace std; 

typedef vector<double> DataVec ;

class DataMat
{
	private:
		vector<vector<double> > _V ;
        
	public:                
//                 typedef vector<double>::value_type             value_type;
//                 typedef vector<double>::allocator_type         allocator_type;
  
//                 typedef vector<vector<double> >::size_type              size_type;
//                 typedef vector<vector<double> >::difference_type        difference_type;

		typedef vector<double>::iterator               iterator;
		typedef vector<double>::const_iterator         const_iterator;

//                 typedef vector<vector<double> >::reverse_iterator       reverse_iterator;
//                 typedef vector<vector<double> >::const_reverse_iterator const_reverse_iterator;

//                 typedef vector<vector<double> >::reference              reference;
//                 typedef vector<vector<double> >::const_reference        const_reference;
                

		DataMat(int nRows=0, int nCols=0) ;
		DataMat(const DataMat& DM) ;
		DataMat(const DataMat& DM, int nRows, int nCols) ; // Resample DM in a nRows*nCols DataMat.
		DataMat(const string& Fname)  {Open(Fname); }
                  
                double 	MaxVal() const ;
		void		Open(const string& Fname) ;
		int             NCols() const                                   { return (_V.size()>0) ? _V[0].size() : 0 ; }
		int             y_size() const                                   { return _V.size(); }
		int             x_size() const                                   { return (_V.size()>0) ? _V[0].size() : 0 ; }
		int             NRows() const                                   { return _V.size(); }
		void            resize(int NR, int NC)                          {_V.resize(NR); for (int i(0); i<NR; i++) _V[i].resize(NC); }
		void            resize(const DataMat& D) ;                
		const DataMat&  Assign (const DataMat& D, double d) ;
		const DataMat&  operator = (const DataMat& D) ;
		const DataMat&  operator += (const DataMat& D) ;
		const DataMat&  operator -= (const DataMat& D) ;
		const DataMat&  operator *= (const DataMat& D) ;
		const DataMat&  operator -= (double d) ;
		const DataMat&  operator *= (double d) ;
		double          GetDR() const ;
		double&         operator () (const pair<int, int>& p)           { return _V[p.first][p.second] ; }                
		const double&   operator () (const pair<int, int>& p) const     { return _V[p.first][p.second] ; }                
		const double&   operator()(int i, int j) const                  { return _V[i][j] ; }
		double&         operator()(int i, int j)                        { return _V[i][j] ; }
		
		void 		Histogram(vector<double>& hist, vector<double>& CumulHist, int nBins, bool normalize=true) const; 
                void            NormalizedGrad(DataMat& GradNorm) const ;
                int             InflPoints(vector<pair<int, int> >& p, double Thresh) const ;
		void            AddNoise(double minVal, double maxVal, double Prob) ;                
		void		BinaryWrite(const string& fname, bool Verbose=true) const ;
		void 		ComputeGradientMagSq(DataMat& g) const ;
		inline iterator       begin() {return _V.begin()->begin();}
		inline const_iterator begin() const {return _V.begin()->begin();}  
		inline iterator       end() {return _V.back().end();}
		inline const_iterator end() const {return _V.back().end();}
                                
		friend void     DataMatSubtract(DataMat& Diff, const DataMat& A, const DataMat& B) ;
		friend bool     TestConvergenceIntegSq(const DataMat& M, double Thresh) ;
		friend void     WriteDMatAsImage(const DataMat& Mat, const string& FName) ;
		friend void  	DMat2EXR(const DataMat& Mat, const string& Fname) ;
		friend void  	DMat2EXR2(const DataMat& Mat, const string& Fname) ;
//		friend void  	WriteColorEXR(const string& fname, const DataMat& R, const DataMat& G, const DataMat& B, double Gamma=2.2) ;
		friend ostream& operator <<(ostream& os, const DataMat& Mat) ;
                
	private:
		double          Interp(double i,double j) const ;
        
};

struct less_pair: public binary_function< const pair<int,int>&, const pair<int,int>& , bool> 
{  bool operator()(const pair<int,int>& p1, const pair<int,int>& p2) { return p1.first <= p2.first && p1.second < p2.second; } };

inline void DataMatSubtract(DataMat& Diff, const DataMat& A, const DataMat& B) 
{
	int minNR(min(A.NRows(), B.NRows())), minNC(min(A.NCols(), B.NCols())) ;
	Diff.resize(minNR, minNC) ;
	for (int i(0); i<minNR; i++)
		for(int j(0); j<minNC; j++)
			Diff._V[i][j] = A._V[i][j] - B._V[i][j] ;
}

inline bool TestConvergenceIntegSq(const DataMat& Mat, double Thresh) 
{
	int NR(Mat.NRows()), NC(Mat.NCols()) ;
	double Sum (0.0) ;
	for (int i(0); i<NR; i++)
		for(int j(0); j<NC; j++)
	{
		const double d = Mat._V[i][j] ;
		Sum += d*d ;
	}
	cout << "\t\t" << fabs(Sum)/(NR*NC) << " vs " << Thresh << endl ;
	return fabs(Sum)/(NR*NC) < Thresh;
}

inline bool IsPrettyClose(const DataMat& D1, const DataMat& D2, double Thresh)  
{
	DataMat Diff ;
	DataMatSubtract(Diff, D1, D2) ;
	return TestConvergenceIntegSq(Diff, Thresh) ;
}


inline ostream& operator <<(ostream& os, const DataMat& Mat) 
{
	int nR(Mat.NRows()), nC(Mat.NCols()) ;        
	for (int i(0); i<nR; i++)
	{
		for (int j(0); j<nC; j++)
		{       
			os << Mat._V[i][j] << " " ;                        
		}
		os << endl ;
	}               
	return os << endl ;
}

#endif //_DATAMAT_H_
