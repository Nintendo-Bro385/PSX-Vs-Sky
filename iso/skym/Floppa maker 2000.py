import shutil
import subprocess
import os
import random

number = 0

def VramImagesandPalettePositions():

    VramX=str(input("Enter the X position for the image in vram: "))
    VramY=str(input("Enter the Y position for the image in vram: "))
    PaletteX=str(input("Enter the X position for the image palette in vram: "))
    PaletteY=str(input("Enter the Y Position for the image palette in vram: "))
    ImageColors=str(input("Enter image colors 4bpp or 8bpp: "))
    
    return VramX, VramY, PaletteX, PaletteY, ImageColors

def CreateSheet(VramX, VramY, PaletteX, PaletteY, ImageColors):

    VramSheet=open("Vramsheet.txt","w+")
    VramSheet.write(VramX)
    VramSheet=open("Vramsheet.txt","a+")
    VramSheet.write(" ")
    VramSheet.write(VramY)
    VramSheet.write(" ")
    VramSheet.write(PaletteX)
    VramSheet.write(" ")
    VramSheet.write(PaletteY)
    VramSheet.write(" ")
    VramSheet.write(ImageColors)
    
    VramSheet.close()

def VramSheetsRequired():
    
    Idle=int(input("how many idle sprites?: "))
    Left=int(input("how many left sprites?: "))
    Down=int(input("how many down sprites?: "))
    Up=int(input("how many up sprites?: "))
    Right=int(input("how many right sprites?: "))

    return Idle,Left,Down,Up,Right

def DuplicateVramSheets(number, Idle,Left,Down,Up,Right):

    for counter in range(Idle):
        new_file_name = "idle" + str(number) + ".png.txt"
        original = r'Vramsheet.txt'
        shutil.copy(original, new_file_name)

        subprocess.call((new_file_name), shell=True)
        print("Creating idle",number,".png.txt")
        number=number+1
    number=0  
    for counter in range(Left):
        new_file_name = "left" + str(number) + ".png.txt"
        original = r'Vramsheet.txt'
        shutil.copy(original, new_file_name)

        subprocess.call((new_file_name), shell=True)
        print("Creating left",number,".png.txt")
        number=number+1
    number=0    
    for counter in range(Down):
        new_file_name = "down" + str(number) + ".png.txt"
        original = r'Vramsheet.txt'
        shutil.copy(original, new_file_name)

        subprocess.call((new_file_name), shell=True)
        print("Creating down",number,".png.txt")
        number=number+1
    number=0    
    for counter in range(Up):
        new_file_name = "up" + str(number) + ".png.txt"
        original = r'Vramsheet.txt'
        shutil.copy(original, new_file_name)

        subprocess.call((new_file_name), shell=True)
        print("Creating up",number,".png.txt")
        number=number+1
    number=0
    for counter in range(Right):
        new_file_name = "right" + str(number) + ".png.txt"
        original = r'Vramsheet.txt'
        shutil.copy(original, new_file_name)

        subprocess.call((new_file_name), shell=True)
        print("Creating right",number,".png.txt")
        number=number+1

#Ask for Vram Positions
VramX, VramY, PaletteX, PaletteY, ImageColors = VramImagesandPalettePositions()
#Create Vram Positions sheet
CreateSheet(VramX, VramY, PaletteX, PaletteY, ImageColors)
#Ask how many sheets of each sprite animation
Idle,Left,Down,Up,Right = VramSheetsRequired()
#Create the Vram sheets that are required
DuplicateVramSheets(number,Idle,Left,Down,Up,Right)
