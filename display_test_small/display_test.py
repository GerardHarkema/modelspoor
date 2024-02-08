# If not yet done please install the luma library by
# git clone https://github.com/rm-hull/luma.examples.git
# cd luma.examples
# sudo -H pip3 install -e . (mind the dot at the end)
import time
import sys
from luma.core.interface.serial import spi
from luma.lcd.device import st7735
from luma.core.render import canvas
from PIL import ImageFont

font_path = '/home/gerard/luma.examples/examples/fonts/ChiKareGo.ttf'
s = spi(port=0, device=0, cs_high=True, gpio_DC=23, gpio_RST=24)
device=st7735(s,rotate=0,width=160,height=128,h_offset=0,v_offset=0,bgr=False)

def primitives(device, draw):
	# Draw a rectangle of the same size of screen
	draw.rectangle(device.bounding_box, outline='white')
	# Draw a rectangle
	draw.rectangle((4, 4, 80, 124), outline='blue', fill=(22, 55, 55))
	# Draw an ellipse
	draw.ellipse((6, 6, 78, 122), outline=(254, 155, 0), fill='green')
	# Draw a triangle
	draw.polygon([(90,124), (100,4), (120,124)], outline='blue', fill='red')
	# Draw an X
	draw.line((130, 4, 155, 124), fill='yellow')
	draw.line((130, 124, 155, 4), fill='yellow')
	# Print 'AZ-Delivery'
	draw.text((10, 60), 'AZ-Delivery', fill='red')
	# Change font adn size of text and print 'AZ-Delivery'
	size = 22
	new_font = ImageFont.truetype(font_path, size)
	draw.text((10, 70), 'AZ-Delivery', font=new_font, fill='red')

def changing_var(device):
	size = 40
	new_font = ImageFont.truetype(font_path, size)

	for i in range(100):
		with canvas(device) as draw:
			draw.text((40, 38), 'Changing var.', fill='red')
			if i < 10:
				draw.text((63, 55),'0{}'.format(str(i)),font=new_font,fill='red')
			else:
				draw.text((63, 55), str(i), font=new_font, fill='red')
			time.sleep(0.02)

print('[Press CTRL + C to end the script!]')
try:
	while True:
		device.backlight(False) # if backlight connected to GPIO18
		print('Testing printing variable.')
		changing_var(device)
		time.sleep(1)
		
		print('Testing basic graphics.')
		with canvas(device) as draw:
			primitives(device, draw)
		time.sleep(3)
		
		print('Testing display ON/OFF.')
		for _ in range(5):
			time.sleep(0.5)
			device.hide()
			time.sleep(0.5)
			device.show()
		print('Testing clear display.')
		time.sleep(2)
		device.clear()
		print()
		time.sleep(2)
		
except KeyboardInterrupt:
	device.backlight(True) # if backlight connected to GPIO18
	device.cleanup()
	print('Script end!')
