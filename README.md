# Map-Maker

## Problem Overview
John is a map maker. He is standing at point (0,0) and must explore the area. John can move one space at a time: left, right, up, or down. If John is standing at (x,y), he can move to (x+1, y), (x-1, y), (x, y+1), or (x, y-1). 

Bill is an assassin, and must stop John from exploring. Therefore, Bill decided to plant mines. Bill has placed mines at points where the sum of the absolute value of the coordinate's digits is greater than 21. For example, the point (59, -79) has a mine, because 5 + 9 + 7 + 9 > 21. But the point (-113, -104) has no mine because 1 + 1 + 3 + 1 + 0 + 4 <= 21.

If John steps on a mine he will die. John cannot jump over the mines, so he must walk around the mines to avoid them. How many points can John access while making his map, given that he starts at (0, 0).

## Assumptions
Given the digit sum is the absolute value of the coordinate system, we can simply calculate the number of valid points in one quadrant and multiply that value by 4. We will then remove the overlapping points (those at the axis) to get our final answer.

Our program will use unsigned values, and perform calculations in Quadrant I.