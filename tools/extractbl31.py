#!/usr/bin/env python3
#
# Script to extract PT_LOAD segments from bl31.elf. Derived from
# Rockchip's make_fit_atf.py

import os
import sys

# pip3 install pyelftools / apt install python3-pyelftools
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection
from elftools.elf.segments import Segment, InterpSegment, NoteSegment

with open(sys.argv[1], "rb") as bl31_file:
    bl31 = ELFFile(bl31_file)

    num = bl31.num_segments()
    for i in range(num):
        seg = bl31.get_segment(i)
        if ('PT_LOAD' == seg.__getitem__('p_type')):
            paddr = seg.__getitem__('p_paddr')
            file_name = 'bl31_0x%08x.bin' % paddr
            with open(file_name, "wb") as atf:
                atf.write(seg.data());
