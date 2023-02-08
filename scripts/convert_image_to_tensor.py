import torchvision.transforms as transforms
import torch
from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
import sys
import os

# load image in RGB mode (png files contains additional alpha channel)
img = Image.open('yoho.png').convert('RGB')

# set up transformation to resize the image
resize = transforms.Resize([1080, 1920])

img = resize(img)
img.save("yoho1080.png")
to_tensor = transforms.ToTensor()
# apply transformation and convert to Pytorch tensor
tensor = to_tensor(img)
print(tensor.size())

#show tensor
tensor = tensor.permute(0, 2, 1)

# add another dimension at the front to get NCHW shape
tensor = tensor.unsqueeze(0)
print(tensor.size())
torch.set_printoptions(profile="full")


original_stdout = sys.stdout # Save a reference to the original standard output

with open('image.txt', 'w') as f:
    sys.stdout = f # Change the standard output to the file we created.
    print(tensor)
    sys.stdout = original_stdout #

with open('image.txt', 'r') as infile, open('temp.txt', 'w') as outfile:
    temp = infile.read().replace("(", "").replace(")", "").replace("[", "{").replace("]","}").replace("tensor", "")
    outfile.write(temp)

with open('temp.txt','r') as f:
    with open('input_tensor.h','w') as f2: 
        f2.write("float input_tensor[1][3][1920][1080] = ")
        f2.write(f.read())
        f2.write(';')
os.remove('temp.txt')
os.remove('image.txt')

#reprinting to confirm
#tensor = torch.squeeze(tensor)
#tensor = tensor.permute(1, 2, 0)
#plt.imshow(   tensor )
#plt.savefig('og.png')

#not using matplotlib
#tensor = tensor*255
#tensor = np.array(tensor, dtype=np.uint8)

#img = Image.fromarray(tensor, "RGB")
# Display the Numpy array as Image
#img.show()

# Save the Numpy array as Image
#image_filename = "original1080.jpeg"
#img.save(image_filename)