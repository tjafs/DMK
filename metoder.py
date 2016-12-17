import pickle
import variabler

# Lager et objekt for å bakke alle verdiene inn på en god måte
class data(object):
    def __init__(self,x_data,y_data,data_avstand,dara_avstand_graf1,dara_avstand_graf2,data_P,data_I,data_D):
        self.x_data = x_data
        self.y_data = y_data
        self.data_avstand = data_avstand
        self.dara_avstand_graf1 = dara_avstand_graf1
        self.dara_avstand_graf2 = dara_avstand_graf2
        self.data_P = data_P
        self.data_I = data_I
        self.data_D = data_D


global num
num = 0

def lagring_av_tideligere_data():
    global num
    num += 1

    l1 = ['Logg_', str(num), '.pkl']
    s = ''.join(l1)                         # Bruker ''.join() for og lage en string uten komma

    with open(str(s), 'wb') as output:
        data1 = data( variabler.tid_graf, variabler.x_graf, variabler.avstand_vis, variabler.hold_avstand_graf1, variabler.hold_avstand_graf2,
                      variabler.P_graf_tegn, variabler.I_graf_tegn, variabler.D_graf_tegn)
        pickle.dump(data1, output, pickle.HIGHEST_PROTOCOL)
    del data1
    output.close()


    return

def henting_av_tidligere_data(num):

    l1 = ['logg_', str(num), '.pkl']
    s = ''.join(l1)                     # Bruker ''.join() for og lage en string uten komma

    try:
        with open(str(s), 'rb') as input: # åpner pickel filen
            data1 = pickle.load(input)    # Henter ut objektet der alt er lagret
            tid = data1.x_data            # pakker ut all dataen
            aks = data1.y_data
            avstand = data1.data_avstand
            avstand_graf1 = data1.dara_avstand_graf1
            avstand_graf2 = data1.dara_avstand_graf2
            P = data1.data_P
            I = data1.data_I
            D = data1.data_D

            variabler.data_tid = tid        # lagrer dataen i filen variabler
            variabler.data_aks = aks
            variabler.data_avstand = avstand
            variabler.data_avstand_graf1 = avstand_graf1
            variabler.data_avstand_graf2 = avstand_graf2
            variabler.data_P = P
            variabler.data_I = I
            variabler.data_D = D
            variabler.tidligere_data_plot = 1
            input.close()
            variabler.aks_ok = 1
            variabler.avstand_ok = 1
            variabler.P_ok = 1
            variabler.I_ok = 1
            variabler.D_ok = 1
    except EOFError:
        return 'Hente av data error'   # for å fange Error slik at programet ikke krasjer


    return
#---------------------------------------------------------------------------------

# For e deklarer alle variabler i starten av programet
def instalering_av_variabler():
    variabler.knapp_nummer = 1
    variabler.graf2 = 0
    variabler.stop1 = 0
    variabler.start = 0
    variabler.avstand_vis = 0
    variabler.stop = 0
    variabler.les = 0
    variabler.runde = 0
    variabler.antall_pakker = 0

    variabler.start_plot = 0
    variabler.avstand_vis2 = 0

    variabler.x_Mer_30 = []
    variabler.ok_verdier = 0
    variabler.start_30 = 0

    variabler.data_tid = 0
    variabler.data_aks = 0
    variabler.data_avstand = 0
    variabler.data_avstand_graf1 = 0
    variabler.data_avstand_graf2 = 0
    variabler.data_P = 0
    variabler.data_I = 0
    variabler.data_D = 0
    variabler.avslutt = 0

    variabler.aks_ok = 0
    variabler.avstand_ok = 0
    variabler.P_ok = 0
    variabler.I_ok = 0
    variabler.D_ok = 0

    variabler.tidligere_data_plot = 0
    return