import sys
num = 30

RESPONSE = 2
REQUEST = 1
PULSE = 0

states = [[0, 0, 0] for i in range(num)]
nstate = [[0, 0, 0] for i in range(num)]

def printstate():
    for i in range(num):
        pass
#        print i, 
#    print ''
    for elt in states:
        print elt[0],
    print ''
    for elt in states:
        print elt[1],
    print ''
    for elt in states:
        print elt[2],
    print ''
    print ''
    
def update_cell(i):
    # this propogates a steady pulse
    if i == 0:
        if states[i][PULSE] == 0:
            nstate[i][PULSE] = 1
        if states[i+1][REQUEST] == 1:
            nstate[i][RESPONSE] = 1
        else:
            nstate[i][RESPONSE] = 0
    elif i > 0:
        if states[i][PULSE] is 1: # we're active
            if states[i][RESPONSE] is 1:
                nstate[i][PULSE] = 0
                nstate[i][REQUEST] = 1
                nstate[i][RESPONSE] = 0
            elif i > 1:
                nstate[i][REQUEST] = 1
                nstate[i][PULSE] = 0
            else:
                nstate[i][REQUEST] = states[i+1][REQUEST] # pass requests left
                nstate[i][RESPONSE] = states[i-1][RESPONSE] # pass responses right
        else:
            nstate[i][PULSE] = states[i-1][PULSE]
 #           nstate[i][REQUEST] = 0
#            

def switch_states():
    global states
    for i in range(len(states)):
        states[i] = nstate[i][:]


def sim():
    for i in range(num):
        update_cell(i)
    switch_states()
    printstate()


for i in range(int(sys.argv[1])):
    sim()
    raw_input('')
