# Minesweeper API

This project injects code into the original minesweeper in order to provide an API to interact with the game.

You can also change the smiley button to anything by adding 24x24 px \*.png files to the same folder as the program named:

- `ButtonChecking.png`
- `ButtonDown.png`
- `ButtonIdle.png`
- `ButtonLost.png`
- `ButtonWon.png`

Examples can be seen in the `icons` folder.

## Get set up for building

First download `winmine.exe` for Windows 2000/NT 4.0. It can be found on [archive.org](https://archive.org/details/BestOfWindowsEntertainmentPack64Bit). Other version of `winmine.exe` is not tested and is likely not to work since function call addresses possibly will be changed. For the install target of cmake to work move `winmine.exe` to `external/` in the repository.

To be able to build this, visual studio is needed as well as CMake. Other compilers than MSVC might work but is not tested.

To build it run from the repository root:

```powershell
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

Move `external/winmine.exe` to the `bin` folder or run the install target.

_Please note that the install target is incomplete right now and does only install the winmine.exe file and no of the API and injection files._

## Running the program with the API

For the API to be run, instead of running `winmine.exe`, `minesweeperRunetime.exe` should be run.

To check if the API is running you can press `F10` to show the API terminal. `F9` will hide it again.

## Using the API

To use the API all the functions needed is included in `minesweeperAPIx64.dll` or `minesweeperAPIx86.dll` dependent on the target architecture.

If run from a `x64` architecture, the program must be started manually. The `startMinesweeper()` function is only available to the `x86` architecture for now.

To include the functions include the file `minesweeperAPI/minesweeperAPI.hpp`. When building the `*.dll` files library files should be created as well to link up against.

### API documentation

```c++
int connect();
```

Since the game is running in another process, it is necessary to connect to the game over RPC in order to interact with it. Therefore this command should be called when the game is running with the API loaded.

---

```c++
int disconnect();
```

When done using the API disconnect the RPC connection with this function.

---

```c++
int ResetField();
```

Resets the field to a new game with all tiles hidden.

---

```c++
void GetField(unsigned char* fieldBuffer);
```

Puts the field in it's current state into `fieldBuffer`. `fieldBuffer` must be at least 720 long.

To read the field is a bit complicated. Here is an example:

```c++
char fieldBuffer[720]; // Data array must be 720 long.
GetField(fieldBuffer)
auto height = GetFieldHeight();
auto width = GetFieldWidth();
for(char x = 0; x < width; ++x){
    for(char y = 0; y < height; ++y){
        auto tile = fieldBuffer[y * 30 + x]
        // Do something with tile
    }
}
```

Each tile in the field can have the ascii values "`F`", "`?`", "` `", `#` or a number between 1-8. Where "`F`" means that there is a flag on the tile, "`?`" means that there is a question mark on the tile. "` `" means that the tile is unveiled but no bombs is around it. "`#`" means that the tile is not yet unveiled. And a number means that the tile has a that number bombs adjacent to it.

_Right now it is not possible to get the field where the bombs are shown._

---

```c++
unsigned char GetFieldHeight();
```

Returns the number of tiles there are vertically.

---

```c++
unsigned char GetFieldWidth();
```

Returns the number of tiles there are horizontally.

---

```c++
void ClickOnTile(unsigned char x, unsigned char y);
```

If the tile specified with the coordinate `(x,y)` is within the board with and height then the tile will be reviled just like when clicking on it.

---

```c++
game_status_t GetGameStatus();
```

`GetGameStatus()` returns a given value of the `game_status_t` enum, shown bellow, dependent on what status the game is in.

```c++
typedef enum game_status {
    GAME_RUNNING    = 0,
    GAME_WON    = 1,
    GAME_LOST   = 2
}   game_status_t;
```

#### Only Available on x86

```c++
void startMinesweeper();
```

Starts an instance of Minesweeper with the API injected.

---

```c++
bool isMinesweeperRunning();
```

Checks if Minesweeper is running with the API injected.

---

```c++
void dealocateMinesweeper();
```

Deallocates the Minesweeper process.
