class Colors:
    BACKGROUND = "#f4f4f9"
    HEADER = "#283593"
    TEXT = "#000000"
    BUTTON = "#4caf50"
    BUTTON_TEXT = "#ffffff"

def apply_styles(widget):
    widget.configure(bg=Colors.BACKGROUND, fg=Colors.TEXT)
