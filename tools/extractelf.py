#!/usr/bin/env python3

"""Extracts PT_LOAD regions to individual files"""

import sys

from elftools.elf.elffile import ELFFile

with open(sys.argv[1], "rb") as elf_file:
    prefix = sys.argv[2]

    file = ELFFile(elf_file)

    num = file.num_segments()
    
    for i in range(num):

        seg = file.get_segment(i)

        if ('PT_LOAD' == seg.__getitem__('p_type')):

            paddr = seg.__getitem__('p_paddr')

            file_name = '%s_0x%08x.bin' % (prefix, paddr)

            with open(file_name, "wb") as atf:
                atf.write(seg.data());
