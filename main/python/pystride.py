# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_pystride')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_pystride')
    _pystride = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_pystride', [dirname(__file__)])
        except ImportError:
            import _pystride
            return _pystride
        try:
            _mod = imp.load_module('_pystride', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _pystride = swig_import_helper()
    del swig_import_helper
else:
    import _pystride
del _swig_python_version_info

try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        if _newclass:
            object.__setattr__(self, name, value)
        else:
            self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr(self, class_type, name):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    raise AttributeError("'%s' object has no attribute '%s'" % (class_type.__name__, name))


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except __builtin__.Exception:
    class _object:
        pass
    _newclass = 0

Id_Household = _pystride.Id_Household
Id_K12School = _pystride.Id_K12School
Id_College = _pystride.Id_College
Id_Workplace = _pystride.Id_Workplace
Id_PrimaryCommunity = _pystride.Id_PrimaryCommunity
Id_SecondaryCommunity = _pystride.Id_SecondaryCommunity

def NumOfTypes():
    return _pystride.NumOfTypes()
NumOfTypes = _pystride.NumOfTypes

def IsId(s):
    return _pystride.IsId(s)
IsId = _pystride.IsId

def ToId(s):
    return _pystride.ToId(s)
ToId = _pystride.ToId

def ToSizeT(id):
    return _pystride.ToSizeT(id)
ToSizeT = _pystride.ToSizeT

def ToString(w):
    return _pystride.ToString(w)
ToString = _pystride.ToString
HealthStatus_Susceptible = _pystride.HealthStatus_Susceptible
HealthStatus_Exposed = _pystride.HealthStatus_Exposed
HealthStatus_Infectious = _pystride.HealthStatus_Infectious
HealthStatus_Symptomatic = _pystride.HealthStatus_Symptomatic
HealthStatus_InfectiousAndSymptomatic = _pystride.HealthStatus_InfectiousAndSymptomatic
HealthStatus_Recovered = _pystride.HealthStatus_Recovered
HealthStatus_Immune = _pystride.HealthStatus_Immune
class Health(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Health, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Health, name)
    __repr__ = _swig_repr

    def __init__(self, start_infectiousness=0, start_symptomatic=0, time_infectious=0, time_symptomatic=0):
        this = _pystride.new_Health(start_infectiousness, start_symptomatic, time_infectious, time_symptomatic)
        try:
            self.this.append(this)
        except __builtin__.Exception:
            self.this = this

    def GetEndInfectiousness(self):
        return _pystride.Health_GetEndInfectiousness(self)

    def GetEndSymptomatic(self):
        return _pystride.Health_GetEndSymptomatic(self)

    def GetStartInfectiousness(self):
        return _pystride.Health_GetStartInfectiousness(self)

    def GetStartSymptomatic(self):
        return _pystride.Health_GetStartSymptomatic(self)

    def IsImmune(self):
        return _pystride.Health_IsImmune(self)

    def IsInfected(self):
        return _pystride.Health_IsInfected(self)

    def IsInfectious(self):
        return _pystride.Health_IsInfectious(self)

    def IsRecovered(self):
        return _pystride.Health_IsRecovered(self)

    def IsSusceptible(self):
        return _pystride.Health_IsSusceptible(self)

    def IsSymptomatic(self):
        return _pystride.Health_IsSymptomatic(self)

    def SymptomsStartedToday(self):
        return _pystride.Health_SymptomsStartedToday(self)

    def SetImmune(self):
        return _pystride.Health_SetImmune(self)

    def SetSusceptible(self):
        return _pystride.Health_SetSusceptible(self)

    def StartInfection(self):
        return _pystride.Health_StartInfection(self)

    def StopInfection(self):
        return _pystride.Health_StopInfection(self)

    def Update(self):
        return _pystride.Health_Update(self)
    __swig_destroy__ = _pystride.delete_Health
    __del__ = lambda self: None
Health_swigregister = _pystride.Health_swigregister
Health_swigregister(Health)
cvar = _pystride.cvar
IdPack = cvar.IdPack
IdList = cvar.IdList

class SwigPyIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SwigPyIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SwigPyIterator, name)

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _pystride.delete_SwigPyIterator
    __del__ = lambda self: None

    def value(self):
        return _pystride.SwigPyIterator_value(self)

    def incr(self, n=1):
        return _pystride.SwigPyIterator_incr(self, n)

    def decr(self, n=1):
        return _pystride.SwigPyIterator_decr(self, n)

    def distance(self, x):
        return _pystride.SwigPyIterator_distance(self, x)

    def equal(self, x):
        return _pystride.SwigPyIterator_equal(self, x)

    def copy(self):
        return _pystride.SwigPyIterator_copy(self)

    def next(self):
        return _pystride.SwigPyIterator_next(self)

    def __next__(self):
        return _pystride.SwigPyIterator___next__(self)

    def previous(self):
        return _pystride.SwigPyIterator_previous(self)

    def advance(self, n):
        return _pystride.SwigPyIterator_advance(self, n)

    def __eq__(self, x):
        return _pystride.SwigPyIterator___eq__(self, x)

    def __ne__(self, x):
        return _pystride.SwigPyIterator___ne__(self, x)

    def __iadd__(self, n):
        return _pystride.SwigPyIterator___iadd__(self, n)

    def __isub__(self, n):
        return _pystride.SwigPyIterator___isub__(self, n)

    def __add__(self, n):
        return _pystride.SwigPyIterator___add__(self, n)

    def __sub__(self, *args):
        return _pystride.SwigPyIterator___sub__(self, *args)
    def __iter__(self):
        return self
SwigPyIterator_swigregister = _pystride.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _pystride.SHARED_PTR_DISOWN
class Person(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Person, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Person, name)
    __repr__ = _swig_repr

    def __init__(self, *args):
        this = _pystride.new_Person(*args)
        try:
            self.this.append(this)
        except __builtin__.Exception:
            self.this = this

    def __ne__(self, p):
        return _pystride.Person___ne__(self, p)

    def GetAge(self):
        return _pystride.Person_GetAge(self)

    def GetHealth(self, *args):
        return _pystride.Person_GetHealth(self, *args)

    def GetId(self):
        return _pystride.Person_GetId(self)

    def GetPoolId(self, poolType):
        return _pystride.Person_GetPoolId(self, poolType)

    def IsInPool(self, poolType):
        return _pystride.Person_IsInPool(self, poolType)

    def IsSurveyParticipant(self):
        return _pystride.Person_IsSurveyParticipant(self)

    def ParticipateInSurvey(self):
        return _pystride.Person_ParticipateInSurvey(self)

    def Update(self, isWorkOff, isSchoolOff, adaptiveSymptomaticBehavior):
        return _pystride.Person_Update(self, isWorkOff, isSchoolOff, adaptiveSymptomaticBehavior)

    def SetAge(self, newAge):
        return _pystride.Person_SetAge(self, newAge)

    def SetId(self, id):
        return _pystride.Person_SetId(self, id)

    def SetPoolId(self, type, poolId):
        return _pystride.Person_SetPoolId(self, type, poolId)
    __swig_destroy__ = _pystride.delete_Person
    __del__ = lambda self: None
Person_swigregister = _pystride.Person_swigregister
Person_swigregister(Person)

class PopulationBaseClass(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, PopulationBaseClass, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, PopulationBaseClass, name)
    __repr__ = _swig_repr

    def __init__(self, *args):
        this = _pystride.new_PopulationBaseClass(*args)
        try:
            self.this.append(this)
        except __builtin__.Exception:
            self.this = this
    __swig_destroy__ = _pystride.delete_PopulationBaseClass
    __del__ = lambda self: None

    def at(self, *args):
        return _pystride.PopulationBaseClass_at(self, *args)

    def back(self, *args):
        return _pystride.PopulationBaseClass_back(self, *args)

    def begin(self, *args):
        return _pystride.PopulationBaseClass_begin(self, *args)

    def cbegin(self):
        return _pystride.PopulationBaseClass_cbegin(self)

    def end(self, *args):
        return _pystride.PopulationBaseClass_end(self, *args)

    def cend(self):
        return _pystride.PopulationBaseClass_cend(self)

    def capacity(self):
        return _pystride.PopulationBaseClass_capacity(self)

    def empty(self):
        return _pystride.PopulationBaseClass_empty(self)

    def get_block_count(self):
        return _pystride.PopulationBaseClass_get_block_count(self)

    def get_elements_per_block(self):
        return _pystride.PopulationBaseClass_get_elements_per_block(self)

    def size(self):
        return _pystride.PopulationBaseClass_size(self)

    def resize(self, *args):
        return _pystride.PopulationBaseClass_resize(self, *args)

    def clear(self):
        return _pystride.PopulationBaseClass_clear(self)

    def pop_back(self):
        return _pystride.PopulationBaseClass_pop_back(self)

    def push_back(self, *args):
        return _pystride.PopulationBaseClass_push_back(self, *args)

    def __getitem__(self, pos):
        return _pystride.PopulationBaseClass___getitem__(self, pos)
PopulationBaseClass_swigregister = _pystride.PopulationBaseClass_swigregister
PopulationBaseClass_swigregister(PopulationBaseClass)

class Population(PopulationBaseClass):
    __swig_setmethods__ = {}
    for _s in [PopulationBaseClass]:
        __swig_setmethods__.update(getattr(_s, '__swig_setmethods__', {}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, Population, name, value)
    __swig_getmethods__ = {}
    for _s in [PopulationBaseClass]:
        __swig_getmethods__.update(getattr(_s, '__swig_getmethods__', {}))
    __getattr__ = lambda self, name: _swig_getattr(self, Population, name)

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    if _newclass:
        Create = staticmethod(_pystride.Population_Create)
    else:
        Create = _pystride.Population_Create

    def CreatePerson(self, id, age, householdId, k12SchoolId, college, workId, primaryCommunityId, secondaryCommunityId):
        return _pystride.Population_CreatePerson(self, id, age, householdId, k12SchoolId, college, workId, primaryCommunityId, secondaryCommunityId)

    def GetInfectedCount(self):
        return _pystride.Population_GetInfectedCount(self)

    def CRefPoolSys(self):
        return _pystride.Population_CRefPoolSys(self)

    def CRefGeoGrid(self):
        return _pystride.Population_CRefGeoGrid(self)

    def RefContactLogger(self):
        return _pystride.Population_RefContactLogger(self)

    def RefPoolSys(self):
        return _pystride.Population_RefPoolSys(self)

    def RefGeoGrid(self):
        return _pystride.Population_RefGeoGrid(self)
    __swig_destroy__ = _pystride.delete_Population
    __del__ = lambda self: None
Population_swigregister = _pystride.Population_swigregister
Population_swigregister(Population)

def Population_Create(*args):
    return _pystride.Population_Create(*args)
Population_Create = _pystride.Population_Create

class Sim(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Sim, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Sim, name)

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    if _newclass:
        Create = staticmethod(_pystride.Sim_Create)
    else:
        Create = _pystride.Sim_Create

    def GetCalendar(self):
        return _pystride.Sim_GetCalendar(self)

    def GetConfigValue(self, attribute):
        return _pystride.Sim_GetConfigValue(self, attribute)

    def GetPopulation(self):
        return _pystride.Sim_GetPopulation(self)

    def GetTransmissionRate(self):
        return _pystride.Sim_GetTransmissionRate(self)

    def RefRnManager(self):
        return _pystride.Sim_RefRnManager(self)

    def RefTransmissionProfile(self):
        return _pystride.Sim_RefTransmissionProfile(self)

    def TimeStep(self):
        return _pystride.Sim_TimeStep(self)
    __swig_destroy__ = _pystride.delete_Sim
    __del__ = lambda self: None
Sim_swigregister = _pystride.Sim_swigregister
Sim_swigregister(Sim)

def Sim_Create(config, pop, rnMan):
    return _pystride.Sim_Create(config, pop, rnMan)
Sim_Create = _pystride.Sim_Create


def CreateSim(config):
    return _pystride.CreateSim(config)
CreateSim = _pystride.CreateSim
class TransmissionProfile(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, TransmissionProfile, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, TransmissionProfile, name)
    __repr__ = _swig_repr

    def __init__(self):
        this = _pystride.new_TransmissionProfile()
        try:
            self.this.append(this)
        except __builtin__.Exception:
            self.this = this

    def GetRate(self):
        return _pystride.TransmissionProfile_GetRate(self)

    def Initialize(self, configPt, diseasePt):
        return _pystride.TransmissionProfile_Initialize(self, configPt, diseasePt)
    __swig_destroy__ = _pystride.delete_TransmissionProfile
    __del__ = lambda self: None
TransmissionProfile_swigregister = _pystride.TransmissionProfile_swigregister
TransmissionProfile_swigregister(TransmissionProfile)

# This file is compatible with both classic and new-style classes.


