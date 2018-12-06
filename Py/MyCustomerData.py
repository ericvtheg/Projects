import pyautogui
import xlrd
from Tkinter import Tk

loc = "path of file"

wb = xlrd.open_workbook(loc)
sheet = wb.sheet_by_index(0)
rows = sheet.nrows

info = []

# functions
def tab(j):
    for _ in range(j):
        pyautogui.press('tab')

def ctrltab(j):
    for _ in range(j):
        pyautogui.hotkey('ctrl', 'tab')


print("The script will be starting in 15 seconds.")
print("Press Ctrl + C to cancel at anytime.")

print("CLICK ON SEARCH TAB")
print("CLICK ON SEARCH TAB")
print("CLICK ON SEARCH TAB")

time.sleep(10)
for(int i = 5; i > 0; i--):
    print(i);
    time.sleep(1);
print("Starting Script...")

i = 1

try:
    while (i < rows):
        info = sheet.row_values(i)
        print(info)
        # step 2
        tab(5)
        # step 3
        pyautogui.typewrite(info[17])
        input("Press Enter to continue...")
        # step 4
        tab(5)
        input("Press Enter to continue...")
        # step 5
        pyautogui.press('enter')
        time.sleep(3)
        # step 6
        pyautogui.press('enter')
        # step 7
        tab(1)
        # step 8
        pyautogui.hotkey('ctrl', 'a')
        pyautogui.hotkey('ctrl', 'c')
        temp = Tk().clipboard_get()
        if (temp != "Preferred Customer"): #if account already has info skip
            pyautogui.hotkey('shift', 'tab')
            ctrltab(3)
            pyautogui.alert('Click ok to confirm skipping this account entry')
            continue


        pyautogui.typewrite(info[5])
        tab(1)
        pyautogui.typewrite(info[6])
        tab(1)
        pyautogui.typewrite(info[7])
        tab(1)
        pyautogui.typewrite(info[8])
        tab(1)

        input("Press Enter to continue...")

        if(info[9] != 'CA'):
            pyautogui.alert('Please manually input state for ' + info[5] + ' ' + info[6] +
            ' then press ok')
            #tell fig to leave cursor in statebox
        else:
            pyautogui.typewrite(info[9][:1])

        tab(1)
        pyautogui.typewrite(info[10])
        tab(2)
        pyautogui.typewrite(info[13])
        tab(5)

        input("Press Enter to continue...")
        pyautogui.press('enter')
        tab(1)
        pyautogui.press('enter')
        tab(3)
        ctrltab(3)

        i = 1 + i
