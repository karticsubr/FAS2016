clear; 
%%%%%%%%%%% samplers 
s(1) = GenSamplStruct('Jittered', '') ;
s(2) = GenSamplStruct('Random', '') ;
s(3) = GenSamplStruct('BJittered', '--boxwidth .3') ;
s(4) = GenSamplStruct('BJittered', '--boxwidth .5') ;
s(5) = GenSamplStruct('GJittered', '--sigma .3') ;
s(6) = GenSamplStruct('GJittered', '--sigma .5') ;

%%%%%%%%%%% Integrands
i(1) = GenIntegStruct('QuadPix', ['--points ' num2str(rand(1,4))]) ;
i(2) = GenIntegStruct('PWConstant', '--npts 10 --random') ;
i(3) = GenIntegStruct('PWConstant', '--npts 100 --random') ;
i(4) = GenIntegStruct('PWConstant', '--npts 1000 --random') ;

%%%%%%%%%%% Analysis Parameters
ns = [9 36 100 1024 1600] ;
nr = 500;
ofile = '../out/test.txt';
binfile = '../eea/build/eea' ;
atype = 'err' ;

data = CollectConvData(ns, nr, ofile, binfile, s, i, atype) ;


%% Plot data generated by CollectConvData

PlotConvData(data) ;



