from PIL import Image, ImageDraw, ImageFont
import os
import requests

numbers = [0] * 10

# Define the dimensions of the image
dimension = 28, 28

# Directory containing font files
font_dir = "fonts"

# Create the "database" directory if it doesn't exist
if not os.path.exists("database"):
    os.makedirs("database")

# List all font files in the "fonts" directory
font_files = [f for f in os.listdir(font_dir) if f.endswith(".ttf")]

# Set a threshold value for pixel intensity
threshold = 128

# Loop through the font files
length = len(font_files)
c = 0
for font_file in font_files:
    print(str(c) + ' / ' + str(length))
    c += 1
    font_path = os.path.join(font_dir, font_file)

    # Create a font
    font = ImageFont.truetype(font_path, 30)

    # Loop through the digits
    for digit in range(10):
        # Create a new image with a black background
        image = Image.new("L", dimension, "black")

        if digit == 0:
            image.save(f"database/{digit}_{numbers[digit]}.png")
            numbers[digit] += 1
            continue


        draw = ImageDraw.Draw(image)

        # Calculate text size and position to center the digit
        text = str(digit)
        text_width, text_height = 28, 28

        # Get the bounding box of the text
        bbox = font.getbbox(text)

        # Calculate the position to center the text vertically
        x = (dimension[0] - text_width) / 2
        y = (dimension[1] - bbox[3]) / 2 - bbox[1] + 4

        # Draw the digit on the image in white color
        draw.text((x, y), text, fill="white", font=font)

        # Save the image as a PNG in the "database" directory
        image.save(f"database/{digit}_{numbers[digit]}.png")
        numbers[digit] += 1

print("Images created successfully for each font in the 'fonts' directory")
print(numbers)