import time
import board
import busio

from adafruit_ssd1306 import SSD1306_I2C
from PIL import Image, ImageDraw, ImageFont

# I2C interface using default SDA/SCL (GPIO2/GPIO3)
i2c = busio.I2C(board.SCL, board.SDA)

# Try default SSD1306 address (0x3C), resolution 128x32
display = SSD1306_I2C(128, 32, i2c, addr=0x3C)

# Clear display
display.fill(0)
display.show()

# Create blank image for drawing.
image = Image.new("1", (display.width, display.height))
draw = ImageDraw.Draw(image)

# Load default font.
font = ImageFont.load_default()

# Draw text
draw.text((0, 0), "Hello, Zuno!", font=font, fill=255)
display.image(image)
display.show()

time.sleep(10)
