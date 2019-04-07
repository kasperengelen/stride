import os
import matplotlib.pyplot as plt
import matplotlib.dates as md
import datetime

def time_to_datetime(times, format):
    dates = list()
    for i in times:
        dates.append(datetime.datetime.strptime(i, format))
        
    return dates
    
# Day plot
times = ["04:540",
        "09:489",
        "14:142",
        "18:007",
        "38:496"]
dates = time_to_datetime(times, "%S:%f")
days = [50, 100, 150, 200, 500]

fig = plt.figure()

ax=plt.gca()
xfmt = md.DateFormatter('%S')
ax.yaxis.set_major_formatter(xfmt)

plt.xlabel("Days in simulation")
plt.ylabel("Time in seconds")
plt.ylim([datetime.datetime.strptime("0", "%S"), datetime.datetime.strptime("45", "%S")])

plt.plot(days, dates, marker='o')
fig.savefig(os.path.join("plots", "3_GProf_numdays.eps"))

# Population size
times = ["00:421",
         "00:816",
         "01:332",
         "02:249",
         "04:926"]
dates = time_to_datetime(times, "%S:%f")
pops = [10000, 50000, 100000, 200000, 500000]

fig = plt.figure()

ax=plt.gca()
xfmt = md.DateFormatter('%S')
ax.yaxis.set_major_formatter(xfmt)

plt.xlabel("Population size")
plt.ylabel("Time in seconds")
plt.ylim([datetime.datetime.strptime("0", "%S"), datetime.datetime.strptime("7", "%S")])

plt.plot(pops, dates, marker='o')
fig.savefig(os.path.join("plots", "3_GProf_popsize.eps"))

# Immunity rate
times = ["05:817",
         "05:750",
         "05:643",
         "05:580",
         "05:648"]
dates = time_to_datetime(times, "%S:%f")
imm = [0.2, 0.4, 0.6, 0.8, 1.0]

fig = plt.figure()

ax=plt.gca()
xfmt = md.DateFormatter('%S')
ax.yaxis.set_major_formatter(xfmt)

plt.xlabel("Immunity rate")
plt.ylabel("Time in seconds")
plt.ylim([datetime.datetime.strptime("3", "%S"), datetime.datetime.strptime("7", "%S")])

plt.plot(imm, dates, marker='o')
fig.savefig(os.path.join("plots", "3_GProf_immunityrate.eps"))

# Seeding rate
times = ["06:599",
         "05:589",
         "05:134",
         "04:865"]
dates = time_to_datetime(times, "%S:%f")
seed = [0.2, 0.02, 0.002, 0.0002]

fig = plt.figure()

ax=plt.gca()
xfmt = md.DateFormatter('%S')
ax.yaxis.set_major_formatter(xfmt)

plt.xlabel("Seeding rate")
plt.ylabel("Time in seconds")
plt.ylim([datetime.datetime.strptime("3", "%S"), datetime.datetime.strptime("7", "%S")])

plt.plot(seed, dates, marker='o')
fig.savefig(os.path.join("plots", "3_GProf_seedingrate.eps"))

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
x = [1,2,3,4]
ax=plt.subplot()

plt.xlabel("Contact log modes")
plt.ylabel("Time in minutes")

ax.bar(x, [27.48, 26.48, 0.6, 0.5], tick_label=labels)

fig.savefig(os.path.join("plots", "3_GProf_contactlogmode.eps"))

