import pandas as pd
import numpy as np
from math import sqrt
from scipy.stats import t, norm

def calc_t_value(data_count, confidence):
    """
        Determine the t-value for the specified confidence (must be in [0,100]), and the specified data count.
    """
    alpha = 1 - (confidence / 100)

    # call t(1-alpha/2, n-1)
    return t.ppf(1 - (alpha/2), data_count - 1)
# ENDFUNCTION

def read_data(csv_filename):
    """
        Read the specified CSV file. The last row of the CSV file will be returned in the form of a python list.
    """
    df = pd.read_csv(csv_filename)

    values = []
    for column in df:
        infected = int(df[column][len(df[column])-1])
        values.append(infected)

    return values
# ENDFUNCTION

def calc_mean(data):
    """
        Estimator for the mean of the specified data.
    """
    return np.mean(data)
# ENDFUNCTION

def calc_stdev(data):
    """
        Estimator for the standard deviation of the specified data.
    """

    mean = calc_mean(data)
    squared_offsets = map(lambda x_i : (x_i - mean)**2, data)
    sum_squared_offsets = sum(squared_offsets)

    return sqrt(sum_squared_offsets / (len(data) - 1))
# ENDFUNCTION

def calc_mean_interval(data, confidence):
    """
        Determine a confidence interval for the mean.

        'data' is a list of values.
        'confidence' parameter is in [0,100]
    """
    n = len(data)

    # estimate parameters
    mean_est = calc_mean(data)
    stdev_est = calc_stdev(data)

    # calculate t-value
    t_value = calc_t_value(n, confidence)

    # determine offset
    offset = t_value * stdev_est / sqrt(n)
    interval = [mean_est - offset, mean_est + offset]

    return interval
# ENDFUNCTION

def calc_phi_value(confidence):
    """
        Normal distribution calculation. This calculates Phi for the specified confidence interval so that
        alph = 1 - (conf / 100)
        Phi(1 - (alph/2))

        'confidence' parameter is in [0,100]
    """
    alpha = 1 - (confidence / 100)

    # call phi(1-alpha/2))
    return norm.ppf(1 - (alpha/2))
# ENDFUNCTION

def calc_success_chance(data, success_func):
    """
        Estimator for chance of success.

        data: Elements that contain the result of an experiment
        success_func: a function that accepts one argument, and determines whether the argument represents a success or a failure.
    """

    # count outbreaks
    outbreaks = [elem for elem in data if success_func(elem)]

    return len(outbreaks) / len(data)
# ENDFUNCTION

def calc_pct_interval(data, success_func, confidence):
    """
        Calculate confidence interval for the percentage of successes.

        data: Elements that contain the result of an experiment
        success_func: a function that accepts one argument, and determines whether the argument represents a success or a failure.
        confidence: value between 0 and 100 that denotes the confidence of the interval. e.g. confidence=95
    """

    mean_est = calc_success_chance(data, success_func)
    
    stdev_est = sqrt((mean_est * (1 - mean_est))/len(data))

    offset = calc_phi_value(confidence) * stdev_est

    interval = [mean_est - offset, mean_est + offset]

    return interval
# ENDFUNCTION

csv_list = [
    ["./Vlaanderen_1000sim_300day_SR=0.00000167.csv",91000],
    ["./Vlaanderen_1000sim_300day_SR=0.002.csv",     5000 ]
]

for csv in csv_list:
    print("Processing '" + csv[0] + "'.")

    # for determining the percentage of the successes we use the complete dataset
    data = read_data(csv[0])
    print("Interval pct: '" + str(calc_pct_interval(data, lambda x : x >  csv[1], 95)) + "' (95%).")

    # for determining the average size of the outbreaks, we filter the extinctions
    data_outbreak_only = list(filter(lambda x : x >  csv[1], data))

    print("Interval µ: '" + str(calc_mean_interval(data_outbreak_only, 95)) + "' (95%).")

    print("-------------------")
# ENDFOR
