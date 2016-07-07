function out = ColectConvData (ns, nr, ofile, binfile, sstructs, istructs)

nsamps = num2str(ns) ;
nreps  = num2str(nr) ;
delete(ofile);

nstypes = length(sstructs) ;
nitypes = length(istructs) ;

for i=1:nstypes
    stype = sstructs(i).stype ;
    sarg = sstructs(i).sarg ;
    for j=1:nitypes
        itype = istructs(i).itype ;
        iarg = istructs(i).iarg ;
        execstr = [binfile ' -S --stype ' stype ' ' sarg ' -I --itype ' itype ' ' iarg ' -A --atype var --nsamps  ' nsamps ' --nreps ' nreps ' -G --ofile ' ofile] ; 
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
