import csv
import matplotlib.pyplot as plt
import os
import random
import sys

from pystride.Event import Event, EventType
from pystride.PyController import PyController
from pystride.stride.pystride import ToId, ToString, NumOfTypes

def vaccinate(simulator, event):
    if event.timestep == 7:
        print("BANZAI")
        pop = simulator.GetPopulation()
       
        tot_count = 0
        tot_student_count = 0        

        appel = ToString(2)
        print(appel)

        for pIndex in range(pop.size()):
            # No idea if this works
            person = pop[pIndex]          
            poolId = 2

            if person.GetAge() >= 18 and person.GetAge() <= 26:
                tot_count += 1
            if person.IsInPool(poolId):
                tot_student_count += 1
                # This pop is a student
                if pop[pIndex].GetHealth().IsSusceptible():
                    pop[pIndex].GetHealth().SetImmune()
        print(tot_count, tot_student_count, tot_student_count/tot_count * 100)
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

def plotNewCases(outputPrefix, vaccinationLevels):
    """
        Plot new cases per day for a list of vaccination levels.
    """
    for v in vaccinationLevels:
        days = []
        newCasesPerDay = []
        prevCumulativeCases = 0 # Keep track of how many cases have been recorded until current time-step
        with open(os.path.join(outputPrefix + "_" + str(v), "cases.csv")) as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                days.append(int(row["timestep"]))
                cumulativeCases = int(row["cases"])
                newCasesPerDay.append(cumulativeCases - prevCumulativeCases)
                prevCumulativeCases = cumulativeCases
        plt.plot(days, newCasesPerDay)
    plt.xlabel("Simulation day")
    plt.ylabel("New cases per day")
    plt.legend(["60% immune", "80% immune"])
    plt.show()

def runSimulation(outputPrefix, vaccinate_callback):
    # Set up simulator
    control = PyController(data_dir="data")
    # Load configuration from file
    control.loadRunConfig(os.path.join("config", "run_import_default.xml"))
    # Set some parameters
    
    
    control.runConfig.setParameter("immunity_profile", "AgeDependent")
    control.runConfig.setParameter("immunity_distribution_file", "data/lower_student_immunity.xml")
    control.runConfig.setParameter("immunity_link_probability", 0)   

    # Set population file    
    control.runConfig.setParameter("population_file", "2_2_gengeopop.proto")
    control.runConfig.setParameter("population_type", "import")
    
    control.runConfig.setParameter("output_prefix", outputPrefix)

    print("config done")
    if vaccinate_callback:
        control.registerCallback(vaccinate, EventType.Stepped)
    #control.registerCallback(trackCases, EventType.Stepped)
    # Run simulation
    control.control()

def main():
    outputPrefix = "2-2"
    MassVaccinate = [True]
    # Run simulations
    for v in MassVaccinate:
        runSimulation(outputPrefix, v)
    # Post-processing
    #plotNewCases(outputPrefix, vaccinationLevels)

if __name__=="__main__":
    main()
