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
                1 : {'command':'%UNITS?\r','response':'' },
                2 : {'command':'%DEVICE?\r','response':'' },
                3 : {'command':'%WATERSOURCE?\r','response':'' },
                4 : {'command':'%WATERLEVEL?\r','response':'' },
                5 : {'command':'%LPG?\r','response':'' },
                6 : {'command':'%SEWAGE?\r','response':'' },
                7 : {'command':'%GREY?\r','response':'' },
                8 : {'command':'%ENERGY?\r','response':'' },
                9 : {'command':'%BATTERY?\r','response':'' },
                10 : {'command':'%RTCBATTERY?\r','response':'' },
                11 : {'command':'%GENERATOR?\r','response':'' },
                12 : {'command':'%TEMPS?\r','response':'' },
                13 : {'command':'%UNITTEMP?\r','response':'' },
                14 : {'command':'%WATERPUMPSENSE?\r','response':'' },
                15 : {'command':'%WARNING?\r','response':'' },
                16 : {'command':'%WATER?\r','response':'' },
                17 : {'command':'%STREAMING?\r','response':'' },
                18 : {'command':'%ACENMON?\r','response':'' },
                19 : {'command':'%ALLDATA?\r','response':'' },
                20 : {'command':'%UPDATEALL\r','response':'' },
                21 : {'command':'%RESETWARNINGS\r','response':'' },
                22 : {'command':'%GETTIME\r','response':'' },
                23 : {'command':'%ACVOLTAGEGAIN?\r','response':'' },
                24 : {'command':'%ACFREQ?\r','response':'' },
                25 : {'command':'%ACPGAGAIN?\r','response':'' },
                26 : {'command':'%ACLEGS?\r','response':'' },
                27 : {'command':'%ACCT1GAIN?\r','response':''},
                28 : {'command':'%ACCT2GAIN?\r','response':''},

            },
            "parameters":{
                1 : {'command':'%SETUNITS', 'States':['Units'] },
                2 : {'command':'%SETWATERPUMPSENSE', 'States':['On','Off'] },
                3 : {'command':'%WATER', 'States':['On','Off'] },
                4 : {'command':'%SETSTREAMINGDATA', 'States':['On','Off'] },
                5 : {'command':'%SETOUTPUT', 'States':['On','Off'] },
                6 : {'command':'%READINPUT', 'States':['On','Off'] },
                7 : {'command':'%GETOUTPUT', 'States':['On','Off'] },
                8 : {'command':'%SETACENMON', 'States':['On','Off'] },
                9 : {'command':'%SETACFREQ', 'States':['60','50'] },
                10 : {'command':'%SETACPGAGAIN', 'States':[0,21,42] },
                11 : {'command':'%SETACVOLTAGEGAIN', 'States':[0,1975,60000] },
                12 : {'command':'%SETACLEGS', 'States':[1,2] },
                13 : {'command':'%SETACCT1GAIN', 'States':[0,1975,60000] },
                14 : {'command':'%SETACCT2GAIN', 'States':[0,1975,60000] },
                
            }
        }

    def RecordResults(self,Name,Command,Response,Result):
        pass

    def GenerateParamCommands(self,ID):
        CommandList = []
        if ID == 5:
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
        if ID in [10,11,12,13,14]:
            for State in self.Commands["parameters"][ID]['States']:
                CommandList.append("{0}*{1}\r".format(self.Commands["parameters"][ID]['command'],State))
        else:
             for AcceptedStates in self.Commands["parameters"][ID]['States']:
                for item in self.States[AcceptedStates]:
                    CommandList.append(self.Commands["parameters"][ID]['command'] + '*' + item + '\r')


        return CommandList

    def GenerateAllSingleCommads(self):
        CommandList = []
        for Key in self.Commands["single"]:
            CommandList.append(self.GenerateSingleCommand(Key))
        return CommandList

    def GenerateSingleCommand(self,ID):
        return self.Commands["single"][ID]['command']


test = PIStesting()
#print(test.GenerateAllSingleCommads())
#print(test.GenerateParamCommands(5))
#print(test.GenerateParamCommands(6))
#print(test.GenerateParamCommands(7))
print(test.GenerateParamCommands(10))
print(test.GenerateParamCommands(11))
print(test.GenerateParamCommands(12))
print(test.GenerateParamCommands(13))
print(test.GenerateParamCommands(14))