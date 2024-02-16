from nicegui import ui


def set_function():
    if(button.text == 'ON'):
        button.classes('drop-shadow bg-green', remove='bg-red')   
        button.set_text('OFF')
    else:
        button.classes('drop-shadow bg-red', remove='bg-green')
        button.set_text('ON')

button = ui.button('ON', on_click=lambda: set_function()).classes('drop-shadow bg-red')

ui.run()