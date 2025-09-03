## Widget Styles
The `SGL` widget style setting functions are as follows:     
When **enabling** [CONFIG_SGL_USE_STYLE_UNIFIED_API](file://c:\Users\lsw\Desktop\sgl\source\sgl_config.h#L17-L18), please use the following function to set styles:   
```c
void sgl_obj_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);
```
When **disabling** [CONFIG_SGL_USE_STYLE_UNIFIED_API](file://c:\Users\lsw\Desktop\sgl\source\sgl_config.h#L17-L18), please use the following function to set styles:
```c
void sgl_xxx_set_style(sgl_obj_t *obj, sgl_style_type_t type, size_t value);
```
Where `xxx` is the widget name, such as `sgl_button_set_style`, `sgl_label_set_style`, `sgl_rect_set_style`, etc.


### Button Supported Styles
- SGL_STYLE_POS_X : Set button X coordinate
- SGL_STYLE_POS_Y : Set button Y coordinate  
- SGL_STYLE_SIZE_W : Set button width
- SGL_STYLE_SIZE_H : Set button height
- SGL_STYLE_COLOR : Set button color
- SGL_STYLE_RADIUS : Set button corner radius
- SGL_STYLE_ALPHA : Set button transparency
- SGL_STYLE_PIXMAP : Set button background image
- SGL_STYLE_TEXT : Set button text
- SGL_STYLE_ICON : Set button icon
- SGL_STYLE_ALIGN : Set button text alignment
- SGL_STYLE_TEXT_COLOR : Set button text color
- SGL_STYLE_TEXT_ALPHA : Set button text transparency
- SGL_STYLE_TEXT_MARGIN : Set spacing between button text and button
- SGL_STYLE_TEXT_X_OFFSET : Set button text X-axis offset
- SGL_STYLE_TEXT_Y_OFFSET : Set button text Y-axis offset
- SGL_STYLE_FONT : Set button text font
- SGL_STYLE_BORDER_WIDTH : Set button border width
- SGL_STYLE_BORDER_COLOR : Set button border color

### Label Supported Styles
- SGL_STYLE_POS_X : Set label X coordinate
- SGL_STYLE_POS_Y : Set label Y coordinate
- SGL_STYLE_SIZE_W : Set label width
- SGL_STYLE_SIZE_H : Set label height
- SGL_STYLE_COLOR : Set label color
- SGL_STYLE_BG_COLOR : Set label background color
- SGL_STYLE_ALPHA : Set label transparency
- SGL_STYLE_RADIUS : Set label corner radius
- SGL_STYLE_TEXT : Set label text
- SGL_STYLE_ALIGN : Set label text alignment
- SGL_STYLE_TEXT_COLOR : Set label text color
- SGL_STYLE_FONT : Set label text font
- SGL_STYLE_ICON : Set label icon
- SGL_STYLE_TEXT_MARGIN : Set spacing between label text and label
- SGL_STYLE_TEXT_X_OFFSET : Set label text X-axis offset
- SGL_STYLE_TEXT_Y_OFFSET : Set label text Y-axis offset
- SGL_STYLE_LINE_SPACE : Set label line spacing
- SGL_STYLE_LABEL_TRANSPARENT: Set label to not display background

### Message Box Supported Styles
- SGL_STYLE_POS_X: Set message box X coordinate
- SGL_STYLE_POS_Y : Set message box Y coordinate
- SGL_STYLE_SIZE_W : Set message box width
- SGL_STYLE_SIZE_H : Set message box height
- SGL_STYLE_BG_COLOR : Set message box background color
- SGL_STYLE_RADIUS : Set message box corner radius
- SGL_STYLE_BORDER_COLOR : Set message box border color
- SGL_STYLE_BORDER_WIDTH : Set message box border width
- SGL_STYLE_ALPHA : Set message box transparency
- SGL_STYLE_TEXT : Set message box text
- SGL_STYLE_MSGBOX_TEXT : Set message box text, same effect as SGL_STYLE_TEXT
- SGL_STYLE_TEXT_COLOR : Set message box text color
- SGL_STYLE_MSGBOX_TEXT_COLOR : Set message box text color, same effect as SGL_STYLE_TEXT_COLOR
- SGL_STYLE_MSGBOX_TITLE : Set message box title
- SGL_STYLE_MSGBOX_TITLE_COLOR : Set message box title color
- SGL_STYLE_MSGBOX_TITLE_ICON : Set message box title icon
- SGL_STYLE_MSGBOX_BUTTON_COLOR : Set message box button color
- SGL_STYLE_MSGBOX_CLOSE_TEXT : Set message box close button text
- SGL_STYLE_MSGBOX_CLOSE_TEXT_COLOR : Set message box close button text color
- SGL_STYLE_MSGBOX_CLOSE_ICON : Set message box close button icon
- SGL_STYLE_MSGBOX_APPLY_TEXT : Set message box Apply button text
- SGL_STYLE_MSGBOX_APPLY_TEXT_COLOR : Set message box Apply button text color
- SGL_STYLE_MSGBOX_APPLY_ICON : Set message box Apply button icon
- SGL_STYLE_LINE_SPACE : Set message box line spacing
- SGL_STYLE_FONT : Set message box text font
