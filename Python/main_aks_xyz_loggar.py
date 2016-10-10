__author__ = 'Morten Tengesdal og Kristian Thorsen'
# Dato:2016.08.05
# Oppdatert til Python 3 av K. Thorsen
# Med serieporttraad og med meldingskoe
# Skriptet loggar akselerasjonsdata i X-, Y- og Z-retning i 16-bitsformat og med
# tidsreferanse i 8-bitsformat.
# Meldingsformat STX+(T+samplenr.(2 ASCII HEX-siffer)+X+data(4 ASCII Hex-siffer)
#                                                     Y+data(4 ASCII Hex-siffer)
#                                                     Z+data(4 ASCII Hex-siffer))*N+ETX
# Raadata blir lagra til fil og skalerte data blir plotta.
# Skriptet blir koeyrt saman med prosjektet aks_xyz_loggar_stm32f3_disc paa
# kortet STM32F3Discovery

import threading
import queue
import time
import numpy as np
import serial
import matplotlib.pyplot as mpl



# --------------------------------------------------------------------------
# Metode for aa gi ut int-verdien til eit hexadesimalt teikn i ASCII-format
# --------------------------------------------------------------------------
def hexascii2int(hex_teikn):
    if '0' <= hex_teikn <= '9':
        return (int(ord(hex_teikn) - 48))  # ASCII-koden for '0' er 0x30 = 48
    elif 'A' <= hex_teikn <= 'F':
        return (int(ord(hex_teikn) - 55))  # ASCII-koden for 'A' er 0x41 = 65

#-------------------------------------------------------------------------
# Kode for ein traad som les serieporten konfigurert i hovudtraaden main.
# Lesinga startar naar traaden faar ein 'k'(koeyr) via ein kommandokoe og
# stansar naar traaden faar ein 's' og etterpaa les meldingshalen ETX.
# Alle mottatte teikn blir lagt inn i ei meldingsliste.
# Serieporten blir stengt til slutt..
#-------------------------------------------------------------------------
def seriekomm(serieport, kommando_koe, meldingar):  # Innhald i traaden
    try:
        ny_kommando = kommando_koe.get()  # Vil henga til han faar foerste kommandoen
    except Exception:
        pass  # Ignorer, men kvitter ut evt. unntak som oppstaar.

    tilstand = ny_kommando

    while tilstand == 'k':  # Saa lenge ein vil k(oeyra logging)

        #		while serieport.inWaiting() > 0:
        teikn = str(serieport.read(1), encoding='utf-8')  # Les eitt teikn.  #KT La til convert til str
                                                          # Vil blokkera/henga til det er kome noko aa lesa
        meldingar.append(teikn)

        try:
            ny_kommando = kommando_koe.get(block=False)  # Her skal ein ikkje henga/blokkera
        except Exception:  # men bare sjekka om det er kome ny kommando
            pass  # Her faar ein eit"Empty"-unntak kvar gong ein les ein tom koe. Dette skal
        # ignorerast, men kvitterast ut.

        if ny_kommando == 's':
            tilstand = ny_kommando  # Stans logging men fullfoer lesing t.o.m meldingshalen ETX

    while teikn != '\x03':  # Heilt til og med meldingshalen ETX
        #		while serieport.inWaiting() > 0:
        teikn = str(serieport.read(1), encoding='utf-8')  # Les eitt teikn. #KT La til convert til str
        meldingar.append(teikn)

    serieport.close()  # Steng ned
    print(serieport.name, 'er stengt')




#metode for å lage en x-datastreng for seg selv, som kan kalles opp som en tråd.
#
def lag_x_data(ut_data, inn_data):
    while(1):
        if(len(inn_data)>20):
            k = len(inn_data)
            i = k - 5
            if inn_data[i] == 'X':
                #print('fant X data')
                ut_data.append( 4096 * hexascii2int(inn_data[i + 1]) + 256 * hexascii2int(inn_data[i + 2]) + 16 * hexascii2int(inn_data[i + 3]) + hexascii2int(inn_data[i + 4]))
            #else: print ('finner ikke X data verdier')

        #else: print ('lengden til mcmeldingar er ikke over 5 enda')



#-------------------------------------------------------------------------------------
# Hovudtraad (main).
# Denne opnar loggefil, kommandokoe, serieport og startar serietraaden.
# Saa vil traaden venta paa koeyr-kommando fraa tastaturet. Han vil saa gi melding via
# ein brukarkommandokoe til serietraaden om aa starta logging og til mikrokontrolleren
# om aa starta maalingane og sending av filtrerte X-, Y-, Z-data med tidsreferanse.
# Saa vil han venta paa stoppkommando fraa tastaturet. Etter aa ha faatt denne vil han
# gi melding til serietraaden og saa mikrokontrolleren om aa stoppa. Serietraaden vil
# daa halda fram til han les meldingshalen ETX.

# Serietraaden vil skriva ut heile meldinga og vil saa laga raae (dvs. uskalerte) tids-
# og akselerasjonslister som blir skrivne ut.
# Saa blir det laga skalerte lister samt lister for absolutt akselerasjon samt stamp-
# og rullvinkel. Alt dette blir saa plotta til slutt.
#-------------------------------------------------------------------------------------
def main():
    kommando = '0'
    fileName = 'logg.txt'
    f = open(fileName, 'r+')
    x_data = []
    uC_meldingar = []
    brukarkommandoar = queue.Queue()

    connected = True
    port = 'COM4'
    baud = 115200  # 9600

    serieport = serial.Serial(port, baud, timeout=1)

    if serieport.isOpen():
        print(serieport.name, 'er open')
    else:
        serieport.open()

    serie_traad = threading.Thread(target=seriekomm, args=(serieport, brukarkommandoar, uC_meldingar))
    serie_traad.start()

    lag_x_data_traad = threading.Thread(target=lag_x_data, args=(x_data, uC_meldingar))
    lag_x_data_traad.start()



    print('Loggaren er klar')

    while kommando != 'k':
        kommando = input('Gi kommando(k-koeyr logging, s-stopp logging):\n')  # Loepande lesing, dvs. vil staa her
    # til det kjem noko inn fraa tastaturet.

    print('Startar logging')
    brukarkommandoar.put(kommando)  # Gi melding til serietraaden om aa starta sjekking av port
    serieport.write('k'.encode('utf-8'))  # Gi melding til uC-en om aa koeyra i gong # KT la til encoding

    while kommando != 's':
        kommando = input('Gi kommando:\n')  # Loepande lesing, dvs. staar her

    brukarkommandoar.put(kommando)  # Gi melding til serietraaden om aa stoppa, men fullfoera logging tom. ETX
    time.sleep(1)  # Sikra at traaden faar med seg slutten paa meldinga
    serieport.write('s'.encode('utf-8'))  # Gi melding til uC-en om aa stoppa sending av nye data #KT La til encoding
    print('Stoppar logging')

    # serieport.close()     # Det er naa kome kommando om aa stoppa logginga
    # print '%s %s'  %(serieport.name, 'er stengt')

    print(uC_meldingar)

    f.write(str(uC_meldingar))
    f.close()

 # Lag lister av raadata.
    tid_raa = []
    a_x_raa = []
    a_y_raa = []
    a_z_raa = []

    for i in range(0, len(uC_meldingar)):
        if uC_meldingar[i] == 'T':
            tid_raa.append(16 * hexascii2int(uC_meldingar[i + 1]) + hexascii2int(uC_meldingar[i + 2]))

        elif uC_meldingar[i] == 'X':
            #Fiks slik at ein faar fram negative tal.
            a_x_raa.append(
                4096 * hexascii2int(uC_meldingar[i + 1]) + 256 * hexascii2int(uC_meldingar[i + 2]) + 16 * hexascii2int(
                    uC_meldingar[i + 3]) + hexascii2int(uC_meldingar[i + 4]))
        elif uC_meldingar[i] == 'Y':
            #Fiks slik at ein faar fram negative tal.
            a_y_raa.append(
                4096 * hexascii2int(uC_meldingar[i + 1]) + 256 * hexascii2int(uC_meldingar[i + 2]) + 16 * hexascii2int(
                    uC_meldingar[i + 3]) + hexascii2int(uC_meldingar[i + 4]))
        elif uC_meldingar[i] == 'Z':
            #Fiks slik at ein faar fram negative tal.
            a_z_raa.append(
                4096 * hexascii2int(uC_meldingar[i + 1]) + 256 * hexascii2int(uC_meldingar[i + 2]) + 16 * hexascii2int(
                    uC_meldingar[i + 3]) + hexascii2int(uC_meldingar[i + 4]))



 # Lag skalerte lister og rekna ut tilleggsvariablar.
    a_x = []
    a_y = []
    a_z = []
    aks_abs = []  # sqrt(ax**2 + ay**2 + az**2)
    ayz_abs = []
    rull = []     # rullvinkel psi i grader (om x-aksen)
    stamp = []    # stampvinkel theta i grader (om y-aksen)

    for i in range(0, len(a_x_raa)):
        if a_x_raa[i] >= 32768:
            a_x.append((float(a_x_raa[i])-65536.0)/1000.0) # 1mg pr. LSb iflg. databladet.
        else:
            a_x.append(float(a_x_raa[i]/1000.0))

    for i in range(0, len(a_y_raa)):
        if a_y_raa[i] >= 32768:
            a_y.append((float(a_y_raa[i])-65536.0)/1000.0)
        else:
            a_y.append(float(a_y_raa[i]/1000.0))

    for i in range(0, len(a_z_raa)):
        if a_z_raa[i] >= 32768:
            a_z.append((float(a_z_raa[i])-65536.0)/1000.0)
        else:
            a_z.append(float(a_z_raa[i])/1000.0)

    for i in range(0, len(a_z)):
        aks_abs.append(np.sqrt(a_x[i]**2 + a_y[i]**2 + a_z[i]**2))
    for i in range(0, len(a_z_raa)):
        ayz_abs.append(np.sqrt(a_y[i]**2 + a_z[i]**2))

    for i in range(0, len(a_x)):
        stamp.append(np.arctan2(a_x[i], ayz_abs[i]) * 180 / np.pi)

    for i in range(0, len(a_x)):
        if a_z[i] == 0:
            if i == 0:
                rull.append(0)
            else:
                rull.append(rull[i-1])     #Unngaa deling paa null

        else:
            rull.append(np.arctan2(a_y[i], a_z[i]) * 180 / np.pi)

  # Skal laga ei kontinuerleg aukande tidsliste som startar i null.
    tid = []
    Ts = 0.1  # Sampleintervall i sekund
    tidsomloepnr = 0

    for j in range(0, len(tid_raa)):
        tid.append(tid_raa[j] + tidsomloepnr * 256)

        if tid_raa[j] == 255: # Tidsreferansen er paa 8 bit og rullar rundt kvar 256. gong
            tidsomloepnr = tidsomloepnr + 1

    skyv = tid[0] # Vil at tidslista skal starta paa null.
    print(skyv)
    for k in range(0, len(tid)):
        tid[k] = Ts * (tid[k] - skyv)

 # Skriv ut listene for kontroll
 #   print(tid_raa)
 #   print(tid)
 #   print(len(tid))
    print('X RÅDATA:')
    print(a_x_raa)
    print(len(a_x_raa))
 #   print(a_y_raa)
 #   print(len(a_y_raa))
 #   print(a_z_raa)
 #   print(len(a_z_raa))
 #   print(stamp)
 #   print(len(a_x))
 #   print(rull)
 #   print(len(a_x))
    print('X DATA:')
    print(x_data)
    print(len(x_data))



 # Seks subplott med felles tidsakse.
    f, aks_sub = mpl.subplots(6, sharex=True)
    aks_sub[0].plot(tid, a_x)
    aks_sub[1].plot(tid, a_y)
    aks_sub[2].plot(tid, a_z)
    aks_sub[3].plot(tid, aks_abs)
    aks_sub[4].plot(tid, stamp)
    aks_sub[5].plot(tid, rull)
    aks_sub[5].set_xlabel('Tid [sek]')
    aks_sub[1].set_ylabel('Akselerasjon [g]')
    aks_sub[4].set_ylabel('Vinkel [grader]')
    aks_sub[0].set_title('a_x')
    aks_sub[1].set_title('a_y')
    aks_sub[2].set_title('a_z')
    aks_sub[3].set_title('aks_abs - absolutt akselerasjon')
    aks_sub[4].set_title('Stampvinkel (om Y-aksen)')
    aks_sub[5].set_title('Rullvinkel (om X-aksen)')
    aks_sub[0].grid()
    aks_sub[1].grid()
    aks_sub[2].grid()
    aks_sub[3].grid()
    aks_sub[4].grid()
    aks_sub[5].grid()

    mpl.show()

    print('Slutt i main')

if __name__ == "__main__":
    main()
