import threading
import queue
import time
import numpy as np
import serial
import matplotlib.pyplot as mpl

#metode for å lage en x-datastreng for seg selv, som kan kalles opp som en tråd.
#
def lag_x_data(ut_data, inn_data):
    while(1):
        #time.sleep(0.1)
        i = len(inn_data)
        #print('Antall elementer avlest: ')
        #print(i)

        if(i>50):
            if inn_data[i-1] == 'Y':
                print('fant X data')
                #print(inn_data[i])
                print(inn_data[i-1])
                print(inn_data[i-2])
                print(inn_data[i-3])
                print(inn_data[i-4])
                print(inn_data[i - 5])
                print(inn_data[i - 6])
                #original fra x raa data: ut_data.append( 4096 * hexascii2int(inn_data[i + 1]) + 256 * hexascii2int(inn_data[i + 2]) + 16 * hexascii2int(inn_data[i + 3]) + hexascii2int(inn_data[i + 4]))
                ut_data.append( 4096 * hexascii2int(inn_data[i - 5]) + 256 * hexascii2int(inn_data[i - 4]) + 16 * hexascii2int(inn_data[i - 3]) + hexascii2int(inn_data[i - 2]))
                print(ut_data)
                exit()
           # else: print ('finner ikke X data verdier')

        #else: print ('lengden til mcmeldingar er ikke over 5 enda')


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



# --------------------------------------------------------------------------
# Metode for aa gi ut int-verdien til eit hexadesimalt teikn i ASCII-format
# --------------------------------------------------------------------------
def hexascii2int(hex_teikn):
    if '0' <= hex_teikn <= '9':
        return (int(ord(hex_teikn) - 48))  # ASCII-koden for '0' er 0x30 = 48
    elif 'A' <= hex_teikn <= 'F':
        return (int(ord(hex_teikn) - 55))  # ASCII-koden for 'A' er 0x41 = 65

