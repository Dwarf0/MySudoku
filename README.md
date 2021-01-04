Last summer holidays, I did some Sudoku puzzles. I got stuck on some hard ones and checking possible values of each cell for each strategies was getting tedious.
That's why I decided to automate these checks and make a simple Sudoku solver (only using the tricks I know of) with an UI on top of it.

I am developping this project on Windows with Visual Studio and using :
- cmake 3.1.0 to set the environment,
- C++ 11 as programming language,
- Qt5 as a framework for my UI,
- GTest for testing the solver
- Doxygen for documentation 

At some point, I plan to use QTranslator for localization of the few sentences.


Some features that I'll also try to add later:
- save current state of the Sudoku (and then reload it to come back to it later)
- add an undo/redo
- run my solver on a QThread worker right after loading a Sudoku
- initialize a Sudoku from a picture by processing the picture of a Sudoku with OpenCV , segmenting each cell and send each one through a MNIST trained convolutional neural network to get the associated number
