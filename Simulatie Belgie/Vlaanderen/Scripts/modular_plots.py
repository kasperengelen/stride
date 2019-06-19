import matplotlib.pyplot as plt
import pandas as pd

def addCSVToPlot(csv_file_path, plot_type, pattern, label, alpha=1.0, threshold_hist=None):
    """
    csv_file_path: path to the CSV file that contains the data that needs to be plotted.
    plot_type: "CASE_PER_DAY", "CASE_PER_DAY_AVG", "FINALDAY_HIST"
    pattern: a matplotlib-compatible pattern speciciation (is inputted directly into matplotlib methods, unless it is equal to None). Note: this pattern must be compatible with the selected plot type.
    label: A string containing the label of the plot.
    threshold_hist: if not none, this will be used to determine what simulations are included in the histogram.
    """
    df = pd.read_csv(csv_file_path)

    if pattern is None:
        pattern = '-' # TODO set to default style and color

    if plot_type == "CASE_PER_DAY" or plot_type == "CASE_PER_DAY_AVG":
        # calculate the list of new cases (both for per-day and average)

        # list, each entry is a list that represents one day. Each entry of that list is the new cases for that day, one entry per CSV
        new_cases = [] 
        for column in df: # iterate over simulations. "col" contains the seed.
            x = range(len(df[column])) # create range of day 0 to final day

            # list of differences between days
            y = [df[column][i] - df[column][i-1] for i in range(1, len(df[column]))]
            y.insert(df[column][0], 0)
            
            new_cases.append(y)

            # plot in case of per-day
            if plot_type == "CASE_PER_DAY":
                plt.plot(x, y, pattern, label=label, alpha=alpha)
        # ENDFOR

        # plot in case of average
        if plot_type == "CASE_PER_DAY_AVG":
            n_timesteps = len(new_cases[0]) # nr of simulations
            avg_df = [] # list of new cases per day, each entry is the average over all simulations
            for timestep in range(n_timesteps):
                avg = 0
                for column in new_cases: # iterate over different simulations
                    avg += column[timestep]
                avg /= len(new_cases) # averaged
                avg_df.append(avg)
        # ENDCASE

        plt.plot(range(len(avg_df)), avg_df, pattern, label=label, alpha=alpha)
    elif plot_type == "FINALDAY_HIST":
        values = []
        for column in df:
            infected = int(df[column][len(df[column])-1])

            # if threshold is specified, and the number of infected does not meet the threshold, don't plot it.
            if threshold_hist is not None and infected <= threshold_hist:
                pass
            else:
                values.append(infected)
        plt.hist(values, bins=25, color=pattern, label=label, alpha=alpha)
        pass
    else:
        raise Exception("Unknown plot type '{}'".format(plot_type))
# ENDFUNCTION

def save_to_file(filename):
    plt.gcf().savefig(filename)
    plt.clf()
# ENDFUNCTION

def show_plot():
    plt.show()
    plt.clf()
# ENDFUNCTION





