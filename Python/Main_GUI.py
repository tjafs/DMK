from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import pickle
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import threading
import Logging
from Logging import Company
import Metoder
import tkinter
from tkinter import *
from tkinter import ttk


kor = 0

f = Figure(figsize=(5, 5), dpi=100)
a = f.add_subplot(111)



# opptatering av graf -------------------------------
def animate(i):

    if kor == 1:
        try:
            with open('company_data.pkl', 'rb') as input:
                company1 = pickle.load(input)
                x = company1.x_data
                y = company1.y_data
            a.clear()
            a.plot(x, y)
        except EOFError:
            return 'jalla'


#  ------------------------------------------------------------------

def start_knapp():
    global kor
    global yo
    if kor == 0:
        with open('company_data2.pkl', 'wb') as output:
            company2 = Company(0, 0)
            pickle.dump(company2, output, pickle.HIGHEST_PROTOCOL)
        del company2

        yo = threading.Thread(target=Logging.start_lesing)
        yo.start()
        time.sleep(0.5)
        kor = 1


def stop_knapp():
    global kor
    kor = 0
    global yo

    with open('company_data2.pkl', 'wb') as output2:
        company2 = Company(1, 1)
        pickle.dump(company2, output2, pickle.HIGHEST_PROTOCOL)
    del company2


    lagring_fil = threading.Thread(target=Metoder.lagring_data)
    lagring_fil.start()



root = Tk()
frame = Frame(root)
frame.pack()

# Navn på vindu
root.title('Gruppe 2')
    # Størrelse på vindu
    # root.geometry('300x300')
    # Bakgruns farge
root.configure(background='#d3d3d3')

# bilde
bilde = tkinter.PhotoImage(file='b.gif')
sett_bilde = tkinter.Label(root, image=bilde)
sett_bilde.pack()

knapp_frame = Frame(root)
knapp_frame.pack(side=LEFT)

graf_frame = Frame(root)
graf_frame.pack(side=LEFT)

frame2 = Frame(root)
frame2.pack()


start = tkinter.Button(knapp_frame, text='start', command=start_knapp, height=2, width=6, font=('Helvetica', 16), bd=3)
start.pack()

stop = tkinter.Button(knapp_frame, text='stop', command=stop_knapp, height=2, width=6, font=('Helvetica', 16), bd=3)
stop.pack()

avslutt = tkinter.Button(frame2, text='Avslutt', command=stop_knapp, height=2, width=6, font=('Helvetica', 16), bd=3)
avslutt.pack()

figur = FigureCanvasTkAgg(f, knapp_frame)
figur.show()
figur.get_tk_widget().pack(side=tkinter.TOP, fill=tkinter.BOTH, expand=True)



ani = animation.FuncAnimation(f, animate, interval=500)
root.mainloop()