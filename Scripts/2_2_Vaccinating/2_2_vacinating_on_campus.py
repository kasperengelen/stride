import csv
import matplotlib.pyplot as plt
import os
import random
import sys

from pystride.Event import Event, EventType
from pystride.PyController import PyController
from pystride.stride.pystride import Id_College

vaccinatedFolder = "Vaccinated"
notVaccinatedFolder = "NotVaccinated"

def vaccinate(simulator, event):
    if event.timestep == 7:
        pop = simulator.GetPopulation()
        for pIndex in range(pop.size()):
            if pop[pIndex].GetPoolId(Id_College):
                if pop[pIndex].GetHealth().IsSusceptible():
                    pop[pIndex].GetHealth().SetImmune()
        
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
    days = range(0, 50)
    for v in [False, True]:
        totalNewCases = [0]*50
        for i in range(0, iterations):
            cumulativeCasesPerDay = []
            
            if v:
                folderName = os.path.join(outputPrefix, vaccinatedFolder + "_" + str(i))
            else:
                folderName = os.path.join(outputPrefix, notVaccinatedFolder + "_" + str(i))
                
            with open(os.path.join(folderName, "cases.csv")) as csvfile:
                reader = csv.DictReader(csvfile, fieldnames=["timestep", "cases"])
                row1 = next(reader)
                
                for row in reader:
                    cumulativeCases = int(row["cases"])
                    cumulativeCasesPerDay.append(cumulativeCases)
                    
            prevCumulativeCases = cumulativeCasesPerDay[0] # Keep track of how many cases have been recorded until current time-step
            for j in range(len(cumulativeCasesPerDay)):
                totalNewCases[j] += cumulativeCasesPerDay[j] - prevCumulativeCases
                prevCumulativeCases = cumulativeCasesPerDay[j]
                
        averageCasesPerDay = [i/iterations for i in totalNewCases]
        plt.plot(days, averageCasesPerDay)
        
    plt.xlabel("Simulation day")
    plt.ylabel("New cases per day")
    plt.legend(["Students are vaccinated", "Students are not vaccinated"])
    plt.show()

def plotCumulativeCases(outputPrefix, iterations):
    """
        Plot cumulative cases per day.
    """
    days = range(0, 50)
    for v in [False, True]:
        totalNewCases = [0]*50
        for i in range(0, iterations):
            cumulativeCasesPerDay = []
            
            if v:
                folderName = os.path.join(outputPrefix, vaccinatedFolder + "_" + str(i))
            else:
                folderName = os.path.join(outputPrefix, notVaccinatedFolder + "_" + str(i))
                
            with open(os.path.join(folderName, "cases.csv")) as csvfile:
                reader = csv.DictReader(csvfile, fieldnames=["timestep", "cases"])
                row1 = next(reader)
                
                for row in reader:
                    cumulativeCases = int(row["cases"])
                    cumulativeCasesPerDay.append(cumulativeCases)
                    
            for j in range(len(cumulativeCasesPerDay)):
                totalNewCases[j] += cumulativeCasesPerDay[j]
                
        averageCasesPerDay = [i/iterations for i in totalNewCases]
        plt.plot(days, averageCasesPerDay)
        
    plt.xlabel("Simulation day")
    plt.ylabel("Cumulative cases per day")
    plt.legend(["Students are vaccinated", "Students are not vaccinated"])
    plt.show()

def runSimulation(outputPrefix, v, iteration, rng_seed):
    # Set up simulator
    control = PyController(data_dir="data")
    # Load configuration from file
    control.loadRunConfig(os.path.join("config", "run_import_default.xml"))
    # Set some parameters
    
    if v:
        outputPrefix = os.path.join(outputPrefix, vaccinatedFolder + "_" + str(iteration))
    else:
        outputPrefix = os.path.join(outputPrefix, notVaccinatedFolder + "_" + str(iteration))
        
    control.runConfig.setParameter("immunity_profile", "AgeDependent")
    control.runConfig.setParameter("immunity_distribution_file", "data/lower_student_immunity.xml")
    control.runConfig.setParameter("immunity_link_probability", 0)   
    control.runConfig.setParameter("population_file", "data/2_2_gengeopop.proto")
    control.runConfig.setParameter("geopopulation_file", "data/2_2_gengeopop.proto")
    control.runConfig.setParameter("output_prefix", outputPrefix)
    control.runConfig.setParameter("rng_seed", rng_seed)

    print("config done")
    if v:
        control.registerCallback(vaccinate, EventType.Stepped)
    control.registerCallback(trackCases, EventType.Stepped)
    # Run simulation
    control.control()

def main():
    outputPrefix = "2-2"
    iterations = 20
    MassVaccinate = [[False]*iterations, [True]*iterations]
    # Run simulations
    for i in range(len(MassVaccinate)):                    
        for j in range(len(MassVaccinate[i])):
            v = MassVaccinate[i][j]
            rng_seed = random.randint(0, 10000000)
            runSimulation(outputPrefix, v, j, rng_seed)
            
    # Post-processing
    plotNewCases(outputPrefix, iterations)
    plotCumulativeCases(outputPrefix, iterations)

if __name__=="__main__":
    main()
