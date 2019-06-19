import csv
import matplotlib.pyplot as plt
import os
import random
import sys
import datetime

from pystride.Event import Event, EventType
from pystride.PyController import PyController
from pystride.stride.pystride import Id_College

        
def trackCases(simulator, event):
    """
        Callback function to track cumulative cases
        after each time-step.
    """
    outputPrefix = simulator.GetConfigValue("run.output_prefix")
    timestep = event.timestep
    cases = simulator.GetPopulation().GetInfectedCount()
    with open(os.path.join(outputPrefix, "cases.csv"), "a") as csvfile:
        fieldnames = ["timestep", "cases"]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        if timestep == 0:
            writer.writeheader()
        writer.writerow({"timestep": timestep, "cases": cases})

def plotNewCases(outputPrefix, iterations, daycare_fractions, preschool_fractions, workplace_files, day, legend):
    """
        Plot new cases per day.
    """
    fig = plt.figure()
    days = range(0, day)
    for fraction_ix in range(len(daycare_fractions)):
        for workplace in workplace_files:
            totalNewCases = [0]*day
            for i in range(0, iterations):
                cumulativeCasesPerDay = []
                dc_frac = daycare_fractions[fraction_ix]
                ps_frac = preschool_fractions[fraction_ix]

                folderName = os.path.join(outputPrefix, f"DC-{dc_frac}-PS-{ps_frac}_{i}")
                    
                with open(os.path.join(folderName, "cases.csv")) as csvfile:
                    reader = csv.DictReader(csvfile, fieldnames=["timestep", "cases"])
                    row1 = next(reader)

                    for row in reader:
                        cumulativeCases = int(row["cases"])
                        cumulativeCasesPerDay.append(cumulativeCases)
                    csvfile.close()
                        
                prevCumulativeCases = cumulativeCasesPerDay[0] # Keep track of how many cases have been recorded until current time-step
                for j in range(len(cumulativeCasesPerDay)):
                    totalNewCases[j] += cumulativeCasesPerDay[j] - prevCumulativeCases
                    prevCumulativeCases = cumulativeCasesPerDay[j]
                    
            averageCasesPerDay = [i/iterations for i in totalNewCases]
            plt.plot(days, averageCasesPerDay)
        
    
    plt.xlabel("Simulation day")
    plt.ylabel("New cases per day")
    plt.legend(legend, title="Daycare/Preschool attendance")
    #plt.show()
    fig.savefig(os.path.join(outputPrefix, 'cases_per_day_{}runs.eps'.format(iterations)))


def plotCumulativeCases(outputPrefix, iterations, daycare_fractions, preschool_fractions, workplace_files, day, legend):
    """
        Plot cumulative cases per day.
    """
    fig = plt.figure()
    days = range(0, day)
    for fraction_ix in range(len(daycare_fractions)):
        for workplace in workplace_files:
            totalNewCases = [0]*day
            for i in range(0, iterations):
                cumulativeCasesPerDay = []
                dc_frac = daycare_fractions[fraction_ix]
                ps_frac = preschool_fractions[fraction_ix]

                folderName = os.path.join(outputPrefix, f"DC-{dc_frac}-PS-{ps_frac}_{i}")
                    
                with open(os.path.join(folderName, "cases.csv")) as csvfile:
                    reader = csv.DictReader(csvfile, fieldnames=["timestep", "cases"])
                    row1 = next(reader)

                    for row in reader:
                        cumulativeCases = int(row["cases"])
                        cumulativeCasesPerDay.append(cumulativeCases)
                    csvfile.close()
                        
                for j in range(len(cumulativeCasesPerDay)):
                    totalNewCases[j] += cumulativeCasesPerDay[j]
                    
            averageCasesPerDay = [i/iterations for i in totalNewCases]
            plt.plot(days, averageCasesPerDay)
        
    
    plt.xlabel("Simulation day")
    plt.ylabel("Cumulative cases per day")
    plt.legend(legend, title="Daycare/Preschool attendance")
    #plt.show()
    fig.savefig(os.path.join(outputPrefix, 'cumulative_cases_{}runs.eps'.format(iterations)))

def runSimulation(outputPrefix, daycare_fraction, preschool_fraction, workplace, iteration, rng_seed, days):
    print(f"DC: {daycare_fraction} / PS: {preschool_fraction} - Iteration {iteration}")
    # Set up simulator
    control = PyController(data_dir="data")
    # Load configuration from file
    control.loadRunConfig(os.path.join("config", "daycare-preschool.xml"))
    # Set some parameters

    outputPrefix = os.path.join(outputPrefix, f"DC-{daycare_fraction}-PS-{preschool_fraction}_{iteration}")

    control.runConfig.setParameter("output_prefix", outputPrefix)
    control.runConfig.setParameter("rng_seed", rng_seed)

    control.runConfig.setParameter("num_days", str(days))
    control.runConfig.setParameter("geopop_gen.population_size", "600000")
    control.runConfig.setParameter("geopop_gen.participation_preschool", preschool_fraction)
    control.runConfig.setParameter("geopop_gen.participation_daycare", daycare_fraction)

    control.registerCallback(trackCases, EventType.Stepped)
    # Run simulation
    control.control()
    print("Simulation done")

def main():
    iterations = 20
    days = 50
    outputPrefix = os.path.join("daycare-preschool", f"{iterations}runs_{days}days_seeding0.002")
    # These must be the same size, they'll line up in the simulated scenarios
    daycare_fractions   = [0.2, 0.4, 0.6, 0.8]
    preschool_fractions = [0.2, 0.4, 0.6, 0.8]
    workplace_files = ["workplace_size_distribution2.csv"]

    # Run simulations
    for fraction_ix in range(len(daycare_fractions)):
        for workplace in workplace_files:
            for iteration in range(iterations):
                rng_seed = random.randint(0, 10000000)
                runSimulation(outputPrefix, daycare_fractions[fraction_ix], preschool_fractions[fraction_ix], workplace, iteration, rng_seed, days)

    legend = list()
    for fraction_ix in range(len(daycare_fractions)):
            dc_frac = daycare_fractions[fraction_ix]
            ps_frac = preschool_fractions[fraction_ix]
            legend.append(f"DC: {dc_frac} / PS: {ps_frac}")
    # Post-processing
    plotNewCases(outputPrefix, iterations, daycare_fractions, preschool_fractions, workplace_files, days, legend)
    plotCumulativeCases(outputPrefix, iterations, daycare_fractions, preschool_fractions, workplace_files, days, legend)
    print("Simulation files stored in {}".format(outputPrefix))


if __name__=="__main__":
    random.seed(datetime.datetime.now())
    main()
    