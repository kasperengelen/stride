import config, json

if __name__ == "__main__":

    provinces = dict()
    for province in config.PROVINCIES:
        data = dict()
        data["province"] = province.ID
        data["centrumsteden"] = [cs.naam for cs in province.centrum_steden]
        ratios_centrumsteden = dict()
        for cs in province.centrum_steden:
            ratios_centrumsteden[cs.naam] = cs.ratios
        data["ratios_centrumsteden"] = ratios_centrumsteden
        
        # Re-calculate default ratios
        rest_pop = province.inwoners - sum([cs.inwoners for cs in province.centrum_steden])
        for ratio_ix in range(len(province.ratios)):
            teller = (province.ratios[ratio_ix] * province.inwoners) - sum([(cs.ratios[ratio_ix] * cs.inwoners) for cs in province.centrum_steden])
            province.ratios[ratio_ix] = (teller / rest_pop)

        data["ratios_default"] = province.ratios
        provinces[province.naam] = data
    
with open("demographic_profile.json", "w+") as outf:
    data = dict()
    data["brackets"] = config.BRACKETS
    data["provinces"] = provinces
    outf.write(json.dumps(data, indent=4))