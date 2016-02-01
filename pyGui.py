#!/usr/bin/python

def quit(root):
	root.destroy()

import Tkinter as tk
import tkMessageBox

btk = tk.Tk()
c_dir=0

def dir_change(x):
	global c_dir
	c_dir = x
	print x

#---WIDGET CODE---
b_frame = tk.Frame(btk)
b_frame.pack(fill=tk.X, side=tk.BOTTOM)

forward = tk.Button ( b_frame, text = "Forward", command=lambda *args: dir_change(1))
reverse = tk.Button ( b_frame, text = "Reverse", command=lambda *args: dir_change(4))
left = tk.Button ( b_frame, text = "Left", command=lambda *args: dir_change(2))
right = tk.Button ( b_frame, text = "Right", command=lambda *args: dir_change(3))

b_frame.columnconfigure(0, weight=1)
b_frame.columnconfigure(1, weight=1)

forward.grid(row=0, column=1)
reverse.grid(row=2, column=1)
left.grid(row=1, column=0)
right.grid(row=1, column=2)


btk.mainloop()
