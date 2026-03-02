#!/usr/bin/pyhton

def orderedJunctionBoxes(numberOfBoxes, boxList):
  older_box = []
  newer_box = []
  older_map = {}
  newer_map = {}

  for box in boxList:
    words = box.split()
    merged_id = ''.join(words[1:])  # merge all id words
    id = ' '.join(words[1:])

    print(merged_id, id)
    if merged_id.isalpha():  # True if older box      
      older_map[id] = box
    else:      
      newer_map[id] = box
  
  # sort each box by key
  for key in sorted(older_map.keys()):
    # print('%s %s' % (key, older_map[key]))
    older_box.append(older_map[key])
  for key in sorted(newer_map.keys()):
    # print('%s %s' % (key, newer_map[key]))
    newer_box.append(newer_map[key])

  # add two sorted box
  return older_box+newer_box




def main():
  print(orderedJunctionBoxes(4, ['a1 alps cow bar', 'mi2 jog mid pet', 'wz3 34 54 398', 'x4 45 21 7']))
  

if __name__ == '__main__':
  main()