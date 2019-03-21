import matplotlib.pyplot as plt
import pandas as pd
import math

# format: [REGION,SR, COUNT, DAYS]
param_lists = [
    ["A", "0.002",      100, 50 ],
    ["A", "0.0003",     100, 50 ],
    ["A", "0.00000167", 100, 50 ],
    ["A", "0.002",      100, 500],
    ["A", "0.0003",     100, 500],
    ["A", "0.00000167", 100, 500],
    ["A", "0.00000167", 500, 500],
    ["B", "0.002",      100, 50 ],
    ["B", "0.0003",     100, 50 ],
    ["B", "0.00000167", 100, 50 ],
    ["B", "0.002",      100, 500],
    ["B", "0.0003",     100, 500],
    ["B", "0.00000167", 100, 500],
    ["B", "0.00000167", 500, 500]
]

def getCSVFileName(param_list):
    """Retrieve the filename of the CSV file that was generated for the specified parameters."""
    return "./data/" + "region{}_stan_r0=11_VC=0.8_SR={}_cnt={}_days={}.csv".format(*param_list)
#ENDFUNCTION

def getSVGFileName(param_list, plot_name):
    """Retrieve the filename of the SVG file that will contain the plot of the CSV file that was generated for the specified parameters."""
    return "./figs/" + "region{}_stan_r0=11_VC=0.8_SR={}_cnt={}_days={}_{}.svg".format(*(param_list + [plot_name]))
#ENDFUNCTION

def plot_all(param_lists, to_file=False):
    for param_list in param_lists:
        print("Plotting for parameters (region, seed rate, count, days): " + str(param_list))
        plot_csv(param_list, to_file=to_file)
#ENDFUNCTION

def plot_csv(param_list, to_file=False):
    # Import data from csv file
    fname = getCSVFileName(param_list)
    df = pd.read_csv(fname, sep=",", encoding="utf-8")

    # Plot cumulative cases
    for column in df:
        x = range(len(df[column]))
        y = df[column]
        plt.plot(x, y, '-')

    plt.title("Cumulative cases")
    plt.xlabel("Day")
    plt.ylabel("Cumulative cases")
    #plt.legend(list(df.columns))
    if to_file:
        plt.gcf().savefig(getSVGFileName(param_list, "CUMULATIVE"))
    else:
        plt.show()
    plt.clf()

    # Plot cases per day
    new_cases = list()
    for column in df:
            x = range(len(df[column]))
            y = [df[column][i] - df[column][i-1] for i in range(1, len(df[column]))]
            y.insert(df[column][0], 0)
            new_cases.append(y)
            plt.plot(x, y, '-')

    plt.title("Cases per day")
    plt.xlabel("Day")
    plt.ylabel("New cases")
    #plt.legend(list(df.columns))
    if to_file:
        plt.gcf().savefig(getSVGFileName(param_list, "CASE_PER_DAY"))
    else:
        plt.show()
    plt.clf()

    # Calculate average new cases per day
    n_timesteps = len(new_cases[0])
    avg_df = list()
    for timestep in range(n_timesteps):
            avg = 0
            for column in new_cases:
                    avg += column[timestep]
            avg /= len(new_cases)
            avg_df.append(avg)

    plt.title("Cases per day (averaged)")
    plt.xlabel("Day")
    plt.ylabel("New cases")
    plt.plot(range(len(avg_df)), avg_df, '-')
    if to_file:
        plt.gcf().savefig(getSVGFileName(param_list, "CASE_PER_DAY_AVG"))
    else:
        plt.show()
    plt.clf()

    # Plot histogram of final day frequencies
    values = list()
    for column in df:
            infected = int(df[column][len(df[column])-1])
            values.append(infected)

    plt.title("Final frequency histogram")
    plt.hist(values, bins=25)
    plt.xlabel("Total cases")
    plt.ylabel("Frequency")
    if to_file:
        plt.gcf().savefig(getSVGFileName(param_list, "FINALDAY_HIST"))
    else:
        plt.show()
    plt.clf()
#ENDFUNCTION

if __name__ == "__main__":
    plot_all(param_lists, True)
