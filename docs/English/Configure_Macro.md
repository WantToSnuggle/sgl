## Configuration Macro Description
SGL is a highly configurable GUI library that can be trimmed to reduce Flash and RAM usage through configuration macros. The following describes the configuration macros of `SGL`.

### CONFIG_SGL_PANEL_PIXEL_DEPTH
This macro is used to configure the number of bits for colors. The default is 32 bits, i.e., `CONFIG_SGL_PANEL_PIXEL_DEPTH=32`. If using 8-bit colors, please set this macro to 8, i.e., `CONFIG_SGL_PANEL_PIXEL_DEPTH=8`. For general screens with controllers, the color depth is 16 bits.

### CONFIG_SGL_EVENT_QUEUE_SIZE
This macro is used to configure the depth size of the event queue. The default is 16, i.e., `CONFIG_SGL_EVENT_QUEUE_SIZE=16`. If the queue size is insufficient, please set this macro to a larger value, such as `CONFIG_SGL_EVENT_QUEUE_SIZE=32` or any value you desire. For slower processors, please set this macro to a larger value, such as `CONFIG_SGL_EVENT_QUEUE_SIZE=64` or any value you desire.

### CONFIG_SGL_OBJ_NUM_MAX
This macro is used to configure the number of objects. The default is 64, i.e., `CONFIG_SGL_OBJ_NUM_MAX=64`. If the number of objects is insufficient, please set this macro to a larger value, such as `CONFIG_SGL_OBJ_NUM_MAX=128` or any value you desire.

### CONFIG_SGL_COLOR16_SWAP
This macro is used to set whether 16-bit colors should be swapped. The default is 0, i.e., `CONFIG_SGL_COLOR16_SWAP=0`. If 16-bit colors need to be swapped, please set this macro to 1, i.e., `CONFIG_SGL_COLOR16_SWAP=1`.

### CONFIG_SGL_USE_STYLE_UNIFIED_API
This macro is used to set whether to use a unified style interface, i.e., whether to use the `sgl_obj_set_style` function. If you want to minimize the size, please disable this macro and use the `sgl_xxx_set_style` function instead, as shown below:
1. When [CONFIG_SGL_USE_STYLE_UNIFIED_API](file://c:\Users\lsw\Desktop\sgl\source\sgl_config.h#L17-L18) is enabled, please use the following function to set styles:
```c
...
sgl_obj_t *obj = sgl_button_create(NULL);
...
sgl_obj_set_set_style(obj, SGL_STYLE_COLOR, SGL_COLOR(SGL_RED));
sgl_obj_set_set_style(obj, SGL_STYLE_BORDER_COLOR, SGL_COLOR(SGL_RED));
...

...
sgl_obj_t *obj = sgl_rect_create(NULL);
...
sgl_obj_set_set_style(obj, SGL_STYLE_COLOR, SGL_COLOR(SGL_RED));
sgl_obj_set_set_style(obj, SGL_STYLE_BORDER_COLOR, SGL_COLOR(SGL_RED));
...
```
2. When [CONFIG_SGL_USE_STYLE_UNIFIED_API](file://c:\Users\lsw\Desktop\sgl\source\sgl_config.h#L17-L18) is disabled, please use the following function to set styles:
```c
...
sgl_obj_t *obj = sgl_button_create(NULL);
...
sgl_button_set_style(obj, SGL_STYLE_COLOR, SGL_COLOR(SGL_RED));
sgl_button_set_style(obj, SGL_STYLE_BORDER_COLOR, SGL_COLOR(SGL_RED));
...

...
sgl_obj_t *obj = sgl_rect_create(NULL);
...
sgl_rect_set_style(obj, SGL_STYLE_COLOR, SGL_COLOR(SGL_RED));
sgl_rect_set_style(obj, SGL_STYLE_BORDER_COLOR, SGL_COLOR(SGL_RED));
...

```
