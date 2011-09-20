#CC	= icpc -O2
#CC	= g++ -O0 -g
CC	= g++ -O2

INCLUDE_PATH   = -I./include

COBJ = driver.o convert.o flowSolve.o readinput.o readmesh.o readCmplxmesh.o tristat.o fluxp.o fluxm.o dfluxp.o dfluxm.o dfluxWall.o lu_solve.o gplot.o tplot.o pplot.o gradx.o grady.o fluxV.o dfluxV.o List.o cost.o printResid.o

designOBJ = optimization.o designFlowSolve.o fluxm_B.o fluxp_B.o dCostdQ.o transpose.o multipliers.o reverseMode.o dCostdX.o designPplot.o

main:	$(COBJ)
	$(CC) -o solve.exe *.o

design:	$(COBJ) $(designOBJ)
	$(CC) -o solve.exe *.o

flux:	fluxdrive.o fluxm.o fluxp.o fluxm_B.o fluxp_B.o
	$(CC) -o fluxtest.exe fluxdrive.o fluxm.o fluxp.o fluxm_B.o fluxp_B.o

.f.o	: 
	$(FC) -c $*.f

.cpp.o	: 
	$(CC) $(INCLUDE_PATH) -c $*.cpp

clean:
	rm -f *.o solve.exe solution.dat *.gnu tecplot.phy mesh_*.mesh restart-1.dat mesh.ps
