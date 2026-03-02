### The Skyline Problem using Divide and Conquer algorithm
Given n rectangular buildings in a 2-dimensional city, computes the skyline of these buildings, eliminating hidden lines. The main task is to view buildings from a side and remove all sections that are not visible.
All buildings share common bottom and every building is represented by triplet (left, ht, right)

‘left’: is x coordinated of left side (or wall).
‘right’: is x coordinate of right side
‘ht’: is height of building.

* A skyline is a collection of rectangular strips. 
* A rectangular strip is represented as a pair (left, ht) where left is x coordinate of left side of strip and ht is height of strip.

###Examples:
Input: Array of buildings
       { (1,11,5), (2,6,7), (3,13,9), (12,7,16), (14,3,25),
         (19,18,22), (23,13,29), (24,4,28) }
Output: Skyline (an array of rectangular strips)
        A strip has x coordinate of left side and height 
        (1, 11), (3, 13), (9, 0), (12, 7), (16, 3), (19, 18),  
        (22, 3), (25, 0)

Consider following as another example when there is only one
building
Input:  {(1, 11, 5)}
Output: (1, 11), (5, 0)

## A Simple Solution 
1. initialize skyline or result as empty
2. then one by one add buildings to skyline. 
3. A building is added by first finding the overlapping strip(s). 
4. If there are no overlapping strips, the new building adds new strip(s). 
5. If overlapping strip is found, then height of the existing strip may increase. 
-> Time complexity of this solution is O(n2)

## Better Solution by Divide and Conquer
- The idea is similar to Merge Sort, divide the given set of buildings in two subsets. 
- Recursively construct skyline for two halves and finally merge the two skylines.

### How to Merge two Skylines?
The idea is similar to merge of merge sort:
1. start from first strips of two skylines, compare x coordinates. 
2. Pick the strip with smaller x coordinate and add it to result. 
3. The height of added strip is considered as maximum of current heights from skyline1 and skyline2.

### Example to show working of merge:
Height of new Strip is always obtained by takin maximum of following
     (a) Current height from skyline1, say 'h1'.  
     (b) Current height from skyline2, say 'h2'
  h1 and h2 are initialized as 0. 
  h1 is updated when a strip from SkyLine1 is added to result 
  h2 is updated when a strip from SkyLine2 is added.
 
  Skyline1 = {(1, 11),  (3, 13),  (9, 0),  (12, 7),  (16, 0)}
  Skyline2 = {(14, 3),  (19, 18), (22, 3), (23, 13),  (29, 0)}
  Result = {}
  h1 = 0, h2 = 0
 
  Compare (1, 11) and (14, 3).  Since first strip has smaller left x,
  add it to result and increment index for Skyline1. 
  h1 = 11, New Height  = max(11, 0)   
  Result =   {(1, 11)}

  Compare (3, 13) and (14, 3). Since first strip has smaller left x,
  add it to result and increment index for Skyline1
  h1 = 13, New Height =  max(13, 11)
  Result =  {(1, 11), (3, 13)}   
  
  Similarly (9, 0) and (12, 7) are added.
  h1 = 0-> 7, New Height =  max(0, 7) = 7
  Result =   {(1, 11), (3, 13), (9, 0), (12, 7)}

  Compare (16, 0) and (14, 3). Since second strip has smaller left x, 
  it is added to result.
  h2 = 3, New Height =  max(7, 3) = 7? 
  Result =   {(1, 11), (3, 13), (9, 0), (12, 7), (14, 3)}

  Compare (16, 0) and (19, 18). Since first strip has smaller left x, 
  it is added to result.
  h1 = 0, New Height =  max(0, 3) = 3
  Result =   {(1, 11), (3, 13), (9, 0), (12, 7), (14, 3), (16, 3)}

Since Skyline1 has no more items, all remaining items of Skyline2 
are added 
  Result =   {(1, 11), (3, 13), (9, 0), (12, 7), (14, 3), (16, 3), 
              (19, 18), (22, 3), (23, 13), (29, 0)}

One observation about above output is, the strip (16, 3) is redundant
(There is already an strip of same height). We remove all redundant 
strips. 
  Result =   {(1, 11), (3, 13), (9, 0), (12, 7), (14, 3), (19, 18), 
              (22, 3), (23, 13), (29, 0)}

In below code, redundancy is handled by not appending a strip if the 
previous strip in result has same height.

