from PIL import Image
import os
rootdir = './'
exit()
for subdir, dirs, files in os.walk(rootdir):
    for file in files:
        s = os.path.join(subdir, file)
        if s[-4:-1] + s[-1] == '.png':
            os.remove(s)

print('1/2')

for subdir, dirs, files in os.walk(rootdir):
    for file in files:
        s = os.path.join(subdir, file)
        if s[-4:-1] + s[-1] == '.jpg':
            im1 = Image.open(s)
            im1.save(s[:-3] + 'png')
            os.remove(s);