from subprocess import call
from threading import Thread
import Tkinter as tk
import time
import httplib

GRID_W = "500"
GRID_H = "500"

def onKeyPress(event):
    x = event.char

    if x == "d":
        canvas.move(a, 2, 0)
    elif x == "a":
        canvas.move(a, -2, 0)
    elif x == "w":
        canvas.move(a, 0, -2)
    elif x == "s":
        canvas.move(a, 0, 2)

def button_1():
   try:
      httpServ = httplib.HTTPConnection("192.168.10.4", 80)
      httpServ.connect()
      httpServ.request('GET', "/casino_mode=on")
      httpServ.close()
   except IOError:
	return


def gui():
   root = tk.Tk()
   root.title("test")
   root.geometry(GRID_W + "x" + GRID_H)
   canvas = tk.Canvas(root, width=int(GRID_W), height=int(GRID_H), borderwidth=0,  highlightthickness=0, bg="black")
   canvas.grid()
   b = tk.Button(canvas, text="Button1", command=button_1)
   b.pack()

   #root.bind_all('<Key>', onKeyPress)
   root.mainloop()




thread_1 = Thread(target = gui)
thread_1.start()
thread_2 = Thread(call(["./mixerh264", "0", "0", "0"]))
thread_2.start()



