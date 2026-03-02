#!/usr/bin/pyhton

import copy
def cellCompete(states, days):
  next_states = [0] * len(states)  # init
  for day in range(0, days):
    if states[1] == 0:
      next_states[0] = 0
    else:
      next_states[0] = 1

    if states[-2] == 0:
      next_states[-1] = 0
    else:
      next_states[-1] = 1
    
    for idx in range(1, len(states)-1):
      print(idx, states[idx-1], states[idx+1])
      if states[idx-1] == states[idx+1]:
        next_states[idx] = 0
      else:
        next_states[idx] = 1        
    print('next_states', next_states)
    states = copy.deepcopy(next_states)
    print('states', states)
  return states

def main():
  print(cellCompete([1, 0, 0, 0, 0, 1, 0, 0], 1))
  print(cellCompete([1, 1, 1, 0, 1, 1, 1, 1], 2))

if __name__ == '__main__':
  main()



