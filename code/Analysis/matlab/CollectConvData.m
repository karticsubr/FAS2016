function out = ColectConvData (ns, nr, ofile, binfile, sstructs, istructs, atype)

nsamps = num2str(ns) ;
nreps  = num2str(nr) ;
delete(ofile);

nstypes = length(sstructs) ;
nitypes = length(istructs) ;

for i=1:nstypes
    stype = sstructs(i).stype ;
    sarg = sstructs(i).sarg ;
    dispstr = ['Collecting data using ' stype ' ' sarg '...'] ; 
    disp(dispstr) ;
    for j=1:nitypes
        itype = istructs(j).itype ;
        iarg = istructs(j).iarg ;
        dispstr = ['     on integrand ' itype ' ' iarg '...'] ; 
        disp(dispstr) ;
        execstr = [binfile ' -S --stype ' stype ' ' sarg ' -I --itype ' itype ' ' iarg ' -A --atype ' atype ' --nsamps  ' nsamps ' --nreps ' nreps ' -G --ofile ' ofile] ; 
        system(execstr);
    end
end

M = csvread(ofile) ;
[h w] = size(M) ;

out.ns = ns ;
out.nr = nr ;
out.ofile = ofile; 
out.S = M(:, 1:w/2) ;
out.V = M(:,w/2+1:w) ;
out.sstructs = sstructs ; 
out.istructs = istructs ;
