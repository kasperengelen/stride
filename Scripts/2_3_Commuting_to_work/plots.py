import matplotlib.pyplot as plt
import pandas as pd
import scipy.stats
import math
import sys
import os
import csv


def plotCumulativeCases(iterations):
	"""
        Plot cumulative cases per day.
    """
	fig = plt.figure()
	days = range(0, 51)
	for k in range(5):
		totalNewCases = [0] * 51
		for i in range(0, iterations):
			cumulativeCasesPerDay = []


			with open(sys.argv[k+1]) as csvfile:
				reader = csv.DictReader(csvfile, fieldnames=["timestep", "cases"])
				row1 = next(reader)

				for row in reader:
					cumulativeCases = int(row["cases"])
					cumulativeCasesPerDay.append(cumulativeCases)
				csvfile.close()

			for j in range(len(cumulativeCasesPerDay)):
				totalNewCases[j] += cumulativeCasesPerDay[j]

		averageCasesPerDay = [i / iterations for i in totalNewCases]
		plt.plot(days, averageCasesPerDay)

	plt.xlabel("Simulation day")
	plt.ylabel("Cumulative cases per day")
	plt.legend(["0.0", "0.3", "0.5", "0.7", "1.0"])
	fig.savefig("population500000_Cumulative.svg", format="svg")
	plt.show()

def plotNewCases():
	fig = plt.figure()
	for k in range (5):
		df = pd.read_csv(sys.argv[k+1], sep=",", encoding="utf-8")
		new_cases = list()
		for column in df:
			x = range(len(df[column]))
			y = [df[column][i] - df[column][i - 1] for i in range(1, len(df[column]))]
			y.insert(df[column][0], 0)
			new_cases.append(y)
			# plt.plot(x, y, '-')
		# plt.show()

		# Calculate average new cases per day
		n_timesteps = len(new_cases[0])
		avg_df = list()
		for timestep in range(n_timesteps):
			avg = 0
			for column in new_cases:
				avg += column[timestep]
			avg /= len(new_cases)
			avg_df.append(avg)

		plt.plot(range(len(avg_df)), avg_df, '-')
	plt.xlabel("Day")
	plt.ylabel("New cases")
	plt.legend(["0.0", "0.3", "0.5", "0.7", "1.0"])
	fig.savefig("population500000_cases_per_day.svg", format="svg")
	plt.show()


plotCumulativeCases(10)
plt.close()

plotNewCases()


