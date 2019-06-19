from modular_plots import *

plot1 = ["RegionA_SR=0.002_Monday_Days=50_Cnt=1000.csv",       "RegionB_SR=0.002_Monday_Days=50_Cnt=1000.csv",       "plot_compSize_SR=0.002_Days=50",       None]
plot2 = ["RegionA_SR=0.002_Monday_Days=300_Cnt=1000.csv",      "RegionB_SR=0.002_Monday_Days=300_Cnt=1000.csv",      "plot_compSize_SR=0.002_Days=300",      None]
plot3 = ["RegionA_SR=0.00000167_Monday_Days=300_Cnt=1000.csv", "RegionB_SR=0.00000167_Monday_Days=300_Cnt=1000.csv", "plot_compRate_SR=0.00000167_Days=300", None]
plot4 = ["RegionA_SR=0.00000167_Monday_Days=300_Cnt=1000.csv", "RegionB_SR=0.00000167_Monday_Days=300_Cnt=1000.csv", "plot_compSize_SR=0.00000167_Days=300", 92000]
    
source_folder = "./count1000/"
target_folder = "./count1000_plots/"

def do_comparison_plot(csv_pair):
    """Create plot in which two datasets are compared. The parameter 'csv_pair' determines what is compared."""
    addCSVToPlot(source_folder + csv_pair[0], plot_type="CASE_PER_DAY_AVG", pattern="g", label="RegionA")
    addCSVToPlot(source_folder + csv_pair[1], plot_type="CASE_PER_DAY_AVG", pattern="b", label="RegionB")

    plt.xlabel("Day")
    plt.ylabel("New cases")
    plt.legend()

    save_to_file(target_folder + csv_pair[2] + "_CASE_PER_DAY_AVG.svg")

    addCSVToPlot(source_folder + csv_pair[0], plot_type="FINALDAY_HIST", pattern="g", alpha=0.5, label="RegionA", threshold_hist=csv_pair[3])
    addCSVToPlot(source_folder + csv_pair[1], plot_type="FINALDAY_HIST", pattern="b", alpha=0.5, label="RegionB", threshold_hist=csv_pair[3])

    plt.xlabel("Amount of infected")
    plt.ylabel("Frequency")
    plt.legend()
    
    save_to_file(target_folder + csv_pair[2] + "_FINALDAY_HIST.svg")
# ENDFUNCTION

 
def doPlots():
    ################ HISTOGRAMS ################
    # SR=0.00000167
    addCSVToPlot("./Belgie_1000sim_300day_SR=0.00000167.csv", plot_type="FINALDAY_HIST", pattern="g", alpha=0.5, label="Infected")

    plt.xlabel("Amount of infected")
    plt.ylabel("Amount of simulations")

    save_to_file("HIST_SR=0.00000167.svg")

    # SR=0.00000167 filtered at 87k
    addCSVToPlot("./Belgie_1000sim_300day_SR=0.00000167.csv", plot_type="FINALDAY_HIST", pattern="g", alpha=0.5, label="Infected", threshold_hist=87000)

    plt.xlabel("Amount of infected")
    plt.ylabel("Amount of simulations")

    save_to_file("HIST_FILTER87K_SR=0.00000167.svg")

    # SR=0.00000167 filtered at 91k
    addCSVToPlot("./Belgie_1000sim_300day_SR=0.00000167.csv", plot_type="FINALDAY_HIST", pattern="g", alpha=0.5, label="Infected", threshold_hist=91000)

    plt.xlabel("Amount of infected")
    plt.ylabel("Amount of simulations")

    save_to_file("HIST_FILTER91K_SR=0.00000167.svg")

    # SR=0.002
    addCSVToPlot("./Belgie_1000sim_300day_SR=0.002.csv", plot_type="FINALDAY_HIST", pattern="g", alpha=0.5, label="Infected")

    plt.xlabel("Amount of infected")
    plt.ylabel("Amount of simulations")

    save_to_file("HIST_SR=0.002.svg")

    ################ AVG PER DAY ################
    addCSVToPlot("./Belgie_1000sim_300day_SR=0.00000167.csv", plot_type="CASE_PER_DAY_AVG", pattern="g", label="?")

    plt.xlabel("Day")
    plt.ylabel("New cases")
    plt.legend()

    save_to_file("AVG_SR=0.00000167.svg")

    addCSVToPlot("./Belgie_1000sim_300day_SR=0.002.csv", plot_type="CASE_PER_DAY_AVG", pattern="g", label="?")

    plt.xlabel("Day")
    plt.ylabel("New cases")
    plt.legend()

    save_to_file("AVG_SR=0.002.svg")

doPlots()