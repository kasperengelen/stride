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

for plot in [plot1, plot2, plot3, plot4]:
    print("plotting {}".format(plot[2]))
    do_comparison_plot(plot)

 
