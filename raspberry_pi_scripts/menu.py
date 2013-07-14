#!/usr/bin/python

import os
import json

class Menu(object):
    def __init__(self, dir='./menu'):
        self.base_dir = dir
        self.menu_base = os.path.join(os.path.abspath(dir), 'menu.menuconfig')
        self.populate_menu()
    
    
    def populate_menu_level(self, menu):
        menu_start = []
        print 'reading ', menu
        with open(os.path.join(os.path.abspath(self.base_dir), menu), 'r') as f:
            menu_start = json.loads(f.read())

            for idx,contents in enumerate(menu_start):
                if "subtree" in contents['action']['type']:
                    contents['action']['subtree'] = self.populate_menu_level('{f}.menuconfig'.format(f=contents['action']['arg']))
                    menu_start[idx] = contents
        return menu_start

    def populate_menu(self):
        self.master_menu = self.populate_menu_level(self.menu_base)
