from PIL import Image, ImageDraw, ImageFont
import os
import requests

# Define the dimensions of the image
dimension = 28, 28

# Directory containing font files
font_dir = "fonts/"

# Create the "training_set" directory if it doesn't exist
if not os.path.exists("training_set"):
    os.makedirs("training_set")

for i in range(10):
    if not os.path.exists('training_set/' + str(i)):
        os.makedirs('training_set/' + str(i))

f2 = [f for f in os.listdir(font_dir)]

# List all font files in the "fonts" directory
font_files = [j + '/' + f for j in f2 for f in os.listdir(font_dir + j) if f.endswith(".ttf")]

# Set a threshold value for pixel intensity
threshold = 128

indices = [0] * 10

# Loop through the font files
for font_file in font_files:
    font_path = os.path.join(font_dir, font_file)

    # Create a font
    font = ImageFont.truetype(font_path, 30)

    # Loop through the digits
    for digit in range(10):
        # Create a new image with a black background
        image = Image.new("L", dimension, "black")

        if digit == 0:
            image.save(f"training_set/{digit}/{indices[digit]}.png")
            indices[digit] += 1
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

        # Save the image as a PNG in the "training_set" directory
        image.save(f"training_set/{digit}/{indices[digit]}.png")
        indices[digit] += 1

print("Images created successfully for each font in the 'fonts' directory")