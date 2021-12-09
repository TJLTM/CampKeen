import re

class PIStesting():
    def __init__(self):
        self.Results = {}
        self.IO = {
            'Input':3,
            'Output':4,
        }
        self.States = {
            'On' : ['on','On','oN','ON'],
            'Off': ['off','Off','oFf','ofF','OFf','OfF','oFF','OFF'],
            'Units': ['I','i','M','m']
        }
        self.Commands = {
            "single": {
                1 : {'command':'%UNITS?\r','response':'', "Regex": re.compile()},
                2 : {'command':'%DEVICE?\r','response':'', "Regex": re.compile()},
                3 : {'command':'%WATERSOURCE?\r','response':'', "Regex": re.compile()},
                4 : {'command':'%WATERLEVEL?\r','response':'', "Regex": re.compile()},
                5 : {'command':'%LPG?\r','response':'', "Regex": re.compile()},
                6 : {'command':'%SEWAGE?\r','response':'', "Regex": re.compile()},
                7 : {'command':'%GREY?\r','response':'', "Regex": re.compile()},
                8 : {'command':'%ENERGY?\r','response':'', "Regex": re.compile()},
                9 : {'command':'%BATTERY?\r','response':'', "Regex": re.compile()},
                10 : {'command':'%RTCBATTERY?\r','response':'', "Regex": re.compile()},
                11 : {'command':'%GENERATOR?\r','response':'', "Regex": re.compile()},
                12 : {'command':'%TEMPS?\r','response':'', "Regex": re.compile()},
                13 : {'command':'%UNITTEMP?\r','response':'', "Regex": re.compile()},
                14 : {'command':'%WATERPUMPSENSE?\r','response':'', "Regex": re.compile()},
                15 : {'command':'%WARNING?\r','response':'', "Regex": re.compile()},
                16 : {'command':'%WATER?\r','response':'', "Regex": re.compile()},
                17 : {'command':'%STREAMING?\r','response':'', "Regex": re.compile()},
                18 : {'command':'%ACENMON?\r','response':'', "Regex": re.compile()},
                19 : {'command':'%ALLDATA?\r','response':'', "Regex": re.compile()},
                20 : {'command':'%UPDATEALL\r','response':'', "Regex": re.compile()},
                21 : {'command':'%RESETWARNINGS\r','response':'', "Regex": re.compile()},
                22 : {'command':'%RESETALLALARMS\r','response':'', "Regex": re.compile()},
            },
            "parameters":{
                1 : {'command':'%SETUNITS', 'States':['Units'], "Regex": re.compile()},
                2 : {'command':'%SETWATERPUMPSENSE', 'States':['On','Off'], "Regex": re.compile()},
                3 : {'command':'%WATER', 'States':['On','Off'], "Regex": re.compile()},
                4 : {'command':'%SETSTREAMINGDATA', 'States':['On','Off'], "Regex": re.compile()},
                5 : {'command':'%SETOUTPUT', 'States':['On','Off'], "Regex": re.compile()},
                6 : {'command':'%READINPUT', 'States':['On','Off'], "Regex": re.compile()},
                7 : {'command':'%GETOUTPUT', 'States':['On','Off'], "Regex": re.compile()},
                8 : {'command':'%SETACENMON', 'States':['On','Off'], "Regex": re.compile()},
            }
        }

    def RecordResults(self,Name,Command,Response,Result):
        pass

    def GenerateParamCommands(self,ID):
        CommandList = []
        if ID not in [5,6,7]:
            for AcceptedStates in self.Commands["parameters"][ID]['States']:
                for item in self.States[AcceptedStates]:
                    CommandList.append(self.Commands["parameters"][ID]['command'] + '*' + item + '\r')
        elif ID == 5:
            for IONumber in range(1,self.IO['Output']+1):
                for AcceptedStates in self.Commands["parameters"][ID]['States']:
                    for item in self.States[AcceptedStates]:
                        CommandList.append("{0}*{1}*{2}\r".format(self.Commands["parameters"][ID]['command'],IONumber,item))
        elif ID == 6: 
            for IONumber in range(1,self.IO['Input']+1):
                CommandList.append("{0}*{1}\r".format(self.Commands["parameters"][ID]['command'],IONumber))
        elif ID == 7: 
            for IONumber in range(1,self.IO['Output']+1):
                CommandList.append("{0}*{1}\r".format(self.Commands["parameters"][ID]['command'],IONumber))

        return CommandList

    def GenerateAllSingleCommads(self):
        CommandList = []
        for Key in self.Commands["single"]:
            CommandList.append(self.GenerateSingleCommand(Key))
        return CommandList

    def GenerateSingleCommand(self,ID):
        return self.Commands["single"][ID]['command']


test = PIStesting()
print(test.GenerateAllSingleCommads())
print(test.GenerateParamCommands(5))
print(test.GenerateParamCommands(6))
print(test.GenerateParamCommands(7))