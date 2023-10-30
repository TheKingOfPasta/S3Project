# S3Project

EPITA S3 project (sudoku), presented by Nord En Face


How to use:

 - Using the command line : go in the solver/ folder, run the 'make' command, and launch ./solver {path}, with path being the path to a text file (assumed correct)
 
 - To test the image processing functions, go in the img_processing/ folder, run 'make', and execute ./img_action
   - ./img_action is used to apply all image transformations (rotate/rotate auto/adaptive thresholding/sobel/inverting)
        - execute ./img_action without parameters to get a tutorial
        - execute ./img_action with a single parameter (example: ./img_action -t) to get help on that function 

 - Using the GUI : go in the gui/ folder, run the 'make' command, and launch ./gui



Using the GUI:
 - On the left are the functions you can execute, on the right are the files of your system.
 - Simply click on a file to choose it, or click on a folder to go into the folder.
 - Folders are in blue and files in white
 - To go back, click on the .. button from the list
