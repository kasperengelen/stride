import csv
import matplotlib.pyplot as plt
import os
import random
import sys

from pystride.Event import Event, EventType
from pystride.PyController import PyController

def trackCases(simulator, event):
    """
        Callback function to track cumulative cases
        after each time-step.

        Writes timestep data into a "cases.csv" file that can later be used for plotting
    """
    outputPrefix = simulator.GetConfigValue("run.output_prefix")
    timestep = event.timestep
    cases = simulator.GetPopulation().GetInfectedCount()

    if timestep == 0: # begin a new file, so erase present file
        mode = "w"
    else: # continue existing file, so append mode
        mode = "a"
    
    with open(os.path.join(outputPrefix, "cases.csv"), mode) as csvfile:
        fieldnames = ["timestep", "cases"]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        if timestep == 0:
            writer.writeheader()
        writer.writerow({"timestep": timestep, "cases": cases})
        
def computeAvgCases(outprefix, R0_vals, n_iterations):

    # Compute Averages
    all_avgs = list()
    for r0 in R0_vals:
        level_cases = list()
        for iteration in range(n_iterations):
            inputmap = "{}_{}_{}".format(outprefix, r0, iteration)
            days = list()
            newCasesPerDay = list()
            prevCumulativeCases = 0 # Keep track of how many cases have been recorded until current time-step
            with open(os.path.join(inputmap, "cases.csv"), "r") as csvfile:
                reader = csv.DictReader(csvfile)
                for row in reader:
                    days.append(int(row["timestep"]))
                    cumulativeCases = int(row["cases"])
                    newCasesPerDay.append(cumulativeCases - prevCumulativeCases)
                    prevCumulativeCases = cumulativeCases
            level_cases.append(newCasesPerDay)
        # Compute avg over iterations in this level
        level_avgs = list()
        for timestep in range(len(level_cases[0])):
            s = 0
            for iteration in level_cases:
                s += iteration[timestep]
            s /= len(level_cases)
            level_avgs.append(int(s))
        all_avgs.append(level_avgs)
    
    # Save to csv
    for lIx in range(len(all_avgs)):
        fname = "{}_avgs.csv".format(R0_vals[lIx])
        with open(fname, "w") as csvfile:
            fieldnames = ["timestep", "cases"]
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
            writer.writeheader()
            for i in range(len(all_avgs[lIx])):
                writer.writerow({"timestep": i, "cases": all_avgs[lIx][i]})
        

def plotCases(R0_vals, seed):
    """
        Plot new cases per day for a list of vaccination levels. (That have previously been simulated!)
    """
    given_days = [0, 13, 14, 25, 26, 35, 39, 43, 48]
    given_cases = [1, 1, 3,  3,  12, 15, 45, 45, 80]
    plt.plot(given_days, given_cases)
    for r0 in R0_vals:
        days = list()
        cases = list()
        with open("{}_avgs.csv".format(r0), "r") as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                days.append(int(row["timestep"]))
                cases.append(int(row["cases"]))
        plt.plot(days, cases)
    plt.xlabel("Simulation day")
    plt.ylabel("New cases per day")
    legend = ["Reference"] + [str(r0) for r0 in R0_vals]
    plt.legend(legend)
    #plt.show()
    plotname = "R0_plot.svg"
    plt.gcf().savefig(plotname)
    plt.clf()


def runSimulation(output_prefix, r0, rng_seed, iteration=1):
    """Run the simulator, for one immunity level."""
    # Set up simulator
    control = PyController(data_dir="data")
    # Load configuration from file
    cfile = os.path.join("config", "outbreak_2019_estimates.xml")
    control.loadRunConfig(cfile)
    # Set some parameters
    control.runConfig.setParameter("immunity_profile", "Random")    
    control.runConfig.setParameter("immunity_rate", 70.5 / 100)
    control.runConfig.setParameter("rng_seed", rng_seed)
    control.runConfig.setParameter("vaccine_profile", "None")
    control.runConfig.setParameter("r0", r0)
    outputmap = "{}_{}_{}".format(output_prefix, r0, iteration)
    control.runConfig.setParameter("output_prefix", outputmap)
    control.registerCallback(trackCases, EventType.Stepped)
    # Run simulation
    control.control()

def main(iterations = 1):
    output_prefix = "1_4_out/R0"
    R0_vals = [14, 15, 16]

    for i in range(iterations):
        # determine seed
        rng_seed = random.randint(0,10000000)
        print("Using seed " + str(rng_seed))
		
        # Run simulations
        for r0 in R0_vals:
            runSimulation(output_prefix, r0, rng_seed, i)

    # Post-processing
    computeAvgCases(output_prefix, R0_vals, iterations)
    plotCases(R0_vals, "0")

if __name__=="__main__":
    if len(sys.argv) == 1:
        iter_count = 1
    else:
        iter_count = int(sys.argv[1])
    
    main(iter_count)
