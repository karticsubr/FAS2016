
stypes = {'Jittered' 'Random' 'BJittered' 'GJittered'};
     
sargs =  { '','','--boxwidth .5', '--sigma .5'};

itypes = {'QuadPix' 'PWConstant' 'PWConstant' 'PWConstant'}; 

iargs =  {  ...
            ['--points ' num2str(rand(1,4))], ...
            '--npts 10 --random', ...
            '--npts 100 --random', ...
            '--npts 1000 --random' ...
          };

ns = [9 36 100 512] ;
nr = 5;
ofile = '../out/test.txt';
binfile = '../eea/build/eea' ;
nsamps = num2str(ns) ;
nreps  = num2str(nr) ;
delete(ofile);

for i=1:length(stypes)
    stype = stypes{i} ;
    sarg = sargs{i} ;
    for j=1:length(itypes)
        itype = itypes{j} ;
        iarg = iargs{j}; 
        execstr = [binfile ' -S --stype ' stype ' ' sarg ' -I --itype ' itype ' ' iarg ' -A --atype var --nsamps  ' nsamps ' --nreps ' nreps ' -G --ofile ' ofile] ; 
        system(execstr);
    end
end


%%
M = csvread(ofile) ;
[h w] = size(M) ;
S = M(:, 1:w/2) ;
V = M(:,w/2+1:w) ;

figure(1) ;
clf; 

figure(2) ;
clf; 

ni = length(itypes) ;
ns = length(stypes) ;

cmap = lines(ns) ;
marker = 'o+*^><' ;
for i=1:h
    snum = (floor((i-1)/ns)) + 1;
    inum = mod(i-1,ni)+1 ;
    mk=marker(mod(inum,length(marker))) ;
    col = cmap(snum,:) ;
    figure(1) ;
    x = S(i,:) ; 
    y = V(i,:) ;
    loglog(x, y, ['--' mk], 'color', col) ;
    P = polyfit(log(x),log(y),1) ;
    hold on; 
    loglog(x, exp(polyval(P,log(x))), 'color', col) ;
    ax1 = gca ;
    
    figure(2) ;
    sp(i) = plot(P(1), exp(P(2)), mk, 'Color', col) ;
    ax2 = gca ;
    hold on; 
    legstr{snum}=stypes{snum} ;
    spidx(snum) = i ;
    legstr2{inum}=[itypes{inum} iargs{inum}] ;
    spidx2(inum) = i ;
end
figure(2) ;
legend([sp(spidx) sp(spidx2)], [legstr legstr2], 'location', 'NorthEast') ;

%%

stype = 'Jittered' ;
sarg = '';
itype = 'PWConstant' ;
iargs = '--npts 100  --random' ;
nsamps = '9 36 100 512' ;
nreps = '100 ' ;
execstr = [binfile ' -S --stype ' stype ' ' sarg ' -I --itype ' itype ' ' iargs ' -A --atype var --nsamps  ' nsamps ' --nreps ' nreps ' -G --ofile ' ofile] ;

execstr

%%

system(execstr) ;


%%



h_lines=plot(rand(20,8));

hold on;h_dashed=plot(rand(20,4)+2,'--')

hold on;h_syms=plot(4+rand(20,4),'linestyle','none','marker','+');

leg1=legend(h_lines(1:4),'First','Second','Third','Fourth','location', 'south')

copied_leg1=copyobj(leg1,gcf)

% leg2=legend(h_dashed(1:4),'1','2','3','4','location','west')

% copied_leg2=copyobj(leg2,gcf)

% leg3=legend(h_syms(1:2),'symbol_1','symbol_2','location','north')

































