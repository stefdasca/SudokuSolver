# SudokuSolver
  
### What is Sudoku?

Sudoku is a logic-based combinatorial number-placement puzzle. In classic Sudoku, the objective is to fill a 9x9 grid with digits so that each line, column and each of the 9 3x3 subgrids that compose the grid contain all of the digits from 1 to 9. The puzzle starts with a partially completed grid and it has a unique solution. 

### Why did I create a Sudoku Solver?

I have always been interested in logic puzzles and mathematical games and as I was playing sudoku with my girlfriend, I wanted to learn more about the strategies which can be used to solve more and more grids while being faster and I realized that it could be a good opportunity to end up implementing a solver using my favorite programming languages. 

### How does the solver work?

At first, I am using an algorithm based on a few sudoku strategies which fill as many squares as possible, using only notes and logic. The algorithm works as following:

* Add notes at the beginning (i. e: mark all the possible squares with all the possible digits according to the rules of sudoku)
* While the sudoku is not completed, do the following:
    1. Whenever one of these situations occurs, fill the square with the number which fits the description
        1. If in a 3*3 grid there is only one position with a certain number, fill it
        2. If in a 1*9 grid there is only one position with a certain number, fill it
        3. If in a 9*1 grid there is only one position with a certain number, fill it
    2. Notes reduction
        1. After completing each square, remove all the notes with that value on the 3*3, 1*9 and 9*1 grids 
        2. If in a 3*3 grid there is only one row/column with a certain number, remove it from the other 3*3 from the same row/column
        3. If in a 3*3, 1*9, 9*1 grid there is a case where the size of a set is equal to the number of empty slots, remove the numbers from the other slots from 3*3 and 1*9/9*1 grids, depending on the case
        4. If in a 3*3, 1*9, 9*1 grid there is a case where the size of a set is equal to the number of empty slots the numbers show up, remove the numbers from the other slots from 3*3 and 1*9/9*1 grids, depending on the case


* The algorithm will keep running as long as there has been some modification done during the previous step (either a new square gets solved or some notes have been removed)

Then, in case the grid has not been filled completely, I am going to use the remaining notes in order to run a simple brute force generator until it finds the solution of the sudoku algorithm. Because a large part of the sudoku has been filled already using the logic and the notes, the brute force generator will run really fast and the solution will be found in a matter of milliseconds. 

### Implementing the solver in C++ and Python
    
The algorithm has been firstly implemented in C++, which is by far faster than Python and this allowed me to test the performance of the program in a more proper manner. After implementing it in C++, I proceeded to translate it in Python, and while for the grids which can be solved using only logic and notes it is doing rather well, it is struggling with the ones which require brute force, because Python is well known for its bad performance when it comes to recursive algorithms. 

### Data regarding the performance of the Sudoku solver in the context of the three datasets I used

In order to test my program’s performance, I used three datasets, using 1 million grids from each of them. 

1. https://www.kaggle.com/radcliffe/3-million-sudoku-puzzles-with-ratings
2. https://www.kaggle.com/bryanpark/sudoku
3. https://www.kaggle.com/rohanrao/sudoku

The input for each of the datasets is available here: https://drive.google.com/drive/folders/13w9UBtqPJ4czkSe5N5eQL7L9LZYRf6-l?usp=sharing

The results for each of the datasets, assuming the grids were added in a random order

| No. of grids  | set 1 | set 2 | set 3 |
|     :---:     |     :---:      |     :---:      |     :---:      |
| ------------- | ------------- | ------------- | ------------- |
|100 | 0.585s| 0.176s| 0.266s |
|500 | 2.997s| 0.876s| 1.372s|
| 1000| 6.053s| 1.736s| 2.75s|
|2000 | 11.916s| 3.457s| 5.495s|
|4000 |23.587s | 6.916s| 10.899s|
|5000 | 29.508s| 8.641s| 13.644s|
| 8000| 46.93s| 13.8s| 21.915s|
|10000 | 58.516s| 17.249s| 27.351s|
|20000 |117.202s |34.558s |54.229s |
| 40000| 233.799s| 69.073s| 109.298s|
|50000 |292.041s | 86.489s| 136.66s|
| 100000|583.539s |172.958s |272.869s |
|200000 |1166.14s |345.964s |544.796s |
| 400000| 2330.89s|698.793s |1090.5s |
|500000 |2913.3s |871.366s |1364.44s |
|800000 |4659.17s |1394.55s |2187.0s |
|1000000 |5823.8s |1745.38s | 2736.72s|
