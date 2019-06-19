from modular_plots import *


def doPlots():
    ################ HISTOGRAMS ################
    # SR=0.00000167
    addCSVToPlot("./Vlaanderen_1000sim_300day_SR=0.00000167.csv", plot_type="FINALDAY_HIST", pattern="g", alpha=0.5, label="Infected")

    plt.xlabel("Amount of infected")
    plt.ylabel("Amount of simulations")

    save_to_file("HIST_SR=0.00000167.eps")

    # SR=0.00000167 filtered at 87k
    addCSVToPlot("./Vlaanderen_1000sim_300day_SR=0.00000167.csv", plot_type="FINALDAY_HIST", pattern="g", alpha=0.5, label="Infected", threshold_hist=87000)

    plt.xlabel("Amount of infected")
    plt.ylabel("Amount of simulations")

    save_to_file("HIST_FILTER87K_SR=0.00000167.eps")

    # SR=0.00000167 filtered at 91k
    addCSVToPlot("./Vlaanderen_1000sim_300day_SR=0.00000167.csv", plot_type="FINALDAY_HIST", pattern="g", alpha=0.5, label="Infected", threshold_hist=91000)

    plt.xlabel("Amount of infected")
    plt.ylabel("Amount of simulations")

    save_to_file("HIST_FILTER91K_SR=0.00000167.eps")

    # SR=0.002
    addCSVToPlot("./Vlaanderen_1000sim_300day_SR=0.002.csv", plot_type="FINALDAY_HIST", pattern="g", alpha=0.5, label="Infected")

    plt.xlabel("Amount of infected")
    plt.ylabel("Amount of simulations")

    save_to_file("HIST_SR=0.002.eps")

    ################ AVG PER DAY ################
    addCSVToPlot("./Vlaanderen_1000sim_300day_SR=0.00000167.csv", plot_type="CASE_PER_DAY_AVG", pattern="g", label="?")

    plt.xlabel("Day")
    plt.ylabel("New cases")

    save_to_file("AVG_SR=0.00000167.eps")

    addCSVToPlot("./Vlaanderen_1000sim_300day_SR=0.002.csv", plot_type="CASE_PER_DAY_AVG", pattern="g", label="?")

    plt.xlabel("Day")
    plt.ylabel("New cases")

    save_to_file("AVG_SR=0.002.eps")

doPlots()
