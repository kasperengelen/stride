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

def plotNewCases(outputPrefix, iterations, fractions, workplace_files, day, legend):
    """
        Plot new cases per day.
    """
    fig = plt.figure()
    days = range(0, day)
    for fraction in fractions:
        for workplace in workplace_files:
            totalNewCases = [0]*day
            for i in range(0, iterations):
                cumulativeCasesPerDay = []

                folderName = os.path.join(outputPrefix, workplace + str(fraction) + "_" + str(i))
                    
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
    plt.legend(legend, title="Fraction commuters", loc="best")
    #plt.show()
    fig.savefig(os.path.join(outputPrefix, 'cases_per_day_{}runs.eps'.format(iterations)))


def plotCumulativeCases(outputPrefix, iterations, fractions, workplace_files, day, legend):
    """
        Plot cumulative cases per day.
    """
    fig = plt.figure()
    days = range(0, day)
    for fraction in fractions:
        for workplace in workplace_files:
            totalNewCases = [0]*day
            for i in range(0, iterations):
                cumulativeCasesPerDay = []

                folderName = os.path.join(outputPrefix, workplace + str(fraction) + "_" + str(i))
                    
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
    plt.legend(legend, title="Fraction commuters", loc="best")
    #plt.show()
    fig.savefig(os.path.join(outputPrefix, 'cumulative_cases_{}runs.eps'.format(iterations)))

def runSimulation(outputPrefix, fraction, workplace, iteration, rng_seed, days):
    # Set up simulator
    control = PyController(data_dir="data")
    # Load configuration from file
    control.loadRunConfig(os.path.join("config", "workplace.xml"))
    # Set some parameters

    outputPrefix = os.path.join(outputPrefix, workplace + str(fraction) + "_" + str(iteration))

    control.runConfig.setParameter("geopop_gen.fraction_workplace_commuters", fraction)
    control.runConfig.setParameter("disease_config_file", "disease_influenza.xml")
    control.runConfig.setParameter("output_prefix", outputPrefix)
    control.runConfig.setParameter("rng_seed", rng_seed)

    control.runConfig.setParameter("num_days", str(days))
    control.runConfig.setParameter("geopop_gen.population_size", "600000")
    control.runConfig.setParameter("geopop_gen.workplace_distribution_file", "data/" + workplace)

    control.registerCallback(trackCases, EventType.Stepped)
    # Run simulation
    control.control()

def main():
    iterations = 20
    days = 50
    outputPrefix = os.path.join("workplace", f"commuting_runs{iterations}_r02_days{days}_imm0.4")
    fractions = [0.0, 0.5, 1.0]
    workplace_files = ["workplace_size_distribution2.csv", "workplace_size_distribution.csv"]

    # Run simulations
    for fraction in fractions:
        for workplace in workplace_files:
            for iteration in range(iterations):
                rng_seed = random.randint(0, 10000000)
                #runSimulation(outputPrefix, fraction, workplace, iteration, rng_seed, days)

    legend = list()
    for fraction in fractions:
        for alg in ["OG", "New"]:
            legend.append(alg + ": " + str(fraction))
    # Post-processing
    plotNewCases(outputPrefix, iterations, fractions, workplace_files, days, legend)
    plotCumulativeCases(outputPrefix, iterations, fractions, workplace_files, days, legend)
    print("Simulation files stored in {}".format(outputPrefix))


if __name__=="__main__":
    random.seed(datetime.datetime.now())
    main()
    
