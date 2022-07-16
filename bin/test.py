import minesweeper
from minesweeper import GameStatus
m = minesweeper.Minesweeper()

if not m.IsRunning():
    m.Start()
m.Connect()
while True:
    try:
        s = input()
        if m.GetGameStatus() != GameStatus.GAME_RUNNING:
            m.ResetField()
        t = s.split(",")
        x = int(t[0])
        y = int(t[1])
        m.ClickOnTile(x, y)
    except ValueError:
        if s.rstrip().lstrip() == "reset":
            m.ResetField()
    m.PrintField()
    status = m.GetGameStatus()
    if status == GameStatus.GAME_WON:
        print("You won!")
    elif status == GameStatus.GAME_LOST:
        print("You lost!")
print("Could not connect!")