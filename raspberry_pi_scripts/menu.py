#!/usr/bin/python

import os
import json
import serial
from menuscreens import MenuScreens


class MenuController(object):
    def __init__(self):
        self.ser = serial.Serial('/dev/ttyACM0', 9600, timeout=0)
        self.menu = Menu()


    def loop(self):
        res = self.ser.readline()
        if res != '':
            self.menu.perform_action(res)

        self.menu.display()

class Menu(object):
    def __init__(self, dir='./menu'):
        self.base_dir = dir
        self.menu_base = os.path.join(os.path.abspath(dir), 'menu.menuconfig')
        self.populate_menu()

    def increment_and_return_index(self):
        old_index = self.item_index
        self.item_index = self.item_index + 1
        return old_index

    def populate_menu_level(self, menu):
        menu_start = []
        print 'reading ', menu
        with open(os.path.join(os.path.abspath(self.base_dir), menu), 'r') as f:
            menu_start = json.loads(f.read())

            for idx,contents in enumerate(menu_start):
                contents['index'] = self.increment_and_return_index()
                if "subtree" in contents['action']['type']:
                    contents['action']['subtree'] = self.populate_menu_level('{f}.menuconfig'.format(f=contents['action']['arg']))
                    menu_start[idx] = contents
        return menu_start

    def populate_menu(self):
        self.item_index = 1
        self.master_menu = self.populate_menu_level(self.menu_base)

    def perform_action(self, buttonvalue):
        if buttonvalue == '1':
            pass
            
            
 
    def display(self):
        elem = self.find_current() 
        if elem.get('display_proc', False):
            eval('MenuScreens.{method}(self.ser)'.format(method=elem.get('display_proc')))
        else:
            MenuScreens.toScreen(elem['display'], self.ser)
