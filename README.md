Last summer holidays, I did some Sudoku puzzles. I got stuck on some hard ones and checking possible values of each cell for each strategies was getting tedious.
That's why I decided to automate these checks and make a simple Sudoku solver (only using the tricks I know of) with an UI on top of it.

I am developping this project on Windows with Visual Studio and using :
- cmake 3.1.0 to set the environment,
- C++ 11 as programming language,
- Qt5 as a framework for my UI,
- GTest for testing the solver

I also plan to add Doxygen for documentation and use QTranslator for localization of the few words (for the sake of it).

Some features that could be added:
- being able to save current state of the Sudoku and then reload it (to come back at it later)
- add an undo/redo
- add a proper solver (might run it a QThread worker right after loading the csv)
