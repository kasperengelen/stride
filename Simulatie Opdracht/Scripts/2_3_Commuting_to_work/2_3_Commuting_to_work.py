import csv
import matplotlib.pyplot as plt
import os
import random
import sys

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

def plotNewCases(outputPrefix, iterations):
    """
        Plot new cases per day.
    """
    fig = plt.figure()
    days = range(0, 50)
    for fraction in [0.0, 0.25, 0.5, 0.75, 1.0]:
        totalNewCases = [0]*50
        for i in range(0, iterations):
            cumulativeCasesPerDay = []

            folderName = os.path.join(outputPrefix, str(fraction) + "_" + str(i))
                
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
    plt.legend(["0.0", "0.25", "0.5", "0.75", "1.0"])
    plt.show()
    fig.savefig(os.path.join(outputPrefix, 'cases_per_day_{}runs.eps'.format(iterations)))


def plotCumulativeCases(outputPrefix, iterations):
    """
        Plot cumulative cases per day.
    """
    fig = plt.figure()
    days = range(0, 50)
    for fraction in [0.0, 0.25, 0.5, 0.75, 1.0]:
        totalNewCases = [0]*50
        for i in range(0, iterations):
            cumulativeCasesPerDay = []

            folderName = os.path.join(outputPrefix, str(fraction) + "_" + str(i))
                
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
    plt.legend(["0.0", "0.25", "0.5", "0.75", "1.0"])
    plt.show()
    fig.savefig(os.path.join(outputPrefix, 'cumulative_cases_{}runs.eps'.format(iterations)))

def runSimulation(outputPrefix, fraction, iteration, rng_seed):
    # Set up simulator
    control = PyController(data_dir="data")
    # Load configuration from file
    control.loadRunConfig(os.path.join("config", "run_generate_default.xml"))
    # Set some parameters

    outputPrefix = os.path.join(outputPrefix, str(fraction) + "_" + str(iteration))

    control.runConfig.setParameter("geopop_gen.population_size", 10000)
    control.runConfig.setParameter("geopop_gen.participation_college", 0.0)
    control.runConfig.setParameter("geopop_gen.particpation_workplace", 1.0)
    control.runConfig.setParameter("geopop_gen.fraction_college_commuters", 0.0)
    control.runConfig.setParameter("geopop_gen.fraction_workplace_commuters", fraction)

    control.runConfig.setParameter("output_prefix", outputPrefix)
    control.runConfig.setParameter("rng_seed", rng_seed)

    print("config done")
    control.registerCallback(trackCases, EventType.Stepped)
    # Run simulation
    control.control()

def main():
    outputPrefix = os.path.join("2-3", str(random.randint(0, 10000000)))
    iterations = 100
    fraction_commuters = [[0.0]*iterations, [0.25]*iterations, [0.5]*iterations, [0.75]*iterations, [1.0]*iterations]
    # Run simulations
    for i in range(len(fraction_commuters)):
        for j in range(len(fraction_commuters[i])):
            fraction = fraction_commuters[i][j]
            rng_seed = random.randint(0, 10000000)
            runSimulation(outputPrefix, fraction, j, rng_seed)

    # Post-processing
    plotNewCases(outputPrefix, iterations)
    plotCumulativeCases(outputPrefix, iterations)
    print("Simulation files stored in {}".format(outputPrefix))


if __name__=="__main__":
    main()
