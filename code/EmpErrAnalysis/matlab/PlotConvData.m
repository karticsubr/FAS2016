function pltout = PlotConvData (dout) 
fsz = 12;
maxlegwidth = 15 ;

ni = length(dout.istructs) ;
ns = length(dout.sstructs) ;
h = size(dout.S,1) ;
S = dout.S ;
V = dout.V ;

hf1 = figure('PaperPosition',[0 0 14 14],'PaperSize',[14 14]);

hf2 = figure('PaperPosition',[0 0 26 14],'PaperSize',[25 14]);

cmap = lines(ns) ;
marker = 'o+*^><' ;
for i=1:h
    snum = (floor((i-1)/ns)) + 1;
    inum = mod(i-1,ni)+1 ;

    mk=marker(mod(inum,length(marker))) ;
    col = cmap(snum,:) ;
    
    figure(hf1) ;
    x = S(i,:) ; 
    y = V(i,:) ;
    loglog(x, y, ['--' mk], 'color', col) ;
    P = polyfit(log(x),log(y),1) ;
    hold on; 
    loglog(x, exp(polyval(P,log(x))), 'color', col) ;
    
    figure(hf2) ;
    sp(i) = plot(P(1), exp(P(2)), mk, 'Color', col) ;
    hold on; 
    sp2(i) = plot(P(1), exp(P(2)), mk, 'Color', col) ;
    
    sa = dout.sstructs(snum).sarg ;
    legstr{snum}=[ dout.sstructs(snum).stype ' ' sa(1:min(length(sa), maxlegwidth)) '...'];
    spidx(snum) = i ;
    ia = dout.istructs(inum).iarg ;
    legstr2{inum}=[dout.istructs(inum).itype ' '  ia(1:min(length(ia), maxlegwidth)) '...'] ;
    spidx2(inum) = i ;
end
figure(hf1); 
xlabel('log-#samples')
ylabel('log-variance')
set(gca, 'fontsize', fsz) ;
print('../out/convs.pdf', '-dpdf') ;

figure(hf2) ;
xlabel('convergence rate')
ylabel('variance at low sampling')

set(gca, 'fontsize', fsz) ;
legend([sp(spidx) sp2(spidx2)], [legstr legstr2], 'location', 'NorthEastOutside', 'Fontsize', fsz) ;
print('../out/scatter.pdf', '-dpdf') ;

close all ;
end