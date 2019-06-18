import matplotlib.pyplot as plt
import csv
import json
import random
import datetime
import copy


def read_household_json():
    with open('households_flanders.json') as json_data:
        d = json.load(json_data)

        return d["householdsList"]


def read_profile_json():
    with open('demographic_profile.json') as json_data:
        d = json.load(json_data)
        return d


def sample_set(HHList, target_ratios):
    # Brackets
    b = len(target_ratios)
    eps = 0.005

    HHList = HHList[:]
    # HHList.extend(HHList)

    total = 0
    bracks = [0, 0]
    for HH in HHList:
        for person in HH:
            age = int(person)
            if age <= 18:
                bracks[0] += 1
            else:
                bracks[1] += 1
            total += 1
    done = False

    HHList2 = HHList[:]
    total2 = total
    bracks2 = bracks[:]

    wc = 0
    while not done:

        # Pick a random row

        # Check if removing is beneficial for ratio's
        # Remove if would decrease ratio of at least 2 bins we want to decrease

        def check_remove(target_ratios, total2, bracks2, row):
            gain_br = [0, 0]
            for person in row:
                age = int(person)
                if age <= 18:
                    gain_br[0] += 1
                else:
                    gain_br[1] += 1

            new_ratios = [(bracks2[0] - gain_br[0]) / (total2 - gain_br[0]),
                          (bracks2[0] - gain_br[0]) / (total2 - gain_br[0])
                          ]
            if abs(target_ratios[0] - new_ratios[0]) < abs(target_ratios[0] - bracks2[0] / total2):
                return True
            else:
                return False

        found = False
        row_i = random.randint(0, len(HHList2) - 1)

        for _ in range(100):
            if row_i >= len(HHList2):
                break

            row = HHList2[row_i]

            remove = check_remove(target_ratios, total2, bracks2, row)

            if remove:
                for person in row:
                    age = int(person)
                    if age <= 18:
                        bracks2[0] -= 1
                    else:
                        bracks2[1] -= 1
                    total2 -= 1
                del HHList2[row_i]
                wc = 0
                break

            row_i += 1

        # if wc > 10000:
        #    break

        # if len(HHList2) < 8050:
        #    print("Y")
        #    break

        wc += 1

        # Check if ratio's close enough
        if abs(target_ratios[0] - bracks2[0] / float(total2)) < eps:
            if abs(target_ratios[1] - bracks2[1] / float(total2)) < eps:
                done = True
                break

    # print(total2)
    # print(len(HHList2))
    #print(target_ratios)
    #print("[" + str(bracks2[0] / total2) + ", " + str(bracks2[1] / total2) + "]")

    return HHList2


def write_household_CSV(HH, name):
    with open(f'Households/household_{name}.csv', 'w+') as csvfile:
        ages = 12

        csvwriter = csv.writer(csvfile, delimiter=',')

        header = list()
        for i in range(ages):
            header.append(f"hh_age{i}")
        csvwriter.writerow(header)

        for household in HH:
            household.extend(["NA"] * (ages - len(household)))
            csvwriter.writerow(household)


def generate_households():
    profile = read_profile_json()
    household = read_household_json()
    provinces = profile["provinces"]

    for province_id in profile["provinces"]:
        province = provinces[province_id]
        centrumsteden = province["centrumsteden"]

        ratios_centrumsteden = province["ratios_centrumsteden"]
        for centrumstad in centrumsteden:
            ratio_stad = ratios_centrumsteden[centrumstad]
            # Quick Fix very bad, fix pl0x
            ratios_stad = [ratio_stad[0], ratio_stad[1] + ratio_stad[2]]
            new_household = sample_set(copy.deepcopy(household), ratios_stad)

            write_household_CSV(new_household, f"city_{centrumstad}")

        ratios_default = province["ratios_default"]
        ratios_default = [ratios_default[0], ratios_default[1] + ratios_default[2]]

        new_household = sample_set(copy.deepcopy(household), ratios_default)
        province_name = province["province"]
        write_household_CSV(new_household, f"province_{province_name}")


if __name__ == '__main__':
    lowest = 0
    highest = 99
    total = 26079
    HH_amount = 8766

    random.seed(datetime.datetime.now())
    generate_households()

    # OG
    # 0.27121438705471834 7073
    # 0.6487595383258561 16919
    # 0.0800260746194256 2087
