import csv
import matplotlib.pyplot as plt
import os
import random
import sys

from pystride.Event import Event, EventType
from pystride.PyController import PyController

def trackCases(simulator, event, iteration):
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
    
    with open(os.path.join(outputPrefix, "cases{}.csv".format(iteration)), mode) as csvfile:
        fieldnames = ["timestep", "cases"]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        if timestep == 0:
            writer.writeheader()
        writer.writerow({"timestep": timestep, "cases": cases})
        
def computeAvgCases(outprefix, immunity_levels, seed):

    for level in immunity_levels:
        

def plotNewCases(output_prefix, immunity_levels, seed):
    """
        Plot new cases per day for a list of vaccination levels. (That have previously been simulated!)
    """
    given_days = [0, 13, 14, 25, 26, 35, 39, 43, 48]
    given_cases = [1, 1, 3,  3,  12, 15, 45, 45, 80]
    plt.plot(given_days, given_cases)
    for level in immunity_levels:
        days = []
        newCasesPerDay = []
        prevCumulativeCases = 0 # Keep track of how many cases have been recorded until current time-step
        with open(os.path.join(output_prefix + "_" + str(level), "cases.csv")) as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                days.append(int(row["timestep"]))
                cumulativeCases = int(row["cases"])
                newCasesPerDay.append(cumulativeCases - prevCumulativeCases)
                prevCumulativeCases = cumulativeCases
        plt.plot(days, newCasesPerDay)
    plt.xlabel("Simulation day")
    plt.ylabel("New cases per day")
    legend = ["Reference"] + [str(level) for level in immunity_levels]
    plt.legend(legend)
    #plt.show()
    plt.gcf().savefig("run_" + str(seed) + ".svg")
    plt.clf()


def runSimulation(output_prefix, immunity_level, rng_seed):
    """Run the simulator, for one immunity level."""
    # Set up simulator
    control = PyController(data_dir="data")
    # Load configuration from file
    cfile = os.path.join("config", "outbreak_2019_estimates.xml")
    control.loadRunConfig(cfile)
    # Set some parameters
    control.runConfig.setParameter("immunity_profile", "Random")    
    control.runConfig.setParameter("immunity_rate", immunity_level / 100)
    control.runConfig.setParameter("rng_seed", rng_seed)
    control.runConfig.setParameter("output_prefix", output_prefix + "_" + str(immunity_level))
    control.registerCallback(trackCases, EventType.Stepped)
    # Run simulation
    control.control()

def main(iterations = 1):
    output_prefix = "Immunity"
    immunity_levels = [70.5, 70.75, 71, 71.25, 71.5, 71.75] #[74, 75, 76, 77, 78]

	for _ in range(iterations):
        # determine seed
        rng_seed = random.randint(0,10000000)
        print("Using seed " + str(rng_seed))
		
        # Run simulations
        for level in immunity_levels:
            runSimulation(output_prefix, level, rng_seed)

    # Post-processing
    plotNewCases(output_prefix, immunity_levels, rng_seed)

if __name__=="__main__":
    if len(sys.argv) == 1:
        iter_count = 1
    else:
        iter_count = int(sys.argv[1])
    
    main(iter_count)
