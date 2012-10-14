UnHexDump
=========

This program takes the ascii output of HexDump on a Cisco 797x or 79x1
phone and converts it into a binary file.

Possibly also works with other HexDump implementations that output
in a format like this example:

000a100 6972 676e 6428 3e2d 616c 7473 754e 626d
000a110 7265 202c 2d63 643e 6169 656c 4e64 6d75
000a120 6562 2c72 7320 7a69 6f65 2866 2d64 6c3e

Where each line contains 8 groups of two bytes prefixed with 
the relative address of the line.

IMPORTANT: This program is set to fix endian issues with respect to
           translating from Big Endian on the MIPS processor to little
           endian on my Intel processor. Comment out the FIX_ENDIAN define
           below to disable this behaviour.

Also note: This program is quite hackish and does little in the way of sanity
           checking. It will probably fail on output that differs too far from
           the example above.

That said it has successfully converted hundreds of kilobytes of hex output
streamed from a PuTTY session to a Cisco IP phone.

Use it at your own peril >D