#!/usr/bin/env python

from os import system
import re

liftRe = re.compile("Total lift = ([-0-9.]+)")
dragRe = re.compile("Total drag = ([-0-9.]+)")

for mesh in ["camber.mesh","circle.mesh","diamond.mesh","naca0012.mesh","pig.mesh","prius.mesh"]:
   for mach_number in ["0.6","0.8","1.0"]:
      for density in ["1.0","0.75","0.5"]:
         for angle_of_attack in ["-5.0","0.0","5.0","10.0","15.0","20.0","25.0"]:
            print "Simulating", mesh, "at mach", mach_number, "density", density, "and angle of attack", angle_of_attack

            #setup input file
            f = open("input.in","w")
            f.write("Adaptation Code Input File\n")
            f.write("\n")
            f.write("accuracy  1       // Order of accuracy\n")
            f.write("viscosity 0       // 1 for viscous, 0 for inviscid\n")
            f.write("maxInIt   100     // maximum number of inner itterations for flow solver\n")
            f.write("maxOutIt  200     // maximum number of outer itterations for flow solver\n")
            f.write("maxOutS   1000    // maximum number of outer itterations for Winslow Smoother\n")
            f.write("AdFlag    0       // Adaptation Flag: 0-none, 1-pressure, 2-velocity, 3-mach\n")
            f.write("DFlag     0       // Derefinement Flag: 1-derefine, 0-don't derefine\n")
            f.write("SFlag     2       // Smooth Flag: 0-don't smooth, 1-winslow smooth, 2-Complex Winslow\n")
            f.write("p         2       // Adaptation length parameter\n")
            f.write("restart   0       // Restart Flag: 0-start from freestream, 1-start from restart file\n")
            f.write("design    0       // Design Flag: 1 if sensitivity derivatives are to be calculated\n")
            f.write("mach      "+mach_number+"    // Mach Number\n")
            f.write("AoA       "+angle_of_attack+"    // angle of attack (in degrees) for the flow solver\n")
            f.write("gamma     1.4     // ratio of specific heats\n")
            f.write("rho       "+density+"     // density\n")
            f.write("mu        1.0     // viscosity\n")
            f.write("CFLmin    1.0     // minimum CFL number\n")
            f.write("CFLmax    100.0   // maximum CFL number\n")
            f.write("iramp     100.0   // number of iterations to ramp CFL from min to max\n")
            f.write("Rthresh   0.001   // Refinement threshold   (if < 0, Rthresh = Af_mean + C_R*Af_sdev)\n")
            f.write("Dthresh   0.00001 // Derefinement threshold (if < 0, Dthresh = Af_mean - C_D*Af_sdev)\n")
            f.write("alpha     0.0     // Angle of Attack (in degrees) for the smoother (rotation angle)\n")
            f.close()

            #run case
            system("./solve.exe "+mesh+" > solver.out")

            #generate images
            system("tec360 -p screenshot.mcr")

            #attach lift & drag to images
            output = "".join([line for line in open("solver.out").readlines()])
            liftSearch = liftRe.search(output)
            dragSearch = dragRe.search(output)
            print liftSearch, dragSearch
            if liftSearch and dragSearch:
               lift = liftSearch.group(1)
               drag = dragSearch.group(1)
               liftDrag = "Lift: "+lift+" Drag: "+drag
               for filename in ["density.jpg","mach.jpg","pressure.jpg"]:
                  system("convert "+filename+" -background White -pointsize 24 label:'"+liftDrag+"' -gravity Center -append "+filename)

            #move output files
            base_filename = mesh[:mesh.find(".")]+"_m"+mach_number+"_d"+density+"_a"+angle_of_attack
            solution_filename = base_filename+".dat"
            density_filename = base_filename+"_density.jpg"
            mach_filename = base_filename+"_mach.jpg"
            pressure_filename = base_filename+"_pressure.jpg"
            print "Writing result to", solution_filename
            system("mv solution.dat results/"+solution_filename)
            system("mv density.jpg results/"+density_filename)
            system("mv mach.jpg results/"+mach_filename)
            system("mv pressure.jpg results/"+pressure_filename)
