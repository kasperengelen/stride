class CentrumStad:

    def __init__(self, naam, inwoners, ratios):
        self.naam = naam
        self.inwoners = inwoners
        self.ratios = ratios


class Province:

    def __init__(self, ID, naam, inwoners, ratios, CentrumSteden):
        self.ID = ID
        self.naam = naam
        self.inwoners = inwoners
        self.ratios = ratios
        self.centrum_steden = CentrumSteden


# Configures data as given by https://provincies.incijfers.be
BRACKETS = [17, 18, 64, 65]

CS_ANTWERPEN = CentrumStad("Antwerpen", 523496, [0.226, 0.608, 0.166])
CS_TURNHOUT = CentrumStad("Turnhout", 44211, [0.194, 0.613, 0.193])
CS_MECHELEN = CentrumStad("Mechelen", 86431, [0.221, 0.608, 0.171])
PROV_ANTWERPEN = Province("antwerp", 1, 1849967, [0.201, 0.608, 0.191], [CS_ANTWERPEN, CS_TURNHOUT, CS_MECHELEN])

CS_AALST = CentrumStad("Aalst", 85764, [0.195, 0.609, 0.196])
CS_GENT = CentrumStad("Gent", 260537, [0.193, 0.642, 0.166])
CS_SINT_NIKLAAS = CentrumStad("Sint-Niklaas", 77390, [0.215, 0.588, 0.198])
PROV_OOST_VLAANDEREN = Province("east_flanders", 4, 1506866, [0.195, 0.610, 0.196],
                                [CS_AALST, CS_GENT, CS_SINT_NIKLAAS])

CS_BRUGGE = CentrumStad("Brugge", 118579, [0.166, 0.599, 0.234])
CS_KORTRIJK = CentrumStad("Kortrijk", 76441, [0.193, 0.593, 0.214])
CS_OOSTENDE = CentrumStad("Oostende", 71481, [0.155, 0.568, 0.278])
CS_ROESELARE = CentrumStad("Roeselare", 62445, [0.192, 0.608, 0.200])
PROV_WEST_VLAANDEREN = Province("west_flanders", 3, 1192886, [0.179, 0.591, 0.229],
                                [CS_BRUGGE, CS_KORTRIJK, CS_OOSTENDE, CS_ROESELARE])

CS_GENK = CentrumStad("Genk", 66159, [0.198, 0.614, 0.189])
CS_HASSELT = CentrumStad("Hasselt", 77709, [0.168, 0.615, 0.218])
PROV_LIMBURG = Province("limburg", 7, 872923, [0.186, 0.617, 0.197], [CS_GENK, CS_HASSELT])

CS_LEUVEN = CentrumStad("Leuven", 101461, [0.173, 0.666, 0.161])
PROV_VLAAMS_BRABANT = Province("flemish_brabant", 2, 1139541, [0.205, 0.605, 0.191], [CS_LEUVEN])

PROV_HENEGOUWEN = Province("hainaut", 5, 1341645, [0.20891517502767126,
                                                   0.6076100607835903,
                                                   0.18347476418873845], [])

PROV_LUIK = Province("liege", 6, 1105326, [0.20550498224053357,
                                           0.6097142381523641,
                                           0.18478077960710235], [])

PROV_LUXEMBURG = Province("luxemburg", 8, 283227, [0.2202826707905673,
                                                   0.6116719098108584,
                                                   0.16804541939857429], [])

PROV_NAMEN = Province("namur", 9, 493073, [0.20590054616659195,
                                           0.6116254591105171,
                                           0.18247399472289094], [])

PROVINCIES = [PROV_ANTWERPEN, PROV_OOST_VLAANDEREN, PROV_WEST_VLAANDEREN, PROV_LIMBURG, PROV_VLAAMS_BRABANT,
              PROV_HENEGOUWEN, PROV_LUIK, PROV_LUXEMBURG, PROV_NAMEN]
