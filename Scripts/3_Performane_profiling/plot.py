import matplotlib.pyplot as plt
import datetime

def time_to_datetime(times):
    dates = list()
    for i in times:
        dates.append(datetime.datetime.strptime(i, "%H:%M:%S:%f"))
        
    return dates
    
# Day plot
times = ["00:00:04:540",
        "00:00:09:489",
        "00:00:14:142",
        "00:00:18:007",
        "00:00:38:496"]
dates = time_to_datetime(times)
days = [50, 100, 150, 200, 500]

plt.plot(days, dates)
plt.xlabel("Days in simulation")
plt.ylabel("Time")
plt.show()

# Population size
times = ["00:00:00:421",
         "00:00:00:816",
         "00:00:01:332",
         "00:00:02:249",
         "00:00:04:926"]
dates = time_to_datetime(times)
pops = [10000, 50000, 100000, 200000, 500000]

plt.plot(pops, dates)
plt.xlabel("Population size")
plt.ylabel("Time")
plt.ylim([datetime.datetime.strptime("0", "%S"), datetime.datetime.strptime("7", "%S")])
plt.show()

# Immunity rate
times = ["00:00:05:817",
    "00:00:05:750",
    "00:00:05:643",
    "00:00:05:580",
    "00:00:05:648"]
dates = time_to_datetime(times)
imm = [0.2, 0.4, 0.6, 0.8, 1.0]

plt.plot(imm, dates)
plt.xlabel("Immunity rate")
plt.ylabel("Time")
plt.ylim([datetime.datetime.strptime("3", "%S"), datetime.datetime.strptime("10", "%S")])
plt.show()

# Seeding rate
times = ["00:00:06:599",
"00:00:05:589",
"00:00:05:134",
"00:00:04:865"]
dates = time_to_datetime(times)
seed = [0.2, 0.002, 0.0002, 0.0002]

plt.plot(seed, dates)
plt.xlabel("Seeding rate")
plt.ylabel("Time")
plt.ylim([datetime.datetime.strptime("0", "%S"), datetime.datetime.strptime("7", "%S")])
plt.show()
