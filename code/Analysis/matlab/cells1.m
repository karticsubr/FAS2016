%% PBRT integrands
tic; 
clear ;
s(1) = GenSamplStruct('Jittered', '') ;
s(2) = GenSamplStruct('Random', '') ;

legend = 'SubSurf sph';
i(1) = GenIntegStruct('Pbrt', ...
            [ '--epath /home/kartic/pbrt/pbrt-v3/pbrt ' ...
            '--spath /home/kartic/pbrt/pbrt-v3-scenes/subusrface-spheres/spheres.pbrt '...
            '--pypath /home/kartic/Projects/FAS2016/code/Analysis/python/pbrt-cl.py '...
            '--crop 0.2 0.21 0.79 0.8 --pbrtstype $$SAMP$$ --refnspp 0 --img pbrt-eea.exr'], ...
            legend ) ;

legend = 'head' ;
i(2) = GenIntegStruct('Pbrt', ...
            [ '--epath /home/kartic/pbrt/pbrt-v3/pbrt ' ...
            '--spath /home/kartic/pbrt/pbrt-v3-scenes/head/head.pbrt '...
            '--pypath /home/kartic/Projects/FAS2016/code/Analysis/python/pbrt-cl.py '...
            '--crop 0.58 0.6 0.365 0.38 --pbrtstype $$SAMP$$ --refnspp 0 --img pbrt-eea.exr'], ...
            legend ) ;

% legend = 'glass' ;
% i(3) = GenIntegStruct('Pbrt', ...
%             [ '--epath /home/kartic/pbrt/pbrt-v3/pbrt ' ...
%             '--spath /home/kartic/pbrt/pbrt-v3-scenes/caustic-glass/glassBID.pbrt '...
%             '--pypath /home/kartic/Projects/FAS2016/code/Analysis/python/pbrt-cl.py '...
%             '--crop 0.5 0.51 0.5 0.51 --pbrtstype $$SAMP$$ --refnspp 0 --img pbrt-eea.exr'], ...
%             legend ) ;
% 
        ns = [4 16 64 1204] ;
nr =  50;
ofile = '../out/test';
binfile = '../build/eea' ;
atype = 'var' ;

system (['rm ' ofile '*.txt']) ;
data = CollectConvData(ns, nr, ofile, binfile, s, i, atype) ;
toc
%% Plot data generated by CollectConvData

PlotConvData(data) ;


%% Other integrands

clear; 
%%%%%%%%%%% samplers 
s(1) = GenSamplStruct('Jittered', '') ;
s(2) = GenSamplStruct('Random', '') ;
s(3) = GenSamplStruct('BJittered', '--boxwidth .3') ;
s(4) = GenSamplStruct('BJittered', '--boxwidth .5') ;
s(5) = GenSamplStruct('GJittered', '--sigma .3') ;
s(6) = GenSamplStruct('GJittered', '--sigma .5') ;
s(7) = GenSamplStruct('Grid', '') ;

%%%%%%%%%%% Integrands
i(1) = GenIntegStruct('QuadPix', ['--points ' num2str(rand(1,4))]) ;
i(2) = GenIntegStruct('PWConstant', '--npts 100 --random') ;
i(3) = GenIntegStruct('Gaussian', '--center .5 .5 --sigma .2 .4') ;
i(4) = GenIntegStruct('Disk', '--center .5 .5 --rad .3') ;
i(4) = GenIntegStruct('Disk', '--center .5 .5 --rad .3') ;

%%%%%%%%%%% Analysis Parameters
ns = [9 36 100 1024] ;
nr =  50;
ofile = '../out/test';
binfile = '../build/eea' ;
atype = 'var' ;

system (['rm ' ofile '*.txt']) ;
data = CollectConvData(ns, nr, ofile, binfile, s, i, atype) ;


%% Plot data generated by CollectConvData

PlotConvData(data) ;



