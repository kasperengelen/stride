import os
import matplotlib.pyplot as plt
import matplotlib.dates as md
from datetime import datetime
import time


def time_to_datetime(times, format, n=20):
    dates = list()

    for i in times:
        t = datetime.strptime(i, format)
        td = t - datetime(1900, 1, 1)
        sec = td.total_seconds()
        div_time = sec / n
        fixed_time = datetime.fromtimestamp(div_time)

        f = fixed_time.strftime(format)
        g = datetime.strptime(f, format)
        dates.append(g)

    return dates


# Day plot
fig = plt.figure()
times = ["00:59:322",
         "01:52:714",
         "02:34:824",
         "03:09:711",
         "06:51:445"]
dates = time_to_datetime(times, "%M:%S:%f")
days = [50, 100, 150, 200, 500]

ax = plt.gca()
xfmt = md.DateFormatter('%-S')
ax.yaxis.set_major_formatter(xfmt)

plt.xlabel("Days in simulation")
plt.ylabel("Wall clock time in seconds")
plt.ylim([datetime.strptime("0", "%S"), datetime.strptime("30", "%S")])

plt.plot(days, dates, marker='o')
fig.savefig(os.path.join("plots", "3_numdays.eps"))

# Population size
fig = plt.figure()
times = ["04:093",
         "04:533",
         "05:615",
         "08:298",
         "16:479",
         "34:616"]
dates = time_to_datetime(times, "%S:%f")
pops = [10000, 50000, 100000, 200000, 500000, 1000000]

ax = plt.gca()
xfmt = md.DateFormatter('%-S')
ax.yaxis.set_major_formatter(xfmt)

plt.xlabel("Population size")
plt.ylabel("Wall clock time in seconds")
plt.ylim([datetime.strptime("0", "%S"), datetime.strptime("7", "%S")])
plt.plot(pops, dates, marker='o')

# Population size
times = ["00:06:923",
         "00:10:203",
         "00:15:210",
         "00:25:015",
         "00:52:985",
         "01:39:400"]
dates = time_to_datetime(times, "%M:%S:%f")
pops = [10000, 50000, 100000, 200000, 500000, 1000000]
plt.plot(pops, dates, marker='o')
plt.legend(["Generating a population", "Generating and simulating"])
fig.savefig(os.path.join("plots", "3_popsize.eps"))

# Immunity rate
fig = plt.figure()
times = ["00:21:220",
         "00:20:534",
         "00:20:362",
         "00:20:746",
         "00:20:816",
         "00:20:758"]
dates = time_to_datetime(times, "%M:%S:%f")
imm = [0, 0.2, 0.4, 0.6, 0.8, 1.0]
plt.plot(imm, dates, marker='o')

times = ["05:30:319",
         "03:25:408",
         "02:42:241",
         "01:52:134",
         "01:01:007",
         "00:38:640"]
dates = time_to_datetime(times, "%M:%S:%f")
imm = [0, 0.2, 0.4, 0.6, 0.8, 1.0]

ax = plt.gca()
xfmt = md.DateFormatter('%-S')
ax.yaxis.set_major_formatter(xfmt)

plt.xlabel("Immunity rate")
plt.ylabel("Wall clock time in seconds")
plt.ylim([datetime.strptime("0", "%S"), datetime.strptime("17", "%S")])

plt.plot(imm, dates, marker='o')
plt.legend(["Generating a population", "Generating and simulating"])
fig.savefig(os.path.join("plots", "3_immunityrate.eps"))

# Seeding rate
fig = plt.figure()
times = ["00:20:759",
         "00:21:389",
         "00:20:681",
         "00:20:268",
         "00:20:704",
         "00:20:984",
         "00:20:806"]
dates = time_to_datetime(times, "%M:%S:%f")
seed = [0.1973234, 0.1, 0.06, 0.02, 0.006, 0.002, 0.0002]

plt.plot(seed, dates, marker='o')

times = ["01:13:849",
         "01:16:650",
         "01:16:384",
         "01:16:063",
         "01:10:787",
         "01:00:997",
         "00:52:866"]
dates = time_to_datetime(times, "%M:%S:%f")
seed = [0.1973234, 0.1, 0.06, 0.02, 0.006, 0.002, 0.0002]

ax = plt.gca()
xfmt = md.DateFormatter('%-S')
ax.yaxis.set_major_formatter(xfmt)

plt.xlabel("Seeding rate")
plt.ylabel("Wall clock time in seconds")
plt.ylim([datetime.strptime("0", "%S"), datetime.strptime("7", "%S")])

plt.plot(seed, dates, marker='o')
plt.legend(["Generating a population", "Generating and simulating"])
fig.savefig(os.path.join("plots", "3_seedingrate.eps"))

# Contact log mode
"""times = ["00:27:06",
         "00:26:48",
         "00:00:06",
         "00:00:05"]

dates = time_to_datetime(times, "%H:%M:%S")"""
labels = ["Susceptibles",
          "All",
          "Transmission",
          "None"]
fig = plt.figure()
x = [1, 2, 3, 4]
ax = plt.subplot()

plt.xlabel("Contact log modes")
plt.ylabel("Wall clock time in minutes")

ax.bar(x, [27.48, 26.48, 0.6, 0.5], tick_label=labels)

fig.savefig(os.path.join("plots", "3_contactlogmode.eps"))
