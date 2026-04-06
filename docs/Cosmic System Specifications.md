# Cosmic System Specifications

## Video Out

All video out is memory mapped to a 16k chunk of memory between 0x8000 and 0xC000

Depending on the video mode this is interpereted to the Screen in different ways

```
    ┌───────────────────┐         ┌──────────────────┐                ┌──────────────┐
    │   Video Memory    ├──Reads──┤    Video Chip    ├───Translates───┤  Video Out   │ 
    └───────────────────┘         └──────────────────┘                └──────────────┘
```

The Video system is **not** a gpu. The video chip does not receive instructions, nor does it build the video out. It simply takes the video memory and translates it to an array of pixels depending on what video mode is selected.


The last few bytes of video memory describe what mode the video chip is outputting to and {maybe)the size of the display.

## Video Modes

### Console Mode
In console mode each byte of memory correlates to one character in a the character set (Similar to PETSCII). Each character is an 8x8 character on a 75 column, 50 row display. Each character takes one byte in memory and is entered in sequentially. The first 75 bytes of memory is the first row, the next 75 is the second row and so on.



### Graphics Mode

idk how to do this yet. Maybe grids, maybe hardware sprite? idk
