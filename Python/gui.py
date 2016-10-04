from tkinter import *
import threading
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

#variabeldeklerasjoner

#Funksjonsdeklarasjoner
startlogg = lambda: print("Logging startet")
stopplogg = lambda: print("Logging stoppet")
graflogg = lambda: print("Viser graf av logg fil")


#Graftråd som hele tiden genererer grafen i bakgrunnen, grafen vises når plt.show() startes av GUI knapp
def rtg(frameno):
    x = mu + sigma * np.random.randn(10000)
    n, _ = np.histogram(x, bins, normed=True)
    for rect, h in zip(patches, n):
        rect.set_height(h)

#UNDER HER ER DATA SOM SKAL ERSTATTES MED FAKTISK DATA FRA PYSERIAL
#Her genreres nå tilfeldig data for hver gang main kjører.
#frameno=0 trengs tydeligvis når funksjonen brukes i en tråd.
mu, sigma = 100, 15
fig, ax = plt.subplots()
x = mu + sigma * np.random.randn(10000)
n, bins, patches = plt.hist(x, 50, normed=1, facecolor='blue', alpha=0.75)
ani = animation.FuncAnimation(fig, rtg, blit=False, interval=10,repeat=True)
frameno=0


rtg_traad = threading.Thread(target=rtg(frameno), args=(0))
rtg_traad.start()

#Serieporttråd som bruker flagg til å gå mellom start og stopp logging



#lager vinduet
root = Tk()

#endrer vinduet
root.title("Vindustittel")
root.geometry("100x100")

app = Frame(root)
app.grid()

#Setter opp knapper
startknapp=Button(app, text= "Start logging", command = startlogg)
startknapp.grid()
stoppknapp=Button(app, text= "stop logging", command = stopplogg)
stoppknapp.grid()
grafknapp=Button(app, text= "Graf av logg",  command = plt.show )
grafknapp.grid()


#"kick of the event loop"
root.mainloop()
