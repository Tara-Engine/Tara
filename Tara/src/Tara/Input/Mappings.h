#pragma once

/*
Input Mappings file
This creates all the mappings from various code-names of Tara input to the number
representing the input value in the underlying system

These numbers are intentionally the EXACT SAME and a similar name to GLFW, to make everything
easier for both me as engine programmer, as well as making the engine faster
*/

/*
Joystick Hat Positions (ie, where a joystick is moved to
*/
#define TARA_JOYSTICK_HAT_CENTERED           0
#define TARA_JOYSTICK_HAT_UP                 1
#define TARA_JOYSTICK_HAT_RIGHT              2
#define TARA_JOYSTICK_HAT_DOWN               4
#define TARA_JOYSTICK_HAT_LEFT               8
#define TARA_JOYSTICK_HAT_RIGHT_UP           (GLFW_HAT_RIGHT | GLFW_HAT_UP)
#define TARA_JOYSTICK_HAT_RIGHT_DOWN         (GLFW_HAT_RIGHT | GLFW_HAT_DOWN)
#define TARA_JOYSTICK_HAT_LEFT_UP            (GLFW_HAT_LEFT  | GLFW_HAT_UP)
#define TARA_JOYSTICK_HAT_LEFT_DOWN          (GLFW_HAT_LEFT  | GLFW_HAT_DOWN)

/*Key codes*/
/* The unknown key */
#define TARA_KEY_UNKNOWN            -1

/* 
Printable keys 
Many of these match their ASCII characters. To make things easier.
*/
#define TARA_KEY_SPACE              32
#define TARA_KEY_APOSTROPHE         39  /* ' */
#define TARA_KEY_COMMA              44  /* , */
#define TARA_KEY_MINUS              45  /* - */
#define TARA_KEY_PERIOD             46  /* . */
#define TARA_KEY_SLASH              47  /* / */
#define TARA_KEY_0                  48
#define TARA_KEY_1                  49
#define TARA_KEY_2                  50
#define TARA_KEY_3                  51
#define TARA_KEY_4                  52
#define TARA_KEY_5                  53
#define TARA_KEY_6                  54
#define TARA_KEY_7                  55
#define TARA_KEY_8                  56
#define TARA_KEY_9                  57
#define TARA_KEY_SEMICOLON          59  /* ; */
#define TARA_KEY_EQUAL              61  /* = */
#define TARA_KEY_A                  65
#define TARA_KEY_B                  66
#define TARA_KEY_C                  67
#define TARA_KEY_D                  68
#define TARA_KEY_E                  69
#define TARA_KEY_F                  70
#define TARA_KEY_G                  71
#define TARA_KEY_H                  72
#define TARA_KEY_I                  73
#define TARA_KEY_J                  74
#define TARA_KEY_K                  75
#define TARA_KEY_L                  76
#define TARA_KEY_M                  77
#define TARA_KEY_N                  78
#define TARA_KEY_O                  79
#define TARA_KEY_P                  80
#define TARA_KEY_Q                  81
#define TARA_KEY_R                  82
#define TARA_KEY_S                  83
#define TARA_KEY_T                  84
#define TARA_KEY_U                  85
#define TARA_KEY_V                  86
#define TARA_KEY_W                  87
#define TARA_KEY_X                  88
#define TARA_KEY_Y                  89
#define TARA_KEY_Z                  90
#define TARA_KEY_LEFT_BRACKET       91  /* [ */
#define TARA_KEY_BACKSLASH          92  /* \ */
#define TARA_KEY_RIGHT_BRACKET      93  /* ] */
#define TARA_KEY_GRAVE_ACCENT       96  /* ` */
#define TARA_KEY_WORLD_1            161 /* non-US #1 */
#define TARA_KEY_WORLD_2            162 /* non-US #2 */

/* 
Function keys 
These are ALL larger than a char, and thus clearly not ASCII
*/
#define TARA_KEY_ESCAPE             256
#define TARA_KEY_ENTER              257
#define TARA_KEY_TAB                258
#define TARA_KEY_BACKSPACE          259
#define TARA_KEY_INSERT             260
#define TARA_KEY_DELETE             261
#define TARA_KEY_RIGHT              262
#define TARA_KEY_LEFT               263
#define TARA_KEY_DOWN               264
#define TARA_KEY_UP                 265
#define TARA_KEY_PAGE_UP            266
#define TARA_KEY_PAGE_DOWN          267
#define TARA_KEY_HOME               268
#define TARA_KEY_END                269
#define TARA_KEY_CAPS_LOCK          280
#define TARA_KEY_SCROLL_LOCK        281
#define TARA_KEY_NUM_LOCK           282
#define TARA_KEY_PRINT_SCREEN       283
#define TARA_KEY_PAUSE              284
#define TARA_KEY_F1                 290
#define TARA_KEY_F2                 291
#define TARA_KEY_F3                 292
#define TARA_KEY_F4                 293
#define TARA_KEY_F5                 294
#define TARA_KEY_F6                 295
#define TARA_KEY_F7                 296
#define TARA_KEY_F8                 297
#define TARA_KEY_F9                 298
#define TARA_KEY_F10                299
#define TARA_KEY_F11                300
#define TARA_KEY_F12                301
#define TARA_KEY_F13                302
#define TARA_KEY_F14                303
#define TARA_KEY_F15                304
#define TARA_KEY_F16                305
#define TARA_KEY_F17                306
#define TARA_KEY_F18                307
#define TARA_KEY_F19                308
#define TARA_KEY_F20                309
#define TARA_KEY_F21                310
#define TARA_KEY_F22                311
#define TARA_KEY_F23                312
#define TARA_KEY_F24                313
#define TARA_KEY_F25                314
#define TARA_KEY_KP_0               320
#define TARA_KEY_KP_1               321
#define TARA_KEY_KP_2               322
#define TARA_KEY_KP_3               323
#define TARA_KEY_KP_4               324
#define TARA_KEY_KP_5               325
#define TARA_KEY_KP_6               326
#define TARA_KEY_KP_7               327
#define TARA_KEY_KP_8               328
#define TARA_KEY_KP_9               329
#define TARA_KEY_KP_DECIMAL         330
#define TARA_KEY_KP_DIVIDE          331
#define TARA_KEY_KP_MULTIPLY        332
#define TARA_KEY_KP_SUBTRACT        333
#define TARA_KEY_KP_ADD             334
#define TARA_KEY_KP_ENTER           335
#define TARA_KEY_KP_EQUAL           336
#define TARA_KEY_LEFT_SHIFT         340
#define TARA_KEY_LEFT_CONTROL       341
#define TARA_KEY_LEFT_ALT           342
#define TARA_KEY_LEFT_SUPER         343
#define TARA_KEY_RIGHT_SHIFT        344
#define TARA_KEY_RIGHT_CONTROL      345
#define TARA_KEY_RIGHT_ALT          346
#define TARA_KEY_RIGHT_SUPER        347
#define TARA_KEY_MENU               348

#define TARA_KEY_LAST               TARA_KEY_MENU


/*
Modifier key bits
used in modifer checks. Each one is a bit
*/
#define TARA_MOD_SHIFT           0x0001
#define TARA_MOD_CONTROL         0x0002
#define TARA_MOD_ALT             0x0004
#define TARA_MOD_SUPER           0x0008
#define TARA_MOD_CAPS_LOCK       0x0010
#define TARA_MOD_NUM_LOCK        0x0020

/*
Mouse Button Codes
*/
#define TARA_MOUSE_BUTTON_1         0
#define TARA_MOUSE_BUTTON_2         1
#define TARA_MOUSE_BUTTON_3         2
#define TARA_MOUSE_BUTTON_4         3
#define TARA_MOUSE_BUTTON_5         4
#define TARA_MOUSE_BUTTON_6         5
#define TARA_MOUSE_BUTTON_7         6
#define TARA_MOUSE_BUTTON_8         7
#define TARA_MOUSE_BUTTON_LAST      TARA_MOUSE_BUTTON_8
#define TARA_MOUSE_BUTTON_LEFT      TARA_MOUSE_BUTTON_1
#define TARA_MOUSE_BUTTON_RIGHT     TARA_MOUSE_BUTTON_2
#define TARA_MOUSE_BUTTON_MIDDLE    TARA_MOUSE_BUTTON_3

/*
Joystick Mapping codes
*/
#define TARA_JOYSTICK_1             0
#define TARA_JOYSTICK_2             1
#define TARA_JOYSTICK_3             2
#define TARA_JOYSTICK_4             3
#define TARA_JOYSTICK_5             4
#define TARA_JOYSTICK_6             5
#define TARA_JOYSTICK_7             6
#define TARA_JOYSTICK_8             7
#define TARA_JOYSTICK_9             8
#define TARA_JOYSTICK_10            9
#define TARA_JOYSTICK_11            10
#define TARA_JOYSTICK_12            11
#define TARA_JOYSTICK_13            12
#define TARA_JOYSTICK_14            13
#define TARA_JOYSTICK_15            14
#define TARA_JOYSTICK_16            15
#define TARA_JOYSTICK_LAST          TARA_JOYSTICK_16

/*
Gamepad mappings
*/
#define TARA_GAMEPAD_BUTTON_A               0
#define TARA_GAMEPAD_BUTTON_B               1
#define TARA_GAMEPAD_BUTTON_X               2
#define TARA_GAMEPAD_BUTTON_Y               3
#define TARA_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define TARA_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define TARA_GAMEPAD_BUTTON_BACK            6
#define TARA_GAMEPAD_BUTTON_START           7
#define TARA_GAMEPAD_BUTTON_GUIDE           8
#define TARA_GAMEPAD_BUTTON_LEFT_THUMB      9
#define TARA_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define TARA_GAMEPAD_BUTTON_DPAD_UP         11
#define TARA_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define TARA_GAMEPAD_BUTTON_DPAD_DOWN       13
#define TARA_GAMEPAD_BUTTON_DPAD_LEFT       14
#define TARA_GAMEPAD_BUTTON_LAST            TARA_GAMEPAD_BUTTON_DPAD_LEFT

#define TARA_GAMEPAD_BUTTON_CROSS       TARA_GAMEPAD_BUTTON_A
#define TARA_GAMEPAD_BUTTON_CIRCLE      TARA_GAMEPAD_BUTTON_B
#define TARA_GAMEPAD_BUTTON_SQUARE      TARA_GAMEPAD_BUTTON_X
#define TARA_GAMEPAD_BUTTON_TRIANGLE    TARA_GAMEPAD_BUTTON_Y

/*
Gamepad axises.
*/
#define TARA_GAMEPAD_AXIS_LEFT_X        0
#define TARA_GAMEPAD_AXIS_LEFT_Y        1
#define TARA_GAMEPAD_AXIS_RIGHT_X       2
#define TARA_GAMEPAD_AXIS_RIGHT_Y       3
#define TARA_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define TARA_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define TARA_GAMEPAD_AXIS_LAST          TARA_GAMEPAD_AXIS_RIGHT_TRIGGER